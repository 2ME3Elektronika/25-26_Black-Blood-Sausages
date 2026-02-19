#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// ===== AUDIO OBJECTS =====
AudioInputI2S        i2sIn;
AudioEffectDelay      delay1;
AudioMixer4           mixerFB;    // mezcla señal original + feedback
AudioMixer4           mixerOut;   // mezcla final dry + delay
AudioOutputI2S        i2sOut;

AudioControlSGTL5000  sgtl5000_1;

// ===== CONEXIONES =====
// Dry -> mixerOut
AudioConnection patchCord1(i2sIn, 0, mixerOut, 0);      // dry L
AudioConnection patchCord2(i2sIn, 1, mixerOut, 1);      // dry R
AudioConnection patchCord3(delay1, 0, mixerOut, 2);     // delay -> mixer final
AudioConnection patchCord4(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord5(mixerOut, 0, i2sOut, 1);

// Feedback real: delay + entrada original -> delay
AudioConnection patchCord6(delay1, 0, mixerFB, 0);      // salida delay
AudioConnection patchCord7(i2sIn, 0, mixerFB, 1);       // señal original
AudioConnection patchCord8(mixerFB, 0, delay1, 0);      // mixer -> delay entrada

// ===== POT =====
const int potPin = A0;
float feedback = 0.0;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000) {}

  AudioMemory(100);
  analogReadResolution(10);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  // Mixer salida: dry + delay
  mixerOut.gain(0, 0.65); // dry L
  mixerOut.gain(1, 0.65); // dry R
  mixerOut.gain(2, 0.55); // delay

  // Mixer feedback: señal original + feedback
  mixerFB.gain(0, 0.0);  // feedback inicial
  mixerFB.gain(1, 0.7);  // señal original

  // Delay
  delay1.delay(0, 130); // ms 260 max
}

void loop() {
  // Leer pot
  int potValue = analogRead(potPin);
  float norm = potValue / 1023.0;

  // Curva musical
  feedback = norm * norm * 0.95; // máximo teórico

  // Escalado dinámico para evitar clipping
  float dryLevel = 0.65;    // mixerOut.gain(0)
  float delayLevel = 0.55;  // mixerOut.gain(2)
  float total = dryLevel + delayLevel + feedback;
  if (total > 1.0) {
      feedback *= 1.0 / total;
  }

  mixerFB.gain(0, feedback);

  // Debug opcional
  Serial.println(feedback, 3);
  delay(10);
}
