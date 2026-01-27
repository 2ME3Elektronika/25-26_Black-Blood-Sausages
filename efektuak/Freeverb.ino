#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2sIn;          //xy=110.33330535888672,73.66666412353516
AudioMixer4              mixer2;         //xy=268.66668701171875,280.3333282470703
AudioEffectFreeverb      reverb;         //xy=271.99993896484375,129.99999618530273
AudioMixer4              mixer1;          //xy=468.6665840148926,147.00002098083496
AudioOutputI2S           i2sOut;         //xy=713.6666679382324,75.33332443237305
AudioConnection          patchCord1(i2sIn, 0, mixer2, 0);
AudioConnection          patchCord2(i2sIn, 1, i2sOut, 1);
AudioConnection          patchCord3(mixer2, reverb);
AudioConnection          patchCord4(reverb, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, i2sOut, 0);
AudioConnection          patchCord6(mixer1, 0, mixer2, 0);// berez 1-ean dago
AudioControlSGTL5000     sgtl5000_1;     //xy=70.33333206176758,23.66667366027832
// GUItool: end automatically generated code


void setup() {
  Serial.begin(9600);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);// berez 0.5 dago

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer2.gain(0, 0.9); // hear 90% "wet"
  mixer2.gain(1, 0.1); // and  10% "dry"
}
   
  
void loop() {

}

