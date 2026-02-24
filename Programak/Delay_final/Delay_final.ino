#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>

// ================= AUDIO =================
AudioInputI2S        i2sIn;
AudioEffectDelay     delay1;
AudioMixer4          mixerFB;
AudioMixer4          mixerOut;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000_1;

// Conexiones
AudioConnection patchCord1(i2sIn, 0, mixerOut, 0);
AudioConnection patchCord2(i2sIn, 1, mixerOut, 1);
AudioConnection patchCord3(delay1, 0, mixerOut, 2);
AudioConnection patchCord4(mixerOut, 0, i2sOut, 0);
AudioConnection patchCord5(mixerOut, 0, i2sOut, 1);

AudioConnection patchCord6(delay1, 0, mixerFB, 0);
AudioConnection patchCord7(i2sIn, 0, mixerFB, 1);
AudioConnection patchCord8(mixerFB, 0, delay1, 0);

// ================= CONTROLES =================
const int potPin    = A0;
const int switchPin = 2;
const int encBtn    = 5;

Encoder encoder(3, 4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= ESTADOS =================
enum State {
  STATE_BYPASS,
  STATE_DELAY,
  STATE_MENU,
  STATE_EDIT
};

State currentState = STATE_BYPASS;

// ================= PARÁMETROS =================
int menuIndex = 0;
long lastEncPos = 0;
bool editing = false;

float delayTime = 130.0;   // ms
float delayMix  = 0.55;    // 0.0 - 1.0
float feedback  = 0.0;

const char* menuItems[] = { "TIME", "MIX", "RETURN" };
const int menuSize = 3;

// ================= SWITCH TOGGLE =================
bool lastSwitchState = HIGH;

// ================= SETUP =================
void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(encBtn, INPUT_PULLUP);

  Serial.begin(115200);
  AudioMemory(100);
  analogReadResolution(10);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  mixerOut.gain(0, 0.65);
  mixerOut.gain(1, 0.65);
  mixerOut.gain(2, 0.55);

  mixerFB.gain(0, 0.0);
  mixerFB.gain(1, 0.7);

  delay1.delay(0, delayTime);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

// ================= LOOP =================
void loop() {
  handleSwitch();
  handlePot();
  handleEncoder();
  updateLCD();
}

// ================= FUNCIONES =================

void handleSwitch() {
  bool sw = !digitalRead(switchPin); // activo LOW
  static unsigned long lastDebounce = 0;
  const unsigned long debounceDelay = 50;

  if (sw != lastSwitchState && (millis() - lastDebounce) > debounceDelay) {
    lastDebounce = millis();

    if (sw) { // solo cuando pulsamos
      if(currentState != STATE_BYPASS) {
          // Si el efecto está activo, pulsar switch desactiva todo
          currentState = STATE_BYPASS;
          mixerOut.gain(2, 0.0);
          mixerFB.gain(0, 0.0);
      } else {
          // Activar delay
          currentState = STATE_DELAY;
          mixerOut.gain(2, delayMix);
          mixerFB.gain(0, feedback);
      }
    }
  }
  lastSwitchState = sw;
}

void handlePot() {
  int potValue = analogRead(potPin);
  float norm = potValue / 1023.0;
  feedback = norm * norm * 0.95;

  if(currentState == STATE_DELAY || currentState == STATE_MENU || currentState == STATE_EDIT){
      mixerFB.gain(0, feedback);
  } else {
      mixerFB.gain(0, 0.0);
  }
}

void handleEncoder() {
  long newPos = encoder.read() / 4;
  bool click = !digitalRead(encBtn);

  static bool lastClick = false;

  if(newPos != lastEncPos){
    int diff = newPos - lastEncPos;
    lastEncPos = newPos;

    // =========================================
    // Girando encoder
    if(currentState == STATE_MENU){
      menuIndex = constrain(menuIndex + diff, 0, menuSize - 1);
    }
    else if(currentState == STATE_EDIT){
      if(menuIndex == 0){ // TIME
        delayTime = constrain(delayTime + diff * 5, 10, 600);
        delay1.delay(0, delayTime);
      }
      else if(menuIndex == 1){ // MIX
        delayMix = constrain(delayMix + diff * 0.05, 0.0, 1.0);
        mixerOut.gain(2, delayMix);
      }
    }
  }

  // =========================================
  // Click del encoder
  if(click && !lastClick){
    if(currentState == STATE_DELAY){
      // Entrar al menú de parámetros
      currentState = STATE_MENU;
    }
    else if(currentState == STATE_MENU){
      if(menuIndex == 2){ // RETURN
        currentState = STATE_DELAY;
      } else {
        currentState = STATE_EDIT; // Entrar en edición de TIME o MIX
      }
    }
    else if(currentState == STATE_EDIT){
      currentState = STATE_MENU; // salir al menú
    }
  }

  lastClick = click;
}

void updateLCD(){
  static State lastState = STATE_BYPASS;
  static int lastMenuIndex = -1;
  static float lastDelayTime = -1;
  static float lastDelayMix = -1;

  // Solo actualizar LCD si cambió algo
  if(lastState == currentState && lastMenuIndex == menuIndex 
     && lastDelayTime == delayTime && lastDelayMix == delayMix) return;

  lastState = currentState;
  lastMenuIndex = menuIndex;
  lastDelayTime = delayTime;
  lastDelayMix = delayMix;

  lcd.clear();

  if(currentState == STATE_BYPASS){
    lcd.setCursor(0,0);
    lcd.print("BYPASS");
  }
  else if(currentState == STATE_DELAY){
    lcd.setCursor(0,0);
    lcd.print("DELAY");
  }
  else if(currentState == STATE_MENU){
    lcd.setCursor(0,0);
    lcd.print("MENU:");
    lcd.setCursor(0,1);
    lcd.print(menuItems[menuIndex]);
  }
  else if(currentState == STATE_EDIT){
    lcd.setCursor(0,0);
    lcd.print(menuItems[menuIndex]);
    lcd.setCursor(0,1);
    if(menuIndex == 0) lcd.print(delayTime);   // TIME en ms
    if(menuIndex == 1) lcd.print(delayMix,2);  // MIX 0.00-1.00
  }
}
