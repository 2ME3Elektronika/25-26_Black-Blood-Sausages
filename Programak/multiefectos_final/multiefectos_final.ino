#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// ================= AUDIO =================
AudioInputI2S        i2sIn;

// ----- DELAY -----
AudioEffectDelay     delay1;
AudioMixer4          mixerFB;
AudioMixer4          mixerDelayOut;

// ----- REVERB -----
AudioFilterStateVariable inputFilter;
AudioEffectReverb    reverb;
AudioMixer4          mixerReverbOut;

// ----- CHORUS -----
AudioEffectDelay     chorusDelay1;
AudioEffectDelay     chorusDelay2;
AudioMixer4          mixerChorus;

// ----- FLANGER (manual: delay + feedback + amp) -----
AudioEffectDelay     flangerDelay;
AudioMixer4          flangerFBMixer;
AudioAmplifier       flangerAmp;
AudioMixer4          flangerWetMixer;

// ----- PHASER (4 biquads modulados, SOLO WET) -----
AudioFilterBiquad    ph1, ph2, ph3, ph4;
AudioMixer4          phaserWetMixer;

// ----- TREMOLO (AM real: multiply + LFO) -----
AudioEffectMultiply  tremoloMult;
AudioSynthWaveform   tremLFO;           // seno con offset -> 0..1

// ----- AUTO-WAH (envelope -> SVF bandpass, SOLO WET) -----
AudioAnalyzePeak         wahPeak;
AudioFilterStateVariable wahFilter;
AudioMixer4              wahWetMixer;

// ----- VIBRATO (delay corto modulado) -----
AudioEffectDelay     vibratoDelay;

// ----- DISTORTION (Metal-ish: 3 bandas + hard clip por saturación) -----
AudioFilterStateVariable distLow;
AudioFilterStateVariable distMid;
AudioFilterStateVariable distHigh;
AudioMixer4              distMixer;     // combina bandas (ganancia alta => clip)

// ----- DIST GATE (detector de nivel para silenciar hiss en silencio) -----
AudioAnalyzePeak         distPeak;

// ----- FX CH3 BUS (tremolo + autowah + vibrato + dist comparten mixerOut2 ch3) -----
AudioMixer4          fx3Mixer;

// ----- OUTPUT -----
AudioMixer4          mixerOut;    // dry + delay + reverb + chorus
AudioMixer4          mixerOut2;   // (mixerOut) + flanger + phaser + fx3(ch3)
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= CONNECTIONS =================
// Delay
AudioConnection pc1(i2sIn, 0, mixerFB, 1);
AudioConnection pc2(delay1, 0, mixerFB, 0);
AudioConnection pc3(mixerFB, 0, delay1, 0);
AudioConnection pc4(delay1, 0, mixerDelayOut, 0);

// Reverb
AudioConnection pc5(i2sIn, 0, inputFilter, 0);
AudioConnection pc6(inputFilter, 0, reverb, 0);
AudioConnection pc7(reverb, 0, mixerReverbOut, 0);

// Chorus
AudioConnection pc8(i2sIn, 0, chorusDelay1, 0);
AudioConnection pc9(i2sIn, 0, chorusDelay2, 0);
AudioConnection pc10(chorusDelay1, 0, mixerChorus, 0);
AudioConnection pc11(chorusDelay2, 0, mixerChorus, 1);

// Flanger (manual)
AudioConnection pc12(i2sIn, 0, flangerFBMixer, 0);         // entrada
AudioConnection pc13(flangerFBMixer, 0, flangerDelay, 0);  // -> delay
AudioConnection pc14(flangerDelay, 0, flangerAmp, 0);      // -> amp
AudioConnection pc15(flangerAmp, 0, flangerWetMixer, 0);   // wet out
AudioConnection pc16(flangerDelay, 0, flangerFBMixer, 1);  // feedback loop

// Phaser (SOLO WET)
AudioConnection pc25(i2sIn, 0, ph1, 0);
AudioConnection pc26(ph1, 0, ph2, 0);
AudioConnection pc27(ph2, 0, ph3, 0);
AudioConnection pc28(ph3, 0, ph4, 0);
AudioConnection pc29(ph4, 0, phaserWetMixer, 0);

// Tremolo (AM real) -> fx3Mixer ch0
AudioConnection pc31(i2sIn,       0, tremoloMult, 0);
AudioConnection pc32(tremLFO,     0, tremoloMult, 1);
AudioConnection pc33(tremoloMult, 0, fx3Mixer,    0);

// Auto-wah -> fx3Mixer ch1
AudioConnection pc40(i2sIn, 0, wahFilter, 0);
AudioConnection pc41(wahFilter, 1, wahWetMixer, 0);    // band-pass
AudioConnection pc42(i2sIn, 0, wahPeak, 0);
AudioConnection pc43(wahWetMixer, 0, fx3Mixer, 1);

