#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

AudioInputI2S        i2sIn;
AudioAmplifier       tremoloAmp;
AudioMixer4          mixerOut;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS TREMOLO =================
const float RATE_HZ   = 4.0;   // velocidad del tremolo
const float DEPTH     = 0.9;   // profundidad (0–1)
const float BASE_GAIN = 0.05;  // nivel mínimo (evita silencio total)
const float DRY_GAIN  = 0.0;   // 0 = solo tremolo
const float WET_GAIN  = 1.0;
// =====================================================

// Conexiones
AudioConnection patchCord1(i2sIn, 0, tremoloAmp, 0);
AudioConnection patchCord2(tremoloAmp, 0, mixerOut, 0);
AudioConnection patchCord3(i2sIn, 0, mixerOut, 1);
AudioConnection patchCord4(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord5(mixerOut, 0, i2sOut, 1);

void setup() {
  AudioMemory(120);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  mixerOut.gain(0, WET_GAIN);
  mixerOut.gain(1, DRY_GAIN);

  tremoloAmp.gain(1.0);
}

void loop() {
  float t = millis() / 1000.0;

  // LFO senoidal (0–1)
  float lfo = (sinf(2.0 * PI * RATE_HZ * t) + 1.0) * 0.5;

  // Aplicar profundidad
  float gain = BASE_GAIN + (DEPTH * lfo);

  // Seguridad
  if (gain < 0.0) gain = 0.0;
  if (gain > 1.0) gain = 1.0;

  tremoloAmp.gain(gain);
}
