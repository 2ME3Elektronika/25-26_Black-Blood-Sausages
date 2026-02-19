#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

AudioInputI2S        i2sIn;
AudioEffectDelay     delay1;
AudioMixer4          feedbackMixer;
AudioAmplifier       amp1;
AudioMixer4          mixer1;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// ================= PARÁMETROS FLANGER =================
const float BASE_DELAY_MS = 0.8;    // delay base (muy corto)
const float DEPTH_MS      = 0.8;    // barrido
const float LFO_RATE      = 0.25;   // Hz (jet lento)
const float FEEDBACK_GAIN = 0.75;   // REALIMENTACIÓN 🔥

// =====================================================

// Conexiones
AudioConnection patchCord1(i2sIn, 0, feedbackMixer, 0);   // Entrada
AudioConnection patchCord2(feedbackMixer, 0, delay1, 0); // → Delay
AudioConnection patchCord3(delay1, 0, amp1, 0);          // Delay → Amp
AudioConnection patchCord4(amp1, 0, mixer1, 0);          // Wet
AudioConnection patchCord5(delay1, 0, feedbackMixer, 1); // Feedback loop
AudioConnection patchCord6(mixer1, 0, i2sOut, 0);
AudioConnection patchCord7(mixer1, 0, i2sOut, 1);

void setup() {
  AudioMemory(150);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  // Feedback mixer
  feedbackMixer.gain(0, 1.0);          // señal directa
  feedbackMixer.gain(1, FEEDBACK_GAIN); // feedback

  // Salida
  amp1.gain(1.2);
  mixer1.gain(0, 1.0);  // 100% wet

  // Inicializar delay
  delay1.delay(0, BASE_DELAY_MS);
}

void loop() {
  float t = millis() / 1000.0;

  // LFO senoidal MANUAL
  float mod = sinf(2.0 * PI * LFO_RATE * t);

  float delayTime = BASE_DELAY_MS + DEPTH_MS * mod;

  // Seguridad
  if (delayTime < 0.1) delayTime = 0.1;
  if (delayTime > 5.0) delayTime = 5.0;

  delay1.delay(0, delayTime);
}
