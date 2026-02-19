#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

AudioInputI2S        i2sIn;

// 4 etapas all-pass usando biquads
AudioFilterBiquad    ap1;
AudioFilterBiquad    ap2;
AudioFilterBiquad    ap3;
AudioFilterBiquad    ap4;

AudioMixer4          mixerOut;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS PHASER =================
const float BASE_FREQ = 700.0;   // Hz (centro del barrido)
const float DEPTH     = 400.0;   // amplitud del barrido600
const float LFO_RATE  = 4.00;    // Hz (lento y profundo)0.25
const float DRY_GAIN  = 0.0;     // 0 = phaser puro
const float WET_GAIN  = 1.0;
// =====================================================

// Conexiones en cascada (all-pass)
AudioConnection patchCord1(i2sIn, 0, ap1, 0);
AudioConnection patchCord2(ap1, 0, ap2, 0);
AudioConnection patchCord3(ap2, 0, ap3, 0);
AudioConnection patchCord4(ap3, 0, ap4, 0);
AudioConnection patchCord5(ap4, 0, mixerOut, 0);
AudioConnection patchCord6(i2sIn, 0, mixerOut, 1);
AudioConnection patchCord7(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord8(mixerOut, 0, i2sOut, 1);

void setup() {
  AudioMemory(200);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  mixerOut.gain(0, WET_GAIN);
  mixerOut.gain(1, DRY_GAIN);

  // Inicialización inicial (se actualizará en loop)
  ap1.setLowpass(0, BASE_FREQ, 0.7);
  ap2.setLowpass(0, BASE_FREQ * 1.3, 0.7);
  ap3.setLowpass(0, BASE_FREQ * 1.6, 0.7);
  ap4.setLowpass(0, BASE_FREQ * 2.0, 0.7);
}

void loop() {
  float t = millis() / 1000.0;

  // LFO senoidal
  float lfo = sinf(2.0 * PI * LFO_RATE * t);

  float freq = BASE_FREQ + DEPTH * lfo;

  // límites de seguridad
  if (freq < 100.0)  freq = 100.0;
  if (freq > 3000.0) freq = 3000.0;

  ap1.setLowpass(0, freq, 0.7);
  ap2.setLowpass(0, freq * 1.3, 0.7);
  ap3.setLowpass(0, freq * 1.6, 0.7);
  ap4.setLowpass(0, freq * 2.0, 0.7);
}


