#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

AudioInputI2S        i2sIn;
AudioEffectDelay     delay1;
AudioEffectDelay     delay2;
AudioMixer4          voiceMixer;
AudioMixer4          mixerOut;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS CHORUS =================
const float BASE_DELAY_1 = 12.0;   // ms
const float BASE_DELAY_2 = 18.0;   // ms
const float DEPTH_1      = 4.0;    // ms
const float DEPTH_2      = 6.0;    // ms
const float LFO_RATE_1   = 0.25;   // Hz
const float LFO_RATE_2   = 0.33;   // Hz
const float VOICE_GAIN   = 0.3;
const float DRY_GAIN     = 0.4;
// =====================================================

// Conexiones
AudioConnection patchCord1(i2sIn, 0, delay1, 0);
AudioConnection patchCord2(i2sIn, 0, delay2, 0);
AudioConnection patchCord3(delay1, 0, voiceMixer, 0);
AudioConnection patchCord4(delay2, 0, voiceMixer, 1);
AudioConnection patchCord5(voiceMixer, 0, mixerOut, 0);
AudioConnection patchCord6(i2sIn, 0, mixerOut, 1);
AudioConnection patchCord7(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord8(mixerOut, 0, i2sOut, 1);

void setup() {
  AudioMemory(180);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  voiceMixer.gain(0, VOICE_GAIN);
  voiceMixer.gain(1, VOICE_GAIN);

  mixerOut.gain(0, 1.0);      // chorus
  mixerOut.gain(1, DRY_GAIN); // señal seca

  delay1.delay(0, BASE_DELAY_1);
  delay2.delay(0, BASE_DELAY_2);
}

void loop() {
  float t = millis() / 1000.0;

  float mod1 = sinf(2.0 * PI * LFO_RATE_1 * t);
  float mod2 = sinf(2.0 * PI * LFO_RATE_2 * t + PI / 2);

  float d1 = BASE_DELAY_1 + DEPTH_1 * mod1;
  float d2 = BASE_DELAY_2 + DEPTH_2 * mod2;

  if (d1 < 5.0)  d1 = 5.0;
  if (d1 > 30.0) d1 = 30.0;
  if (d2 < 5.0)  d2 = 5.0;
  if (d2 > 30.0) d2 = 30.0;

  delay1.delay(0, d1);
  delay2.delay(0, d2);
}



