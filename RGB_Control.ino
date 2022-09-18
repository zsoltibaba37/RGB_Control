/*
  RGB Control
*/

float version = 0.01;

// ------------------ LEDS ------------------
// ------------------ LEDS ------------------
const int c = 3;
String ledNames[c] = {"Red:", "Green:", "Blue:"};

int RED_L = 10;
int GREEN_L = 11;
int BLUE_L = 9;
int *leds[c] = { &RED_L, &GREEN_L, &BLUE_L };

int incR = 5;
int incG = 8;
int incB = 12;
int *increments[c] = { &incR, &incG, &incB};

int brR = 0;
int brG = 0;
int brB = 0;
int *brights[c] = { &brR, &brG, &brB};

// ------------------ POTM ------------------
// ------------------ POTM ------------------
const int potR = A0;
const int sNum = 200; // Sample number
void smoothAnalog();
uint32_t brightness;
int lastBrightness = 0;

// ------------------ BUTTON ------------------
// ------------------ BUTTON ------------------
int ButtonSW = 3;
bool ButState;
bool ButLastState;
bool LedState = false;
void sensButton();
void OnDisplay();
void OffDisplay();

// ------------------ I2C Oled ------------------
// ------------------ I2C Oled ------------------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ------------------ SETUP ------------------
// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);
  Serial.println("---RGB--Control---");

  for (auto a : leds) {
    pinMode(*a, OUTPUT);
  }

  pinMode(ButtonSW, INPUT_PULLUP);
  ButLastState = digitalRead(ButtonSW);

  // --------------- OLED ---------------
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D or 0x3C for 128x64
    Serial.println(F("-- SSD1306 allocation failed --"));
    for (;;);
  }

  // ------------------ INITIAL DISPLAY  ------------------
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(49, 10);
  display.println("RGB");
  display.setCursor(25, 30);
  display.println("CONTROL");
  display.display();
  delay(1000);
  display.clearDisplay();
}

// ------------------ LOOP ------------------
// ------------------ LOOP ------------------
void loop() {
  OffDisplay();
  sensButton();

  while ( LedState ) {
    sensButton();
    smoothAnalog();
    for (auto a : leds) {
      analogWrite(*a, brightness);
    }
    OnDisplay();
  }

  if ( !LedState ) {
    for (auto a : leds) {
      digitalWrite(*a, LOW);
    }
  }
}

// ------------------ VOIDS ------------------
// ------------------ VOIDS ------------------

// ---------------- Read Analog ----------------
void smoothAnalog() {
  uint32_t raw = 0;
  for ( int i = 0; i < sNum; i++ ) {
    raw += analogRead(potR);
    //delay(1);
  }
  raw = int(raw / sNum);
  brightness = map(raw, 0, 810, 0, 255);
  if (brightness <= 10) {
    brightness = 0;
  }
  if (brightness >= 248) {
    brightness = 255;
  }
}

// ------------------ Button ------------------
void sensButton() {
  ButState = digitalRead(ButtonSW);
  if ( ButLastState == true && ButState == false) {
    LedState = !LedState;
    //digitalWrite(RED_L, LedState);
    ButLastState = false;
  }
  if (ButState == HIGH) {
    ButLastState = true;
  }
}

// ------------------ Displays ------------------
void OnDisplay() {
  display.clearDisplay();
  display.setCursor(1, 25);
  display.print("Bright:" + (String)brightness);
  display.display();
}

void OffDisplay() {
  display.clearDisplay();
  display.setCursor(1, 25);
  display.println("Press btn.");
  display.display();
}
