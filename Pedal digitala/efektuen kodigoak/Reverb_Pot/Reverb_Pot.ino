#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// ===== AUDIO OBJECTS =====
AudioInputI2S          i2sIn;
AudioFilterStateVariable inputFilter; // filtra graves y agudos
AudioEffectReverb      reverb;
AudioMixer4            mixerOut;
AudioOutputI2S         i2sOut;
AudioControlSGTL5000   sgtl5000_1;

// ===== CONNECTIONS =====
AudioConnection patchCord1(i2sIn, 0, inputFilter, 0);   // input → filtro
AudioConnection patchCord2(inputFilter, 0, reverb, 0);  // filtro → reverb
AudioConnection patchCord3(reverb, 0, mixerOut, 0);     // wet
AudioConnection patchCord4(i2sIn, 1, mixerOut, 1);      // dry R
AudioConnection patchCord5(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord6(mixerOut, 0, i2sOut, 1);

// ===== POT =====
const int potPin = A0;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000) {}

  AudioMemory(20);
  analogReadResolution(10);

  // ===== AUDIO SHIELD =====
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  // ===== FILTRO DE ENTRADA =====
  inputFilter.frequency(1500);  // low-pass para limpiar agudos acumulados
  inputFilter.resonance(0.6);   // Q moderado

  // ===== REVERB =====
  reverb.reverbTime(0.5);  // valor inicial (0.0 - 1.0)

  // ===== MEZCLA =====
  mixerOut.gain(0, 0.85); // wet más alto → pot se nota
  mixerOut.gain(1, 0.65); // dry ligeramente más bajo
}

void loop() {
  int potValue = analogRead(potPin);
  float norm = potValue / 1023.0;

  // Curva exponencial más pronunciada → más dinámica del pot
  float reverbLevel = norm * norm * 0.95; // casi 1.0 máximo
  reverb.reverbTime(reverbLevel);

  Serial.println(reverbLevel, 3);
  delay(10);
}
