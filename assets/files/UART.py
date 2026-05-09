# Step 1: Imports
from machine import Pin, UART
import time
import struct

# Step 2: Pins from your XIAO datasheet
TX_PIN = 43   # XIAO TX
RX_PIN = 44   # XIAO RX

# Step 3: Settings
BAUD = 2000000
N = 1000
START_BYTE = 0xA5

# Step 4: Build a 6-byte frame: [0xA5][seq(4 LE)][xor checksum of first 5 bytes]
def make_frame(k: int) -> bytes:
    seq = struct.pack("<I", k)
    first5 = bytes([START_BYTE]) + seq
    cs = 0
    for bb in first5:
        cs ^= bb
    return first5 + bytes([cs])

# Step 5: Try to open UART bus (some boards use UART(1), some UART(2))
uart = None
for bus in (1, 2, 0):
    try:
        uart = UART(bus, baudrate=BAUD, tx=Pin(TX_PIN), rx=Pin(RX_PIN), timeout=50)
        time.sleep_ms(100)
        print("# UART opened on bus:", bus)
        break
    except Exception as e:
        print("# UART bus", bus, "failed:", e)

if uart is None:
    raise SystemExit("# ERROR: Could not open any UART bus.")

# Step 6: Helper: read exactly n bytes with timeout
def read_exact(n: int, timeout_ms: int = 200) -> bytes:
    t0 = time.ticks_ms()
    out = b""
    while len(out) < n:
        if time.ticks_diff(time.ticks_ms(), t0) > timeout_ms:
            return b""
        chunk = uart.read(n - len(out))
        if chunk:
            out += chunk
    return out

# Step 7: Flush any junk bytes
uart.read()

# Step 8: CSV output file
filename = "latency_uart.csv"
f = open(filename, "w")
f.write("msg_index,dt_seconds,ack_ok\n")

print("msg_index,dt_seconds,ack_ok")

# Step 9: Main test loop
for k in range(N):
    frame = make_frame(k)

    # Step 9.1: Time from send -> full echo received
    t0 = time.ticks_us()

    uart.write(frame)
    echo = read_exact(6, timeout_ms=200)

    t1 = time.ticks_us()
    dt_sec = time.ticks_diff(t1, t0) / 1_000_000.0

    # Step 9.2: Validate echo
    ok = 0
    if len(echo) == 6 and echo == frame:
        ok = 1

    line = "{},{:.6f},{}\n".format(k, dt_sec, ok)
    f.write(line)
    print(line.strip())

    # Step 9.3: small gap helps stability
    time.sleep_ms(2)

f.close()
print("# Done! Saved:", filename)