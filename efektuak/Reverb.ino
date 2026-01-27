#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <Bounce.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

// ================= AUDIO =================
AudioInputI2S        i2sIn;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 audioShield;

AudioEffectFreeverb  reverb;
AudioMixer4          mixer;

// ================= PATCHES =================
AudioConnection pDry(i2sIn, 0, mixer, 0);
AudioConnection pRev(i2sIn, 0, reverb, 0);
AudioConnection pRevMix(reverb, 0, mixer, 1);
AudioConnection pOut(mixer, 0, i2sOut, 0);

// ================= UI =================
LiquidCrystal_I2C lcd(0x27, 16, 2);
Encoder encoder(2, 3);
Bounce encBtn(4, 15);
Bounce revSwitch(5, 15);

// ================= MENU =================
enum MenuState { STATUS, REVERB_MENU };
MenuState menuState = STATUS;

enum RevItem { REV_ROOM, REV_DAMP, REV_MIX, REV_RETURN, REV_ITEMS };
RevItem revItem = REV_ROOM;

// ================= FLAGS =================
bool reverbOn = false;
bool editMode = false;

// ================= PARAMETERS =================
float roomSize = 0.6;
float damping  = 0.3;
float revMix   = 0.5;

// ================= LEVELS =================
const float DRY_GAIN = 0.9;
const float REV_GAIN = 1.3;

// ================= ENCODER =================
long lastEnc = 0;

// ================= SETUP =================
void setup() {
  AudioMemory(40);

  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.5);

  reverb.roomsize(roomSize);
  reverb.damping(damping);

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  updateMixer();
  drawStatus();
}

// ================= LOOP =================
void loop() {
  encBtn.update();
  revSwitch.update();

  // Switch efecto
  if (revSwitch.fallingEdge()) {
    reverbOn = !reverbOn;
    menuState = STATUS;
    updateMixer();
    drawStatus();
  }

  // Encoder giro
  long pos = encoder.read() / 4;
  if (pos != lastEnc) {
    int d = pos - lastEnc;
    lastEnc = pos;
    handleTurn(d);
  }

  // Encoder click
  if (encBtn.fallingEdge()) {
    handleClick();
  }
}

// ================= HANDLERS =================
void handleTurn(int d) {

  if (menuState == REVERB_MENU) {
    if (editMode) {
      if (revItem == REV_ROOM) roomSize = constrain(roomSize + d * 0.02, 0.0, 1.0);
      if (revItem == REV_DAMP) damping  = constrain(damping  + d * 0.02, 0.0, 1.0);
      if (revItem == REV_MIX)  revMix   = constrain(revMix   + d * 0.02, 0.0, 1.0);

      reverb.roomsize(roomSize);
      reverb.damping(damping);
      updateMixer();
    } else {
      revItem = (RevItem)constrain(revItem + d, 0, REV_ITEMS - 1);
    }
    drawReverbMenu();
  }
}

void handleClick() {

  if (menuState == STATUS && reverbOn) {
    menuState = REVERB_MENU;
    revItem = REV_ROOM;
    editMode = false;
    drawReverbMenu();
    return;
  }

  if (menuState == REVERB_MENU) {
    if (revItem == REV_RETURN) {
      menuState = STATUS;
      editMode = false;
      drawStatus();
    } else {
      editMode = !editMode;
      drawReverbMenu();
    }
  }
}

// ================= MIXER =================
void updateMixer() {
  mixer.gain(0, DRY_GAIN);
  mixer.gain(1, reverbOn ? revMix * REV_GAIN : 0.0);
}

// ================= DRAW =================
void drawStatus() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(reverbOn ? "REVERB" : "BYPASS");
  lcd.setCursor(0,1);
  lcd.print("Click encoder");
}

void drawReverbMenu() {
  lcd.clear();
  lcd.setCursor(0,0);

  if (revItem == REV_ROOM) lcd.print("> ROOM");
  else if (revItem == REV_DAMP) lcd.print("> DAMP");
  else if (revItem == REV_MIX)  lcd.print("> MIX");
  else {
    lcd.print("> RETURN");
    lcd.setCursor(0,1);
    lcd.print("Back");
    return;
  }

  lcd.setCursor(0,1);
  lcd.print(editMode ? "* " : "  ");

  if (revItem == REV_ROOM) lcd.print(roomSize, 2);
  else if (revItem == REV_DAMP) lcd.print(damping, 2);
  else if (revItem == REV_MIX)  lcd.print(revMix, 2);
}
