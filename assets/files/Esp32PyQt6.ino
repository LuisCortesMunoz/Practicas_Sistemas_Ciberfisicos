// Step 1: Include Bluetooth Serial (built into ESP32 core)
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Step 2: LED pin (change if your board uses another pin)
const int LED_PIN = 2;

void setup() {
  // Step 3: LED always OFF at start
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Step 4: Start Bluetooth with device name
  Serial.begin(115200);
  SerialBT.begin("ESP32_LED");  // Name shown when pairing

  Serial.println("Bluetooth SPP started: ESP32_LED");
}

void loop() {
  // Step 5: Read command lines from Bluetooth
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    if (cmd == "LED_ON") {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("LED_ON_OK");
    } 
    else if (cmd == "LED_OFF") {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("LED_OFF_OK");
    } 
    else {
      SerialBT.print("UNKNOWN_CMD: ");
      SerialBT.println(cmd);
    }
  }

  delay(10);
}