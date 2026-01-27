#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=94.39999389648436,52.39999389648436
AudioMixer4              mixer2; //xy=258.39999771118164,408.4000015258789
AudioEffectDelay         delay1;         //xy=317.4444465637207,253.11108779907227
AudioMixer4              mixer1;         //xy=456.4444465637207,220.11108779907227
AudioOutputI2S           i2s1;           //xy=671.4444046020508,162.11109352111816
AudioConnection          patchCord1(i2s2, 0, mixer2, 0);
AudioConnection          patchCord2(i2s2, 1, i2s1, 1);
AudioConnection          patchCord3(mixer2, delay1);
AudioConnection          patchCord4(delay1, 0, mixer1, 0);
AudioConnection          patchCord5(delay1, 1, mixer1, 1);
AudioConnection          patchCord6(delay1, 2, mixer1, 2);
AudioConnection          patchCord7(delay1, 3, mixer1, 3);
AudioConnection          patchCord8(mixer1, 0, i2s1, 0);
AudioConnection          patchCord9(mixer1, 0, mixer2, 0); //berez 1 dago
AudioControlSGTL5000     sgtl5000_1;     //xy=119.4444465637207,287.11108779907227
// GUItool: end automatically generated code

void setup() {
  // allocate enough memory for the delay
  AudioMemory(120);
  
  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
  mixer2.gain (1, 0.9);


  // create 3 delay taps, which connect through a
  // mixer to the right channel output
  //delay1.delay(0, 110);
  //delay1.delay(1, 220);
  delay1.delay(2, 330);
}

void loop() {

}
