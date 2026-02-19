#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2sIn;          //xy=110.33330535888672,73.66666412353516
AudioEffectChorus        chorus1;        //xy=255.99999564034596,165.1428527832031
AudioAmplifier           amp1;           //xy=427.42856706891735,171.71428135463168
AudioMixer4              mixer1;          //xy=560.0951538085938,158.42859268188477
AudioOutputI2S           i2sOut;         //xy=713.6666679382324,75.33332443237305
AudioConnection          patchCord1(i2sIn, 0, chorus1, 0);
AudioConnection patchCordDry(i2sIn, 0, mixer1, 1);
//AudioConnection          patchCord2(i2sIn, 1, i2sOut, 1);
AudioConnection          patchCord3(chorus1, amp1);
AudioConnection          patchCord4(amp1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, i2sOut, 0);
AudioConnection patchCord6(mixer1, 0, i2sOut, 1);

AudioControlSGTL5000     sgtl5000_1;     //xy=70.33333206176758,23.66667366027832
// GUItool: end automatically generated code
#define CHORUS_DELAY_LENGTH 4096
short chorusDelayLine[CHORUS_DELAY_LENGTH];



// <<<<<<<<<<<<<<>>>>>>>>>>>>>>>>
void setup() {
  
  Serial.begin(9600);

  AudioMemory(120);
  chorus1.begin(chorusDelayLine, CHORUS_DELAY_LENGTH, 6);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);// berez 0.5 dago
  amp1.gain(1.0);
  //mixer1.gain(0, 0.5); // berez 0.5
  //mixer1.gain(1, 0.5); // berez 0.5
  mixer1.gain(0, 1.0); // solo chorus
  mixer1.gain(1, 1.0); // sin seco
 

}

void loop()
{
  }