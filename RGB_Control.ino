/*
  RGB Control
  0.1 - three Option
  0.2 - remove display, add attachInterrupt to button
*/

float version = 0.2;

// ------------------ LEDS ------------------
// ------------------ LEDS ------------------
const int c = 3;
String ledNames[c] = {"Red:", "Green:", "Blue:"};

int RED_L = 10;
int GREEN_L = 11;
int BLUE_L = 9;
int *leds[c] = { &RED_L, &GREEN_L, &BLUE_L };

int LedState = 0;  // which state the first 0-3
int LedSWState = LOW;
unsigned long prevMillis = 0;
const int blinkDelay = 100;
int LedSWState2 = LOW;
unsigned long prevMillis2 = 0;

// ------------------ POTM ------------------
// ------------------ POTM ------------------
const int potR = A0;
const int sNum = 10; // Sample number
void smoothAnalog();
uint32_t brightness;
int lastBrightness = 0;

// ------------------ BUTTON ------------------
// ------------------ BUTTON ------------------
int ButtonSW = 3;
bool ButLastState;
const int maxStates = 3;

// ------------------ VOIDS ------------------
// ------------------ VOIDS ------------------
void sensButton();
void debounceButton();
void OnDisplay();
void OffDisplay();
void firstState();
void secondState();
void thirdState();
void offState();
void stateDisplay();

bool once = true;

// ------------------ I2C Oled ------------------
// ------------------ I2C Oled ------------------
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//#define SCREEN_ADDRESS 0x3C
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ------------------ SETUP ------------------
// ------------------ SETUP ------------------
void setup() {
  //  Serial.begin(115200);
  //  Serial.println("---RGB--Control---");

  for (auto a : leds) {
    pinMode(*a, OUTPUT);
  }

  pinMode(ButtonSW, INPUT_PULLUP);
  ButLastState = digitalRead(ButtonSW);
  attachInterrupt(digitalPinToInterrupt(ButtonSW), sensButton, FALLING);

  // --------------- OLED ---------------
  //  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D or 0x3C for 128x64
  //    Serial.println(F("-- SSD1306 allocation failed --"));
  //    for (;;);
  //  }

  // ------------------ INITIAL DISPLAY  ------------------

  //  display.clearDisplay();
  //  display.setTextSize(2);
  //  display.setTextColor(WHITE);
  //  display.setCursor(49, 10);
  //  display.println("RGB");
  //  display.setCursor(25, 30);
  //  display.println("CONTROL");
  //  display.display();
  //  delay(10);
  //  display.clearDisplay();
}

// ------------------ LOOP ------------------
// ------------------ LOOP ------------------
void loop() {

  if (LedState == 0) {
    offState();
  }
  if (LedState == 1) {
    firstState();
  }
  if (LedState == 2) {
    secondState();
  }
  if (LedState == 3) {
    thirdState();
  }

  debounceButton();

}

// ------------------ VOIDS ------------------
// ------------------ VOIDS ------------------

// ------------------ CASES ------------------

void offState() {
  while (once) {
    for (auto a : leds) {
      digitalWrite(*a, LOW);
      delay(5);
    }
    once = false;
  }
}

void firstState() {
  smoothAnalog();
  for (auto a : leds) {
    analogWrite(*a, brightness);
  }
  delay(100);
}

void secondState() {
  for (auto a : leds) {
    unsigned int currentMillis = millis();
    if (currentMillis - prevMillis >= blinkDelay) {
      prevMillis = currentMillis;
      if (LedSWState == LOW) {
        LedSWState = HIGH;
      }
      else {
        LedSWState = LOW;
      }
      digitalWrite(*a, LedSWState);
    }
  }
}

void thirdState() {
  for (auto a : leds) {
    unsigned int currentMillis2 = millis();
    if (currentMillis2 - prevMillis2 >= int(analogRead(potR))) {
      prevMillis2 = currentMillis2;
      if (LedSWState2 == LOW) {
        LedSWState2 = HIGH;
      }
      else {
        LedSWState2 = LOW;
      }
      digitalWrite(*a, LedSWState2);
    }
  }
}


// ---------------- Smooth Analog ----------------
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
// ----------------- Interrupt ----------------
void sensButton() {
  if ( ButLastState == true ) {
    LedState++;
    once = true;
    ButLastState = false;
    if ( LedState > maxStates ) {
      LedState = 0;
    }
  }
}

void debounceButton() {
  if (digitalRead(ButtonSW)) {
    ButLastState = true;
  }
}

// ------------------ Displays ------------------
//void OnDisplay() {
//  display.clearDisplay();
//  display.setCursor(1, 25);
//  display.print("Bright:" + (String)brightness);
//  display.display();
//}
//
//void OffDisplay() {
//  display.clearDisplay();
//  display.setCursor(1, 25);
//  display.println("Press btn.");
//  display.display();
//}
//
//void stateDisplay() {
//  display.clearDisplay();
//  display.setCursor(10, 25);
//  display.print((String)LedState);
//  display.display();
//}
