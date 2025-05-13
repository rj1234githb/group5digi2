// UART_ExtLED_Toggle.ino
//  - Sends "Hello from STM32!\r\n" every second.
//  - Receives 't' over Serial, toggles external LED on pin 8, and reports status.

const unsigned long BAUD     = 115200;  // UART baud rate: 115200 bps, 8N1 :contentReference[oaicite:6]{index=6}
const unsigned long PERIOD   = 1000;    // Send interval in milliseconds

const int LED_PIN   = 8;                // External LED connected to digital pin 8 :contentReference[oaicite:7]{index=7}
bool      ledState = LOW;               // Tracks current LED state

unsigned long lastTick = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);             // Configure pin 8 as OUTPUT :contentReference[oaicite:8]{index=8}
  digitalWrite(LED_PIN, ledState);      // Initialize LED OFF
  Serial.begin(BAUD);                   // Begin Serial at 115200 baud :contentReference[oaicite:9]{index=9}
  while (!Serial) { /* wait for Serial port to open */ }
}

void loop() {
  unsigned long now = millis();

  // 1) Periodic greeting every PERIOD ms
  if (now - lastTick >= PERIOD) {
    lastTick = now;
    Serial.print("Hello from STM32!\r\n");  // Send greeting :contentReference[oaicite:10]{index=10}
  }

  // 2) Poll for incoming data
  if (Serial.available() > 0) {             // Check RX buffer :contentReference[oaicite:11]{index=11}
    char c = Serial.read();                 // Read one char
    if (c == 't') {
      // Toggle external LED
      ledState = !ledState;                 
      digitalWrite(LED_PIN, ledState);      // Drive pin HIGH/LOW :contentReference[oaicite:12]{index=12}

      // Report status back to Serial Monitor
      Serial.print("Received 't': LED ");
      Serial.println( ledState ? "ON" : "OFF" );
                                               // Echo LED status :contentReference[oaicite:13]{index=13}
    }
    // (Optional) Handle other characters or echo them:
    // else { Serial.print(c); }
  }
}