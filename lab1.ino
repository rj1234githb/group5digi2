#include <Arduino.h>

// Pin Definitions (adjust to your board)
#define LED1 PA0   // Blinks at 1Hz (Step 3)
#define LED2 PA1   // Blinks when button held (Step 5)
#define LED3 PA2   // Toggles on button press (Step 6)
#define BUTTON PC13 // Onboard button (active-low)

// Variables for Step 3 (1Hz blinking)
uint32_t lastBlinkTime = 0;
const uint16_t blinkInterval = 500; // 500ms

// Variables for Step 5 (button polling)
bool buttonHeld = false;

// Variables for Step 6 (interrupt debouncing)
volatile bool buttonPressed = false;
volatile uint32_t lastInterruptTime = 0;
const uint16_t debounceDelay = 50; // 50ms

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Attach interrupt for Step 6 (toggle LED3 on press)
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonISR, FALLING);
}

void loop() {
  // Step 3: Blink LED1 at 1Hz using millis()
  if (millis() - lastBlinkTime >= blinkInterval) {
    digitalWrite(LED1, !digitalRead(LED1));
    lastBlinkTime = millis();
  }

  // Step 5: Blink LED2 only when button is held (polling)
  buttonHeld = (digitalRead(BUTTON) == LOW); // Active-low check
  digitalWrite(LED2, buttonHeld ? HIGH : LOW);

  // Step 6: Handle interrupt-based toggle for LED3
  if (buttonPressed && (millis() - lastInterruptTime > debounceDelay)) {
    buttonPressed = false;
    digitalWrite(LED3, !digitalRead(LED3));
  }
}

// Interrupt Service Routine (Step 6)
void buttonISR() {
  if (millis() - lastInterruptTime > debounceDelay) {
    buttonPressed = true;
    lastInterruptTime = millis();
  }
}