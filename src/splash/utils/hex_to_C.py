
with open("screen.txt", "r") as f:
    hex_string = f.read().strip()

byte_chunks = [hex_string[i:i+2] for i in range(0, len(hex_string), 2)]

lines = []

for i in range(0, len(byte_chunks), 12):
    group = byte_chunks[i:i+12]
    formatted = [f"0x{int(b, 16):02X}" for b in group]

    line = "    " + ", ".join(formatted) + ","
    lines.append(line)
c_array = "uint8_t data[] = {\n" + "\n".join(lines) + "\n};"

with open("output.txt", "w") as f:
    f.write(c_array)
