#include <SPI.h>

// Step 1: Buffers
volatile uint8_t rx_seq[4] = {0, 0, 0, 0};
volatile uint8_t tx_seq[4] = {0, 0, 0, 0};
volatile uint8_t byte_count = 0;
volatile bool frame_active = false;

// Step 2: CS edge detect on SS (D10 = PB2)
ISR(PCINT0_vect)
{
  if (PINB & _BV(PB2)) {
    // SS HIGH -> end frame
    frame_active = false;
  } else {
    // SS LOW -> start frame
    frame_active = true;
    byte_count = 0;
    SPDR = 0x00; // byte0 output (dummy)
  }
}

// Step 3: SPI byte ISR
ISR(SPI_STC_vect)
{
  uint8_t inByte = SPDR;

  if (!frame_active) {
    SPDR = 0x00;
    return;
  }

  // Receive first 4 bytes (seq)
  if (byte_count < 4) {
    rx_seq[byte_count] = inByte;

    // After receiving the 4th byte, prepare echo AND preload first echo byte
    if (byte_count == 3) {
      tx_seq[0] = rx_seq[0];
      tx_seq[1] = rx_seq[1];
      tx_seq[2] = rx_seq[2];
      tx_seq[3] = rx_seq[3];

      // IMPORTANT: preload first echo byte for NEXT transfer (byte index 4)
      SPDR = tx_seq[0];
    } else {
      // While receiving bytes 0..2, output dummy for next byte
      SPDR = 0x00;
    }
  }
  else {
    // We are currently receiving byte 4..7 (dummy from master).
    // Preload what will be sent on the NEXT byte:
    // end of byte4 -> load tx_seq[1] for byte5
    // end of byte5 -> load tx_seq[2] for byte6
    // end of byte6 -> load tx_seq[3] for byte7
    // end of byte7 -> load 0x00
    if (byte_count == 4) SPDR = tx_seq[1];
    else if (byte_count == 5) SPDR = tx_seq[2];
    else if (byte_count == 6) SPDR = tx_seq[3];
    else SPDR = 0x00;
  }

  byte_count++;
  if (byte_count > 7) byte_count = 8; // wait for CS reset
}

void setup()
{
  pinMode(MISO, OUTPUT);
  pinMode(10, INPUT_PULLUP); // SS idle HIGH

  // Enable pin change interrupt for D10 (PB2)
  PCICR  |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT2);

  // Enable SPI slave + interrupt
  SPCR = _BV(SPE) | _BV(SPIE);
  SPDR = 0x00;
}

void loop()
{

}