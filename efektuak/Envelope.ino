#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

AudioInputI2S        i2sIn;
AudioAnalyzePeak    peak1;
AudioFilterStateVariable filter1;
AudioMixer4         mixerOut;
AudioOutputI2S      i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS AUTO-WAH =================
const float BASE_FREQ   = 300.0;   // Hz (wah cerrado)
const float SWEEP_RANGE = 2000.0;  // cuánto se abre el wah
const float RESONANCE   = 2.8;     // Q (alto = wah marcado)
const float SENSITIVITY = 3.0;     // sensibilidad a la dinámica
const float DRY_GAIN    = 0.0;     // 0 = solo wah
const float WET_GAIN    = 1.0;
// ======================================================

// Conexiones
AudioConnection patchCord1(i2sIn, 0, filter1, 0);
AudioConnection patchCord2(filter1, 1, mixerOut, 0);  // salida band-pass
AudioConnection patchCord3(i2sIn, 0, mixerOut, 1);    // señal seca
AudioConnection patchCord4(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord5(mixerOut, 0, i2sOut, 1);
AudioConnection patchCord6(i2sIn, 0, peak1, 0);

void setup() {
  AudioMemory(180);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  mixerOut.gain(0, WET_GAIN);
  mixerOut.gain(1, DRY_GAIN);

  // Configurar filtro
  filter1.frequency(BASE_FREQ);
  filter1.resonance(RESONANCE);
}

void loop() {
  if (peak1.available()) {
    float env = peak1.read();   // 0.0 – 1.0

    // Escalar envolvente
    float sweep = env * SENSITIVITY;
    if (sweep > 1.0) sweep = 1.0;

    float freq = BASE_FREQ + sweep * SWEEP_RANGE;

    // límites de seguridad
    if (freq < 150.0)  freq = 150.0;
    if (freq > 3500.0) freq = 3500.0;

    filter1.frequency(freq);
  }
}

