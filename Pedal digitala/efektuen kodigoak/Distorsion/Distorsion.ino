#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

AudioInputI2S        i2sIn;

// EQ multibanda (low / mid / high)
AudioFilterStateVariable eqLow;
AudioFilterStateVariable eqMid;
AudioFilterStateVariable eqHigh;

// Mixer para combinar bandas
AudioMixer4          mixer1;

// Saturación simulada con recorte manual en mixer
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS METALZONE =================
const float EQ_LOW  = 100.0;    // Hz 120.0
const float EQ_MID  = 500.0;   // Hz 1000.0
const float EQ_HIGH = 8000.0;   // Hz 6000.0
const float GAIN    = 10.0;      // multiplicador de señal para saturar2.55
const float CLIP_MAX = 0.9;     // recorte máximo 0.8
// ========================================================

// Conexiones
AudioConnection patchCord1(i2sIn, 0, eqLow, 0);
AudioConnection patchCord2(i2sIn, 0, eqMid, 0);
AudioConnection patchCord3(i2sIn, 0, eqHigh, 0);

AudioConnection patchCord4(eqLow, 0, mixer1, 0);
AudioConnection patchCord5(eqMid, 0, mixer1, 1);
AudioConnection patchCord6(eqHigh, 0, mixer1, 2);

AudioConnection patchCord7(mixer1, 0, i2sOut, 0);
AudioConnection patchCord8(mixer1, 0, i2sOut, 1);

void setup() {
  AudioMemory(200);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  // Configuración de EQ multibanda
  eqLow.frequency(EQ_LOW);
  eqLow.resonance(1.0);
  eqLow.octaveControl(2.0);

  eqMid.frequency(EQ_MID);
  eqMid.resonance(1.2);
  eqMid.octaveControl(1.0);

  eqHigh.frequency(EQ_HIGH);
  eqHigh.resonance(1.0);
  eqHigh.octaveControl(1.0);

  // Ganancia inicial de mixer (simula amplificación)
  mixer1.gain(0, GAIN);
  mixer1.gain(1, GAIN);
  mixer1.gain(2, GAIN);
  mixer1.gain(3, 0.0);
}

void loop() {
  // Saturación manual por recorte
  // Este ejemplo no accede directamente a la señal, pero GAIN > 1 y el DAC recortará
  // Esto simula la distorsión tipo Metalzone sin usar Waveshaper ni Amplifier
}