// Vibrato -> fx3Mixer ch2
AudioConnection pc50(i2sIn, 0, vibratoDelay, 0);
AudioConnection pc51(vibratoDelay, 0, fx3Mixer, 2);

// Distortion -> fx3Mixer ch3
AudioConnection pc60(i2sIn,    0, distLow,  0);
AudioConnection pc61(i2sIn,    0, distMid,  0);
AudioConnection pc62(i2sIn,    0, distHigh, 0);

// 0=lowpass, 1=bandpass, 2=highpass
AudioConnection pc63(distLow,  0, distMixer, 0); // LOW
AudioConnection pc64(distMid,  1, distMixer, 1); // MID
AudioConnection pc65(distHigh, 2, distMixer, 2); // HIGH

AudioConnection pc66(distMixer,0, fx3Mixer,  3);

// Detector para gate de distorsión
AudioConnection pc67(i2sIn, 0, distPeak, 0);

// Base buses
AudioConnection pc17(i2sIn, 0, mixerOut, 0);               // dry
AudioConnection pc18(mixerDelayOut, 0, mixerOut, 1);       // delay
AudioConnection pc19(mixerReverbOut, 0, mixerOut, 2);      // reverb
AudioConnection pc20(mixerChorus, 0, mixerOut, 3);         // chorus

AudioConnection pc21(mixerOut, 0, mixerOut2, 0);           // bus base
AudioConnection pc22(flangerWetMixer, 0, mixerOut2, 1);    // flanger (wet)
AudioConnection pc30(phaserWetMixer, 0, mixerOut2, 2);     // phaser (wet)

// fx3Mixer -> mixerOut2 ch3
AudioConnection pc45(fx3Mixer, 0, mixerOut2, 3);

// Output
AudioConnection pc23(mixerOut2, 0, i2sOut, 0);
AudioConnection pc24(mixerOut2, 0, i2sOut, 1);

// ================= CONTROLES =================
const int potPin    = A0;
const int switchPin = 2;
const int encBtn    = 5;

