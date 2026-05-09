-----------------------------SPI ESP32 MICROP-------------------------
from machine import Pin, SPI
import time
import struct

# Step 1: ESP32 VSPI pins (from your datasheet image)
SCK  = 18
MISO = 19
MOSI = 23
CS   = 5

# Step 2: Settings
N = 1000
BAUD = 9600  # start stable; try 250_000 if you get mismatches

# Step 3: SPI init (ESP32 usually supports SPI(1) and SPI(2))
# If SPI(2) fails, change to SPI(1)
spi = SPI(
    2,
    baudrate=BAUD,
    polarity=0,
    phase=0,
    bits=8,
    firstbit=SPI.MSB,
    sck=Pin(SCK),
    mosi=Pin(MOSI),
    miso=Pin(MISO),
)

cs = Pin(CS, Pin.OUT, value=1)

# Step 4: Transfer helper
def xfer8(tx):
    rx = bytearray(8)
    cs.value(0)
    spi.write_readinto(tx, rx)
    cs.value(1)
    return rx

# Step 5: Quick "clock overhead" test (ticks_us only)
t0 = time.ticks_us()
t1 = time.ticks_us()
print("ticks_us overhead (us):", time.ticks_diff(t1, t0))

# Step 6: Build a sample TX buffer
tx = bytearray(8)
tx[0:4] = struct.pack("<I", 123456)   # sample seq
tx[4:8] = b"\x00\x00\x00\x00"

# Step 7: Measure ONE SPI transaction time (us)
t0 = time.ticks_us()
rx = xfer8(tx)
t1 = time.ticks_us()
print("ONE SPI xfer time (us):", time.ticks_diff(t1, t0))
print("Sample RX (hex):", rx.hex())

# Step 8: Now run the 1000 message test and save CSV
filename = "latency_spi.csv"
f = open(filename, "w")
f.write("msg_index,dt_seconds,ack_ok\n")
print("msg_index,dt_seconds,ack_ok")

for k in range(N):
    # Step 9: Build TX: seq (uint32 LE) + 4 dummy bytes
    tx[0:4] = struct.pack("<I", k)
    tx[4:8] = b"\x00\x00\x00\x00"

    # Step 10: Time the transaction
    t0 = time.ticks_us()
    rx = xfer8(tx)
    t1 = time.ticks_us()

    dt_sec = time.ticks_diff(t1, t0) / 1_000_000.0

    # Step 11: UNO echoes seq during rx[4..7]
    echo = struct.unpack("<I", bytes(rx[4:8]))[0]
    ok = 1 if echo == k else 0

    # Step 12: Save + print
    f.write("{},{:.6f},{}\n".format(k, dt_sec, ok))
    print("{},{:.6f},{}".format(k, dt_sec, ok))

    # Step 13: Give CS some high time (helps stability)
    time.sleep_us(500)

f.close()
print("# Done! Saved:", filename)