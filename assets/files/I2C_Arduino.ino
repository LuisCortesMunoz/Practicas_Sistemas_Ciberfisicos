-------------------------CODIGO I2C------------------------------------
// Step 1: Include library
#include <Wire.h>

// Step 2: I2C slave address
#define I2C_ADDRESS 0x08

// Step 3: Store last received message number (4 bytes)
volatile uint32_t last_seq = 0;

void onReceiveHandler(int numBytes)
{
  // Step 4: Expect 4 bytes (uint32_t)
  if (numBytes >= 4)
  {
    uint8_t b0 = Wire.read();
    uint8_t b1 = Wire.read();
    uint8_t b2 = Wire.read();
    uint8_t b3 = Wire.read();

    // Step 5: Rebuild little-endian uint32
    last_seq = ((uint32_t)b0) |
               ((uint32_t)b1 << 8) |
               ((uint32_t)b2 << 16) |
               ((uint32_t)b3 << 24);

    // Step 6: Discard extra bytes if any
    while (Wire.available()) { Wire.read(); }
  }
  else
  {
    // Step 7: Discard unexpected bytes
    while (Wire.available()) { Wire.read(); }
  }
}

void onRequestHandler()
{
  // Step 8: Send back last_seq as 4 bytes (little-endian)
  uint8_t out[4];
  out[0] = (uint8_t)(last_seq & 0xFF);
  out[1] = (uint8_t)((last_seq >> 8) & 0xFF);
  out[2] = (uint8_t)((last_seq >> 16) & 0xFF);
  out[3] = (uint8_t)((last_seq >> 24) & 0xFF);

  Wire.write(out, 4);
}

void setup()
{
  // Step 9: Start serial (for startup message only)
  Serial.begin(115200);
  delay(200);

  // Step 10: Start I2C as SLAVE
  Wire.begin(I2C_ADDRESS);

  // Step 11: Register I2C handlers
  Wire.onReceive(onReceiveHandler);
  Wire.onRequest(onRequestHandler);

  // Step 12: Startup message
  Serial.println("UNO I2C Slave ready (0x08).");
}

void loop()
{
  // Step 13: No continuous prints here (keeps timing stable)
  delay(10);
}