Encoder encoder(3, 4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= ESTADOS =================
enum State { STATE_BYPASS, STATE_EFFECT, STATE_MENU, STATE_EDIT };
enum EffectType {
  EFFECT_DELAY,
  EFFECT_REVERB,
  EFFECT_CHORUS,
  EFFECT_FLANGER,
  EFFECT_PHASER,
  EFFECT_TREMOLO,
  EFFECT_AUTOWAH,
  EFFECT_VIBRATO,
  EFFECT_DISTORTION
};

State currentState = STATE_BYPASS;
EffectType currentEffect = EFFECT_DELAY;

// ================= REVERB TYPES (ROOM / HALL) =================
enum ReverbType { REV_ROOM = 0, REV_HALL = 1 };
ReverbType reverbType = REV_ROOM;

const char* reverbTypeName(ReverbType t){
  return (t==REV_ROOM) ? "ROOM" : "HALL";
}

// ================= PARÁMETROS =================
long lastEncPos = 0;
int menuIndex = 0;

// Delay
float delayTime = 330;
float delayMix  = 0.60;
float delayFB   = 0.0;

// Reverb
float reverbMix  = 0.6;     // <- AHORA: POT controla esto (mix/cantidad)
float reverbSize = 0.5;     // control por encoder (SIZE)
float reverbToneHz = 2500.0f; // control por encoder (TONE)

// Chorus
float chorusMix = 0.6;
float chorusDepth1 = 4.0;
float chorusDepth2 = 6.0;

// Chorus constantes
const float CH_BASE1 = 12.0;
const float CH_BASE2 = 18.0;
const float CH_RATE1 = 0.25;
const float CH_RATE2 = 0.33;
const float CH_GAIN  = 0.35;

// Flanger
float flangeMix      = 0.6;
float flangeRate     = 0.25;
float flangeBaseMs   = 0.8;
float flangeDepthMs  = 0.8;
float flangeFB       = 0.75;
float flangeAmpGain  = 1.2;

// Phaser
const float PH_BASE_FREQ = 700.0f;   // Hz
const float PH_DEPTH     = 900.0f;   // Hz
float       phaserRate   = 0.8f;     // Hz (POT)
const float phaserMix    = 0.60f;    // fijo (solo wet al master)
const float PH_Q         = 1.2f;     // Q

// Tremolo
float       tremoloRate  = 4.0f;     // Hz (POT)
const float TREM_DEPTH   = 0.9f;     // 0..1
const float TREM_BASE    = 0.05f;    // evita silencio total

// Auto-wah
float wahBaseFreq     = 220.0f;   // Hz
float wahSweepRange   = 3200.0f;  // Hz
float wahResonance    = 3.6f;     // Q
float wahSensitivity  = 3.5f;     // POT
const float wahMix    = 1.0f;     // fijo (solo wet)

// Vibrato
float vibBaseMs   = 3.0f;
float vibDepthMs  = 1.0f;   // si todavía "raspa", baja a 0.4
float vibRateHz   = 3.0f;   // POT controla RATE
const float vibDryGain = 0.0f;  // (no se usa: insert)
const float vibWetGain = 0.35f; // headroom

// Distortion (Metal-ish)
const float DIST_EQ_LOW_HZ  = 100.0f;
const float DIST_EQ_MID_HZ  = 500.0f;
const float DIST_EQ_HIGH_HZ = 8000.0f;

// balance tonal entre bandas (luego se multiplica por drive)
const float DIST_LOW_BAL  = 0.80f;
const float DIST_MID_BAL  = 1.10f;
const float DIST_HIGH_BAL = 0.90f;

// drive (POT)
float distDrive = 3.0f;          // multiplicador (provoca clip)
const float distWetGain = 0.85f; // salida al bus fx3 (baja si satura demasiado)

// ===== NOISE GATE DIST =====
float distGateOpenTh  = 0.004f; // umbral para ABRIR
float distGateCloseTh = 0.002f; // umbral para CERRAR (histeresis)
float distGateGain    = 0.0f;   // ganancia suavizada aplicada a fx3Mixer ch3
bool  distGateOpen    = false;

// ================= HELPERS =================
const char* effectName(EffectType e){
  return (e==EFFECT_DELAY)      ? "DELAY"  :
         (e==EFFECT_REVERB)     ? "REVERB" :
         (e==EFFECT_CHORUS)     ? "CHORUS" :
         (e==EFFECT_FLANGER)    ? "FLANGER":
         (e==EFFECT_PHASER)     ? "PHASER" :
         (e==EFFECT_TREMOLO)    ? "TREMOLO":
         (e==EFFECT_AUTOWAH)    ? "AUTOWAH":
         (e==EFFECT_VIBRATO)    ? "VIBRATO": "DIST";
}

// ================= REVERB PRESETS (NO PLATE) =================
void applyReverbPreset(ReverbType t){
  reverbType = t;

  // Nota: NO tocamos reverbMix porque el POT lo controla en directo
  if(t == REV_ROOM){
    reverbSize   = 0.28f;     // cola corta
    reverbToneHz = 5200.0f;   // más brillante
  } else { // REV_HALL
    reverbSize   = 0.78f;     // cola larga
    reverbToneHz = 2400.0f;   // más oscuro
  }

  // aplica a DSP
  reverb.reverbTime(reverbSize);
  inputFilter.frequency(reverbToneHz);
}

// ================= DIST helper =================
void applyDistGains(){
  distMixer.gain(0, DIST_LOW_BAL  * distDrive);
  distMixer.gain(1, DIST_MID_BAL  * distDrive);
  distMixer.gain(2, DIST_HIGH_BAL * distDrive);
  distMixer.gain(3, 0.0f);
}

// ================= DIST GATE =================
void handleDistGate(){
  if(currentEffect != EFFECT_DISTORTION) return;
  if(currentState  == STATE_BYPASS)      return;

  static uint32_t last = 0;
  if (millis() - last < 5) return;  // ~200 Hz
  last = millis();

  // Lee nivel pico
  static float env = 0.0f;
  if(distPeak.available()){
    env = distPeak.read();   // 0..1
    env = sqrtf(env);
  } else {
    env *= 0.95f;           // decaimiento suave si no hay muestra nueva
  }

  // Histeresis
  if(!distGateOpen && env > distGateOpenTh)  distGateOpen = true;
  if( distGateOpen && env < distGateCloseTh) distGateOpen = false;

  float target = distGateOpen ? distWetGain : 0.0f;

  const float kOpen  = 0.35f;
  const float kClose = 0.06f;
  float k = distGateOpen ? kOpen : kClose;

  distGateGain += k * (target - distGateGain);
  fx3Mixer.gain(3, distGateGain);
}

// ================= SETUP =================
void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(encBtn, INPUT_PULLUP);

  Serial.begin(115200);
  AudioMemory(280);
  analogReadResolution(10);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  // Mixer out (dry + 3 FX)
  mixerOut.gain(0, 0.5); // dry
  mixerOut.gain(1, 0.0); // delay
  mixerOut.gain(2, 0.0); // reverb
  mixerOut.gain(3, 0.0); // chorus

  // Mixer final (bus + flanger + phaser + fx3)
  mixerOut2.gain(0, 1.0); // bus base
  mixerOut2.gain(1, 0.0); // flanger off
  mixerOut2.gain(2, 0.0); // phaser off
  mixerOut2.gain(3, 0.0); // fx3 off

  // Delay
  mixerDelayOut.gain(0, delayMix);
  mixerFB.gain(0, 0.0);
  mixerFB.gain(1, 0.7);
  delay1.delay(0, delayTime);

  // Reverb (init)
  mixerReverbOut.gain(0, reverbMix);
  inputFilter.frequency(reverbToneHz);
  inputFilter.resonance(0.35);
  reverb.reverbTime(reverbSize);

  // Chorus
  mixerChorus.gain(0, 0.0);
  mixerChorus.gain(1, 0.0);
  chorusDelay1.delay(0, CH_BASE1);
  chorusDelay2.delay(0, CH_BASE2);

  // Flanger
  flangerFBMixer.gain(0, 1.0);
  flangerFBMixer.gain(1, flangeFB);
  flangerAmp.gain(flangeAmpGain);
  flangerWetMixer.gain(0, 1.0);
  flangerDelay.delay(0, flangeBaseMs);

  // Phaser
  phaserWetMixer.gain(0, 1.0);
  ph1.setLowpass(0, PH_BASE_FREQ,        PH_Q);
  ph2.setLowpass(0, PH_BASE_FREQ * 1.3f, PH_Q);
  ph3.setLowpass(0, PH_BASE_FREQ * 1.6f, PH_Q);
  ph4.setLowpass(0, PH_BASE_FREQ * 2.0f, PH_Q);

  // Tremolo LFO (AM real)
  tremLFO.begin(WAVEFORM_SINE);
  tremLFO.frequency(tremoloRate);
  tremLFO.amplitude(0.0f);
  tremLFO.offset(1.0f);

  // Auto-wah init
  wahWetMixer.gain(0, 1.25f);
  wahFilter.frequency(wahBaseFreq);
  wahFilter.resonance(wahResonance);

  // Vibrato init
  vibratoDelay.delay(0, vibBaseMs);

  // Distortion init (3 bandas usando StateVariable)
  distLow.frequency(DIST_EQ_LOW_HZ);
  distLow.resonance(1.0f);
  distLow.octaveControl(2.0f);

  distMid.frequency(DIST_EQ_MID_HZ);
  distMid.resonance(1.2f);
  distMid.octaveControl(1.0f);

  distHigh.frequency(DIST_EQ_HIGH_HZ);
  distHigh.resonance(1.0f);
  distHigh.octaveControl(1.0f);

  applyDistGains();

  // fx3Mixer init (todo off)
  fx3Mixer.gain(0, 0.0f); // tremolo
  fx3Mixer.gain(1, 0.0f); // autowah
  fx3Mixer.gain(2, 0.0f); // vibrato
  fx3Mixer.gain(3, 0.0f); // dist

  lcd.init();
  lcd.backlight();

  // Preset inicial
  applyReverbPreset(REV_ROOM);
}

