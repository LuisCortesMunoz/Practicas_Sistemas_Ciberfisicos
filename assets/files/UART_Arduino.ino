// Step 1: Constants
const uint8_t START_BYTE = 0xA5;

// Step 2: State machine variables
uint8_t state = 0;
uint8_t buf[6];
uint8_t idx = 0;

// Step 3: Compute XOR checksum of first 5 bytes
uint8_t checksum5(const uint8_t *p) {
  uint8_t c = 0;
  for (int i = 0; i < 5; i++) c ^= p[i];
  return c;
}

void setup() {
  // Step 4: UART for communication (uses D0/D1)
  Serial.begin(2000000);

  // Step 5: IMPORTANT: Do NOT print anything (would corrupt UART stream)
}

void loop() {
  // Step 6: Read all available bytes
  while (Serial.available() > 0) {
    uint8_t b = (uint8_t)Serial.read();

    // Step 7: State 0 = wait for START_BYTE
    if (state == 0) {
      if (b == START_BYTE) {
        buf[0] = b;
        idx = 1;
        state = 1;
      }
    }
    // Step 8: State 1 = read remaining 5 bytes
    else if (state == 1) {
      buf[idx++] = b;

      // Step 9: When full frame received (6 bytes)
      if (idx >= 6) {
        uint8_t cs = checksum5(buf);

        // Step 10: If checksum ok, echo same 6 bytes back
        if (cs == buf[5]) {
          Serial.write(buf, 6);
        }
        // Step 11: Reset state machine
        state = 0;
        idx = 0;
      }
    }
  }
}