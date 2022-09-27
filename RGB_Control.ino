/*
  RGB Control
  0.1 - three Option
  0.2 - remove display, add attachInterrupt to button
  0.3 - Now 6 States, for loop use void names, delete display voids and serial stuffs
  0.31 - I Simplified the code, to avoid Misunderstood later -> renamed few variable name,
*/

float version = 0.31;

// ------------------ LEDS ------------------
// ------------------ LEDS ------------------
const int c = 3;
String ledNames[c] = {"Red:", "Green:", "Blue:"};

int RED_L = 10;
int GREEN_L = 11;
int BLUE_L = 9;
int *leds[c] = { &RED_L, &GREEN_L, &BLUE_L };
int *flash[4] = { &RED_L, &RED_L, &BLUE_L, &BLUE_L };

int menuState = 0;  // Default state  0 - 5
int LedSWState = LOW;
unsigned long prevMillis = 0;
const int blinkDelay = 100;
int LedSWState2 = LOW;
unsigned long prevMillis2 = 0;

// ------------------ POTM ------------------
// ------------------ POTM ------------------
const int potPin = A0;
const int sNum = 10; // Sample number
uint32_t brightness;

// ------------------ BUTTON ------------------
// ------------------ BUTTON ------------------
int ButtonSW = 3;
bool ButLastState;
const int maxStates = 6;

// ------------------ VOIDS ------------------
// ------------------ VOIDS ------------------
void smoothAnalog();

void sensButton();
void debounceButton();

void offState();
void firstState();
void secondState();
void thirdState();
void fourthState();
void fifthState();
void sixthState();

bool once = true; // offState run once

typedef int (*st) (void);

// ------------------ SETUP ------------------
// ------------------ SETUP ------------------
void setup() {
//  Serial.begin(115200);
//  Serial.println("--- RGB--Control ---");
//  Serial.println("--- version:"+(String)version+" ---");

  for (auto a : leds) {
    pinMode(*a, OUTPUT);
  }

  pinMode(potPin, INPUT);
  
  pinMode(ButtonSW, INPUT_PULLUP);
  ButLastState = digitalRead(ButtonSW);
  attachInterrupt(digitalPinToInterrupt(ButtonSW), sensButton, FALLING);
}
// --------------- END SETUP ------------------
// --------------- END SETUP ------------------

// ------------------ LOOP ------------------
// ------------------ LOOP ------------------
void loop() {
  st states[maxStates + 1];
  states[0] = &offState;
  states[1] = &firstState;
  states[2] = &secondState;
  states[3] = &thirdState;
  states[4] = &fourthState;
  states[5] = &fifthState;
  states[6] = &sixthState;

  for (int i = 0; i < maxStates + 1; i++) {
    while (menuState == i) {
      states[i]();
      debounceButton();
    }
  }
}
// --------------- END LOOP ------------------
// --------------- END LOOP ------------------


// ------------------ VOIDS ------------------
// ------------------ VOIDS ------------------

// ------------------ STATES ------------------
void offState() {
  while (once) {
    for (auto a : leds) {
      digitalWrite(*a, LOW);
      delay(5);
    }
    once = false;
    delay(100);
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
      LedSWState = !LedSWState;
      digitalWrite(*a, LedSWState);
    }
  }
}

void thirdState() {
  for (auto a : leds) {
    unsigned int currentMillis2 = millis();
    if (currentMillis2 - prevMillis2 >= int(analogRead(potPin))) {
      prevMillis2 = currentMillis2;
      LedSWState2 = !LedSWState2;
      digitalWrite(*a, LedSWState2);
    }
  }
}

void fourthState() {
  for (auto a : leds) {
    digitalWrite(*a, HIGH);
    delay(250);
    digitalWrite(*a, LOW);
    delay(250);
  }
}

void fifthState() {
  for (auto a : flash) {
    digitalWrite(*a, HIGH);
    delay(100);
    digitalWrite(*a, LOW);
    delay(100);
  }
}

void sixthState() {
  smoothAnalog();
  if (brightness <= 30) {
    brightness = 30;
  }
  for (auto a : leds) {
    digitalWrite(*a, HIGH);
    delay(brightness * 2);
    digitalWrite(*a, LOW);
    delay(brightness * 2);
  }
}

// ---------------- Smooth Analog ----------------
void smoothAnalog() {
  uint32_t raw = 0;
  for ( int i = 0; i < sNum; i++ ) {
    raw += analogRead(potPin);
    //delay(1);
  }
  raw = int(raw / sNum);
  brightness = map(raw, 0, 810, 0, 255);
  if (brightness <= 4) {
    brightness = 0;
  }
  else if (brightness >= 248) {
    brightness = 255;
  }
}

// ------------------ Button ------------------
// ----------------- Interrupt ----------------
void sensButton() {
  if ( ButLastState == true ) {
    menuState++;
    once = true;
    ButLastState = false;
    if ( menuState > maxStates ) {
      menuState = 0;
    }
  }
}

void debounceButton() {
  if (digitalRead(ButtonSW)) {
    ButLastState = true;
  }
}
