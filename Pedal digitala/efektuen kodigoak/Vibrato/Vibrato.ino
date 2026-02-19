#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

// ================= AUDIO =================
AudioInputI2S        i2sIn;
AudioEffectDelay     delay1;
AudioMixer4          mixerOut;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS VIBRATO =================
const float BASE_DELAY_MS = 3.0;    // delay base muy corto 5.0
const float DEPTH_MS = 1.0;         // profundidad del vibrato 2.0
const float LFO_RATE_HZ = 3.0;      // frecuencia del LFO (ciclos por segundo) 5.0
const float DRY_GAIN = 0.0;         // mezcla señal directa
const float WET_GAIN = 0.5;         // mezcla señal vibrato

// ================= PATCH =================
AudioConnection patch1(i2sIn, 0, delay1, 0);
AudioConnection patch2(delay1, 0, mixerOut, 1);
AudioConnection patch3(i2sIn, 0, mixerOut, 0); // dry
AudioConnection patch4(mixerOut, 0, i2sOut, 0);
AudioConnection patch5(mixerOut, 0, i2sOut, 1);

void setup() {
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);

  mixerOut.gain(0, DRY_GAIN);
  mixerOut.gain(1, WET_GAIN);

  delay1.delay(0, BASE_DELAY_MS);
}

void loop() {
  float t = millis() / 1000.0;
  float lfo = sinf(2.0 * PI * LFO_RATE_HZ * t); // LFO senoidal

  float delayTime = BASE_DELAY_MS + DEPTH_MS * lfo;

  // limitar delay
  if (delayTime < 0.5) delayTime = 0.5;
  if (delayTime > 20.0) delayTime = 20.0;

  delay1.delay(0, delayTime);
}
