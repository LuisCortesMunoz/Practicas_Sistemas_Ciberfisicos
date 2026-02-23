---------------------------CODIGO 12C (MICRO)-------------------------
# Step 1: Imports
from machine import Pin, I2C
import time
import struct

# Step 2: I2C pins for XIAO ESP32-S3 (from your datasheet)
SDA_PIN = 5
SCL_PIN = 6

# Step 3: I2C settings
FREQ_HZ = 100000          # try 400000 later if you want
I2C_ADDR = 0x08           # UNO slave address
N = 1000                  # number of messages

# Step 4: Create I2C master
i2c = I2C(0, sda=Pin(SDA_PIN), scl=Pin(SCL_PIN), freq=FREQ_HZ)

# Step 5: Scan to confirm device is detected
print("# Scanning I2C bus...")
devices = i2c.scan()
print("# Found:", [hex(d) for d in devices])

if I2C_ADDR not in devices:
    print("# ERROR: UNO (0x08) not found. Check wiring, level shifter, address.")
    raise SystemExit

# Step 6: Create CSV file on the ESP32 storage
filename = "latency_i2c.csv"
f = open(filename, "w")
f.write("msg_index,dt_seconds,ack_ok\n")

# Step 7: Print CSV header to Thonny console too (easy copy/paste to Excel)
print("msg_index,dt_seconds,ack_ok")

# Step 8: Main test loop
for k in range(N):
    # Step 8.1: Make payload = uint32 little-endian
    payload = struct.pack("<I", k)

    # Step 8.2: Start timing (microseconds)
    t0 = time.ticks_us()

    # Step 8.3: Send message to UNO (write 4 bytes)
    i2c.writeto(I2C_ADDR, payload)

    # Step 8.4: Read echo back from UNO (4 bytes)
    data = i2c.readfrom(I2C_ADDR, 4)

    # Step 8.5: Stop timing
    t1 = time.ticks_us()

    # Step 8.6: Compute round-trip time in seconds
    dt_sec = time.ticks_diff(t1, t0) / 1_000_000.0

    # Step 8.7: Check echo correctness
    ack = struct.unpack("<I", data)[0]
    ok = 1 if ack == k else 0

    # Step 8.8: Save one row
    line = "{},{:.6f},{}\n".format(k, dt_sec, ok)
    f.write(line)
    print(line.strip())

# Step 9: Close file
f.close()
print("# Done! Saved:", filename)