// ================= LOOP =================
void loop() {
  handleSwitch();
  handlePot();
  handleEncoder();
  handleChorusLFO();
  handleFlangerLFO();
  handlePhaserLFO();
  handleTremoloLFO();
  handleAutoWah();
  handleVibrato();
  handleDistGate();
  updateLCD();

  static uint32_t lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();
    Serial.print("CPU cur/max: ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" / ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("   MEM cur/max: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" / ");
    Serial.println(AudioMemoryUsageMax());
  }
}

// ================= SWITCH =================
void handleSwitch(){
  static unsigned long lastTime = 0;
  bool sw = !digitalRead(switchPin);
  static bool prev = false;

  if(sw && !prev && millis()-lastTime > 150){
    currentState = (currentState==STATE_BYPASS)?STATE_EFFECT:STATE_BYPASS;
    lastTime = millis();
  }
  prev = sw;

  if(currentState==STATE_BYPASS) deactivateEffects();
  else activateEffect();
}

// ================= POT =================
void handlePot(){
  float norm = analogRead(potPin)/1023.0f;
  norm *= norm;

  if(currentEffect==EFFECT_DELAY){
    delayFB = norm * 0.95f;
    mixerFB.gain(0, delayFB);
  }
  else if(currentEffect==EFFECT_REVERB){
    // POT = MIX (cantidad)
    reverbMix = constrain(norm, 0.0f, 1.0f);
    mixerReverbOut.gain(0, reverbMix);

    // Si está activo el efecto, actualiza también el envío al master (mixerOut ch2)
    if(currentState != STATE_BYPASS){
      mixerOut.gain(2, reverbMix);
    }
  }
  else if(currentEffect==EFFECT_CHORUS){
    chorusDepth1 = 2.0f + norm*6.0f;
    chorusDepth2 = 3.0f + norm*6.0f;
  }
  else if(currentEffect==EFFECT_FLANGER){
    flangeFB = norm * 0.95f;
    flangerFBMixer.gain(1, flangeFB);
  }
  else if(currentEffect==EFFECT_PHASER){
    phaserRate = 0.05f + norm * 5.95f;
  }
  else if(currentEffect==EFFECT_TREMOLO){
    tremoloRate = 0.2f + norm * 11.8f;
  }
  else if(currentEffect==EFFECT_AUTOWAH){
    wahSensitivity = 0.6f + norm * 5.4f;
  }
  else if(currentEffect==EFFECT_VIBRATO){
    vibRateHz = 0.2f + norm * 10.0f; // 0.2..10.2 Hz
  }
  else if(currentEffect==EFFECT_DISTORTION){
    distDrive = 1.2f + norm * 10.0f;   // 1.2 .. 11.2
    applyDistGains();
  }
}

// ================= CHORUS LFO =================
void handleChorusLFO(){
  if(currentEffect!=EFFECT_CHORUS) return;
  if(currentState==STATE_BYPASS) return;

  float t = millis()/1000.0f;
  float d1 = CH_BASE1 + chorusDepth1*sinf(2*PI*CH_RATE1*t);
  float d2 = CH_BASE2 + chorusDepth2*sinf(2*PI*CH_RATE2*t + PI/2);

  chorusDelay1.delay(0, constrain(d1, 5, 30));
  chorusDelay2.delay(0, constrain(d2, 5, 30));
}

// ================= FLANGER LFO =================
void handleFlangerLFO(){
  if(currentEffect!=EFFECT_FLANGER) return;
  if(currentState==STATE_BYPASS) return;

  static uint32_t last = 0;
  if (millis() - last < 3) return;
  last = millis();

  float t = millis() / 1000.0f;
  float mod = sinf(2.0f * PI * flangeRate * t);
  float targetDt = flangeBaseMs + flangeDepthMs * mod;

  if (targetDt < 0.3f) targetDt = 0.3f;
  if (targetDt > 5.0f) targetDt = 5.0f;

  static float dt = 0.8f;
  dt += 0.12f * (targetDt - dt);

  flangerDelay.delay(0, dt);
}

// ================= PHASER LFO =================
void handlePhaserLFO(){
  if(currentEffect != EFFECT_PHASER) return;
  if(currentState == STATE_BYPASS) return;

  static uint32_t last = 0;
  if (millis() - last < 8) return;
  last = millis();

  float t = millis() / 1000.0f;
  float lfo = sinf(2.0f * PI * phaserRate * t);

  float targetFreq = PH_BASE_FREQ + PH_DEPTH * lfo;
  if (targetFreq < 100.0f)  targetFreq = 100.0f;
  if (targetFreq > 3000.0f) targetFreq = 3000.0f;

  static float smoothedFreq = PH_BASE_FREQ;
  smoothedFreq += 0.15f * (targetFreq - smoothedFreq);

  AudioNoInterrupts();
  ph1.setLowpass(0, smoothedFreq,        PH_Q);
  ph2.setLowpass(0, smoothedFreq * 1.3f, PH_Q);
  ph3.setLowpass(0, smoothedFreq * 1.6f, PH_Q);
  ph4.setLowpass(0, smoothedFreq * 2.0f, PH_Q);
  AudioInterrupts();
}

// ================= TREMOLO LFO =================
void handleTremoloLFO(){
  if(currentEffect != EFFECT_TREMOLO) return;
  if(currentState == STATE_BYPASS) return;

  tremLFO.frequency(tremoloRate);
}

// ================= AUTO-WAH =================
void handleAutoWah(){
  if(currentEffect != EFFECT_AUTOWAH) return;
  if(currentState  == STATE_BYPASS)   return;

  static uint32_t last = 0;
  if (millis() - last < 4) return;
  last = millis();

  if(!wahPeak.available()) return;

  float env = wahPeak.read();
  env = sqrtf(env);

  float sweep = env * wahSensitivity;
  if(sweep > 1.0f) sweep = 1.0f;

  float targetFreq = wahBaseFreq + sweep * wahSweepRange;
  if(targetFreq < 150.0f)  targetFreq = 150.0f;
  if(targetFreq > 3500.0f) targetFreq = 3500.0f;

  static float f = 220.0f;
  f += 0.18f * (targetFreq - f);

  wahFilter.frequency(f);
}

// ================= VIBRATO =================
void handleVibrato(){
  if(currentEffect != EFFECT_VIBRATO) return;
  if(currentState  == STATE_BYPASS)   return;

  static uint32_t last = 0;
  if (millis() - last < 8) return;   // ~125 Hz
  last = millis();

  float t = millis() / 1000.0f;
  float lfo = sinf(2.0f * PI * vibRateHz * t); // -1..1

  float targetDelay = vibBaseMs + vibDepthMs * lfo;

  if (targetDelay < 1.5f) targetDelay = 1.5f;
  if (targetDelay > 6.0f) targetDelay = 6.0f;

  static float d = 3.0f;
  d += 0.08f * (targetDelay - d);

  vibratoDelay.delay(0, d);
}

// ================= ENCODER =================
void handleEncoder(){
  long pos = encoder.read()/4;
  bool click = !digitalRead(encBtn);
  static bool lastClick = false;

  if(pos != lastEncPos){
    int diff = (int)(pos - lastEncPos);
    lastEncPos = pos;

    if(currentState == STATE_EFFECT){
      currentEffect = (EffectType)((currentEffect + diff + 9) % 9);

      if((currentEffect == EFFECT_PHASER || currentEffect == EFFECT_TREMOLO ||
          currentEffect == EFFECT_AUTOWAH || currentEffect == EFFECT_VIBRATO ||
          currentEffect == EFFECT_DISTORTION) &&
         (currentState==STATE_MENU || currentState==STATE_EDIT)){
        currentState = STATE_EFFECT;
      }
    }
    else if(currentState == STATE_MENU){
      if(currentEffect == EFFECT_PHASER || currentEffect == EFFECT_TREMOLO ||
         currentEffect == EFFECT_AUTOWAH || currentEffect == EFFECT_VIBRATO ||
         currentEffect == EFFECT_DISTORTION){
        currentState = STATE_EFFECT;
        menuIndex = 0;
        return;
      }

      int maxIdx =
        (currentEffect==EFFECT_DELAY)   ? 2 :
        (currentEffect==EFFECT_FLANGER) ? 2 :
        (currentEffect==EFFECT_REVERB)  ? 4 : // TYPE, MIX(POT), SIZE, TONE, RETURN
                                         1;

      menuIndex = constrain(menuIndex + diff, 0, maxIdx);
    }
    else if(currentState == STATE_EDIT){
      if(currentEffect == EFFECT_PHASER || currentEffect == EFFECT_TREMOLO ||
         currentEffect == EFFECT_AUTOWAH || currentEffect == EFFECT_VIBRATO ||
         currentEffect == EFFECT_DISTORTION){
        currentState = STATE_EFFECT;
        menuIndex = 0;
        return;
      }

      if(currentEffect==EFFECT_DELAY){
        if(menuIndex==0) delayTime = constrain(delayTime + diff*5, 20, 600);
        else if(menuIndex==1) mixerDelayOut.gain(0, delayMix = constrain(delayMix + diff*0.05f, 0, 1));
        delay1.delay(0, delayTime);
      }
      else if(currentEffect==EFFECT_REVERB){
        // MENU REVERB:
        // 0 TYPE, 1 MIX (solo lectura, POT), 2 SIZE, 3 TONE
        if(menuIndex==0){
          int v = (int)reverbType + diff;
          while(v < 0) v += 2;
          v %= 2;
          applyReverbPreset((ReverbType)v);
        }
        else if(menuIndex==2){
          reverbSize = constrain(reverbSize + diff*0.05f, 0.0f, 1.0f);
          reverb.reverbTime(reverbSize);
        }
        else if(menuIndex==3){
          reverbToneHz = constrain(reverbToneHz + diff*150.0f, 400.0f, 9000.0f);
          inputFilter.frequency(reverbToneHz);
        }
        // menuIndex==1 -> MIX lo maneja el POT
      }
      else if(currentEffect==EFFECT_CHORUS){
        if(menuIndex==0) mixerOut.gain(3, chorusMix = constrain(chorusMix + diff*0.05f, 0, 1));
      }
      else if(currentEffect==EFFECT_FLANGER){
        if(menuIndex==0){
          flangeMix = constrain(flangeMix + diff*0.05f, 0, 1);
        } else if(menuIndex==1){
          flangeRate = constrain(flangeRate + diff*0.05f, 0.05f, 5.0f);
        }
      }
    }
  }

  if(click && !lastClick){
    if(currentEffect == EFFECT_PHASER || currentEffect == EFFECT_TREMOLO ||
       currentEffect == EFFECT_AUTOWAH || currentEffect == EFFECT_VIBRATO ||
       currentEffect == EFFECT_DISTORTION){
      // nada
    } else {
      int lastIdx =
        (currentEffect==EFFECT_DELAY)   ? 2 :
        (currentEffect==EFFECT_FLANGER) ? 2 :
        (currentEffect==EFFECT_REVERB)  ? 4 :
                                         1;

      if(currentState==STATE_EFFECT){ currentState=STATE_MENU; menuIndex=0; }
      else if(currentState==STATE_MENU)
        currentState = (menuIndex==lastIdx)?STATE_EFFECT:STATE_EDIT;
      else if(currentState==STATE_EDIT) currentState=STATE_MENU;
    }
  }
  lastClick = click;
}

// ================= AUDIO ROUTING =================
void activateEffect(){
  deactivateEffects();

  if(currentEffect==EFFECT_DELAY){
    mixerOut.gain(1, delayMix);
    mixerFB.gain(0, delayFB);
  }
  else if(currentEffect==EFFECT_REVERB){
    // reverbMix lo controla el POT en tiempo real
    mixerOut.gain(2, reverbMix);
    mixerReverbOut.gain(0, reverbMix);
  }
  else if(currentEffect==EFFECT_CHORUS){
    mixerChorus.gain(0, CH_GAIN);
    mixerChorus.gain(1, CH_GAIN);
    mixerOut.gain(3, chorusMix);
  }
  else if(currentEffect==EFFECT_FLANGER){
    mixerOut2.gain(1, flangeMix);
  }
  else if(currentEffect==EFFECT_PHASER){
    mixerOut2.gain(2, phaserMix);
  }
  else if(currentEffect==EFFECT_TREMOLO){
    mixerOut2.gain(0, 0.0f);
    mixerOut2.gain(3, 1.0f);

    fx3Mixer.gain(0, 1.0f);
    fx3Mixer.gain(1, 0.0f);
    fx3Mixer.gain(2, 0.0f);
    fx3Mixer.gain(3, 0.0f);

    tremLFO.amplitude(0.5f * TREM_DEPTH);
    tremLFO.offset(TREM_BASE + 0.5f * TREM_DEPTH);
    tremLFO.frequency(tremoloRate);
  }
  else if(currentEffect==EFFECT_AUTOWAH){
    mixerOut2.gain(0, 0.0f);
    mixerOut2.gain(3, 1.0f);

    fx3Mixer.gain(0, 0.0f);
    fx3Mixer.gain(1, wahMix);
    fx3Mixer.gain(2, 0.0f);
    fx3Mixer.gain(3, 0.0f);
  }
  else if(currentEffect==EFFECT_VIBRATO){
    mixerOut2.gain(0, 0.0f);
    mixerOut2.gain(3, 1.0f);

    fx3Mixer.gain(0, 0.0f);
    fx3Mixer.gain(1, 0.0f);
    fx3Mixer.gain(2, vibWetGain);
    fx3Mixer.gain(3, 0.0f);
  }
  else if(currentEffect==EFFECT_DISTORTION){
    mixerOut2.gain(0, 0.0f);
    mixerOut2.gain(3, 1.0f);

    fx3Mixer.gain(0, 0.0f);
    fx3Mixer.gain(1, 0.0f);
    fx3Mixer.gain(2, 0.0f);

    distGateOpen = true;
    distGateGain = distWetGain;
    fx3Mixer.gain(3, distWetGain);

    applyDistGains();
  }
}

void deactivateEffects(){
  mixerOut.gain(1, 0);
  mixerOut.gain(2, 0);
  mixerOut.gain(3, 0);

  mixerFB.gain(0, 0);
  mixerChorus.gain(0, 0);
  mixerChorus.gain(1, 0);

  mixerOut2.gain(1, 0);
  mixerOut2.gain(2, 0);
  mixerOut2.gain(3, 0);

  // por defecto, deja pasar el bus normal
  mixerOut2.gain(0, 1.0f);

  fx3Mixer.gain(0, 0.0f);
  fx3Mixer.gain(1, 0.0f);
  fx3Mixer.gain(2, 0.0f);
  fx3Mixer.gain(3, 0.0f);

  tremLFO.amplitude(0.0f);
  tremLFO.offset(1.0f);
}

// ================= LCD =================
void updateLCD(){
  static State      lastState  = (State)-1;
  static EffectType lastEffect = (EffectType)-1;
  static int        lastMenu   = -1;
  static float      lastVal    = -12345.0f;

  bool screenChanged = (lastState != currentState) ||
                       (lastEffect != currentEffect) ||
                       (lastMenu != menuIndex);

  bool valueChanged = false;
  float v = lastVal;

  if(currentState == STATE_EDIT &&
     currentEffect != EFFECT_PHASER &&
     currentEffect != EFFECT_TREMOLO &&
     currentEffect != EFFECT_AUTOWAH &&
     currentEffect != EFFECT_VIBRATO &&
     currentEffect != EFFECT_DISTORTION){

    if(currentEffect==EFFECT_DELAY){
      v = (menuIndex==0) ? delayTime : delayMix;
    }
    else if(currentEffect==EFFECT_REVERB){
      // En REVERB, el POT mueve MIX, así que en EDIT mostramos cambios de TYPE/SIZE/TONE
      if(menuIndex==0) v = (float)reverbType;
      else if(menuIndex==2) v = reverbSize;
      else if(menuIndex==3) v = reverbToneHz;
      else v = reverbMix; // MIX (solo lectura)
    }
    else if(currentEffect==EFFECT_CHORUS){
      v = chorusMix;
    }
    else if(currentEffect==EFFECT_FLANGER){
      v = (menuIndex==0) ? flangeMix : flangeRate;
    }

    if(fabsf(v - lastVal) > 0.001f) valueChanged = true;
  }

  if(currentEffect == EFFECT_PHASER || currentEffect == EFFECT_TREMOLO ||
     currentEffect == EFFECT_AUTOWAH || currentEffect == EFFECT_VIBRATO ||
     currentEffect == EFFECT_DISTORTION)
    valueChanged = false;

  if(!screenChanged && !valueChanged) return;

  lastState  = currentState;
  lastEffect = currentEffect;
  lastMenu   = menuIndex;

  if(currentState == STATE_EDIT &&
     currentEffect != EFFECT_PHASER &&
     currentEffect != EFFECT_TREMOLO &&
     currentEffect != EFFECT_AUTOWAH &&
     currentEffect != EFFECT_VIBRATO &&
     currentEffect != EFFECT_DISTORTION){
    lastVal = v;
  }

  if(screenChanged) lcd.clear();

  if(currentState==STATE_BYPASS){
    lcd.setCursor(0,0);
    lcd.print("BYPASS");
    lcd.setCursor(0,1);
    lcd.print(effectName(currentEffect));
    return;
  }

  if(currentState==STATE_EFFECT){
    lcd.setCursor(0,0);
    lcd.print(effectName(currentEffect));
    lcd.setCursor(0,1);

    if(currentEffect==EFFECT_PHASER){
      lcd.print("POT=RATE");
    } else if(currentEffect==EFFECT_TREMOLO){
      lcd.print("POT=RATE");
    } else if(currentEffect==EFFECT_AUTOWAH){
      lcd.print("POT=SENS");
    } else if(currentEffect==EFFECT_VIBRATO){
      lcd.print("POT=RATE");
    } else if(currentEffect==EFFECT_DISTORTION){
      lcd.print("POT=DRIVE");
    } else if(currentEffect==EFFECT_REVERB){
      lcd.print("POT=MIX");
    } else {
      lcd.print("CLICK=MENU");
    }
    return;
  }

  if(currentState==STATE_MENU){
    if(currentEffect==EFFECT_PHASER || currentEffect==EFFECT_TREMOLO ||
       currentEffect==EFFECT_AUTOWAH || currentEffect==EFFECT_VIBRATO ||
       currentEffect==EFFECT_DISTORTION){
      currentState = STATE_EFFECT;
      return;
    }

    lcd.setCursor(0,0);
    lcd.print(effectName(currentEffect));
    lcd.print(" MENU");
    lcd.setCursor(0,1);

    if(currentEffect==EFFECT_DELAY){
      lcd.print(menuIndex==0?"TIME":menuIndex==1?"MIX":"RETURN");
    } else if(currentEffect==EFFECT_FLANGER){
      lcd.print(menuIndex==0?"MIX":menuIndex==1?"RATE":"RETURN");
    } else if(currentEffect==EFFECT_REVERB){
      // (MIX existe en menú para ver el valor, pero se cambia con POT)
      lcd.print(menuIndex==0?"TYPE":menuIndex==1?"MIX":menuIndex==2?"SIZE":menuIndex==3?"TONE":"RETURN");
    } else {
      lcd.print(menuIndex==0?"MIX":"RETURN");
    }
    return;
  }

  if(currentEffect==EFFECT_PHASER || currentEffect==EFFECT_TREMOLO ||
     currentEffect==EFFECT_AUTOWAH || currentEffect==EFFECT_VIBRATO ||
     currentEffect==EFFECT_DISTORTION){
    currentState = STATE_EFFECT;
    return;
  }

  char line[17];
  for(int i=0;i<16;i++) line[i]=' ';
  line[16]='\0';

  lcd.setCursor(0,0);

  if(currentEffect==EFFECT_DELAY){
    if(menuIndex==0){
      snprintf(line, 17, "TIME %4.0f ms     ", delayTime);
    } else if(menuIndex==1){
      snprintf(line, 17, "MIX  %1.2f        ", delayMix);
    } else {
      snprintf(line, 17, "RETURN           ");
    }
    lcd.print("DELAY");
  }
  else if(currentEffect==EFFECT_REVERB){
    if(menuIndex==0)      snprintf(line, 17, "TYPE %-5s      ", reverbTypeName(reverbType));
    else if(menuIndex==1) snprintf(line, 17, "MIX  %1.2f (POT) ", reverbMix);
    else if(menuIndex==2) snprintf(line, 17, "SIZE %1.2f        ", reverbSize);
    else if(menuIndex==3) snprintf(line, 17, "TONE %4.0f Hz     ", reverbToneHz);
    else                  snprintf(line, 17, "RETURN           ");
    lcd.print("REVERB");
  }
  else if(currentEffect==EFFECT_CHORUS){
    snprintf(line, 17, "MIX  %1.2f        ", chorusMix);
    lcd.print("CHORUS");
  }
  else if(currentEffect==EFFECT_FLANGER){
    if(menuIndex==0){
      snprintf(line, 17, "MIX  %1.2f        ", flangeMix);
    } else if(menuIndex==1){
      snprintf(line, 17, "RATE %1.2f Hz     ", flangeRate);
    } else {
      snprintf(line, 17, "RETURN           ");
    }
    lcd.print("FLANGER");
  }

  lcd.setCursor(0,1);
  lcd.print(line);
}