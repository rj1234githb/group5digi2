#include <HardwareTimer.h>

#define BLINK_LED_PIN PB12  // Timer-controlled blinking LED (TIM4)
#define PWM_LED_PIN PA6     // PWM fading LED (TIM3_CH1)

// Timers
HardwareTimer timerBlink(TIM4); // For 500ms interrupt
HardwareTimer timerPWM(TIM3);   // For PWM fading

volatile uint8_t fadeDirection = 0; // 0 = fade in, 1 = fade out
uint16_t pwmValue = 0; // Duty cycle (0-999 for 10-bit)

void setup() {
  pinMode(BLINK_LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);

  // Configure TIM4 for 500ms interrupt (blinking)
  timerBlink.setPrescaleFactor(7200 - 1); // 72MHz / 7200 = 10kHz
  timerBlink.setOverflow(5000);           // 10kHz / 5000 = 2Hz (500ms)
  timerBlink.attachInterrupt(timerISR);
  timerBlink.resume();

  // Configure TIM3 for 1kHz PWM (fading)
  timerPWM.setPrescaleFactor(72 - 1);    // 72MHz / 72 = 1MHz
  timerPWM.setOverflow(999, TICK_FORMAT); // 1MHz / 1000 = 1kHz
  timerPWM.setMode(1, TIMER_OUTPUT_COMPARE_PWM1, PWM_LED_PIN);
  timerPWM.resume();
}

void loop() {
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis >= 10) { // Update every 10ms
    if (fadeDirection == 0) {
      pwmValue += 5; // Smaller steps for smoother fade
      if (pwmValue >= 999) fadeDirection = 1;
    } else {
      pwmValue -= 5;
      if (pwmValue <= 0) fadeDirection = 0;
    }
    timerPWM.setCaptureCompare(1, pwmValue, TICK_COMPARE_FORMAT); // Update PWM
    lastMillis = millis();
  }
}

// TIM4 ISR (toggle LED)
void timerISR() {
  digitalWrite(BLINK_LED_PIN, !digitalRead(BLINK_LED_PIN));
}