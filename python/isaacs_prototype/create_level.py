import csv

# Create a level that fits on screen (20 blocks tall to fit 400px display)
width = 2000
height = 20  # Changed from 30 to 20 to fit on screen

# Initialize with all zeros (sky)
level = [[0 for _ in range(width)] for _ in range(height)]

# Ground level (last 3 rows) - with gaps
for y in range(17, 20):
    for x in range(width):
        level[y][x] = 255

# Add gaps in the ground
gaps = [
    (150, 156),   # Gap 1
    (300, 308),   # Gap 2
    (550, 558),   # Gap 3
    (800, 806),   # Gap 4
    (1050, 1058), # Gap 5
    (1300, 1306), # Gap 6
    (1550, 1558), # Gap 7
    (1800, 1808), # Gap 8
]

for y in range(17, 20):
    for gap_start, gap_end in gaps:
        for x in range(gap_start, gap_end):
            if x < width:
                level[y][x] = 0

# Add floating platforms (2 blocks thick for stability)
platforms = [
    # (x_start, x_end, y_position) - positioned in visible area
    (100, 130, 14),      # Platform 1 - 3 blocks above ground
    (180, 220, 12),      # Platform 2 - higher
    (260, 290, 10),      # Platform 3 - even higher
    (330, 380, 13),      # Platform 4
    (420, 460, 11),      # Platform 5
    (500, 540, 15),      # Platform 6 - lower
    (580, 620, 12),      # Platform 7
    (680, 720, 10),      # Platform 8
    (760, 800, 14),      # Platform 9
    (830, 880, 11),      # Platform 10
    (920, 960, 13),      # Platform 11
    (1000, 1040, 9),     # Platform 12 - high
    (1080, 1130, 12),    # Platform 13
    (1180, 1220, 14),    # Platform 14
    (1260, 1300, 11),    # Platform 15
    (1330, 1380, 13),    # Platform 16
    (1420, 1460, 10),    # Platform 17
    (1500, 1540, 15),    # Platform 18
    (1580, 1630, 12),    # Platform 19
    (1680, 1720, 13),    # Platform 20
    (1760, 1800, 11),    # Platform 21
    (1830, 1880, 14),    # Platform 22
    (1920, 1970, 12),    # Platform 23
]

for x_start, x_end, y_pos in platforms:
    # Make platforms 2 blocks thick
    for dy in range(2):
        y = y_pos + dy
        if y < height:
            for x in range(x_start, min(x_end, width)):
                level[y][x] = 255

# Add some spike obstacles on the ground
spikes = [
    (220, 224, 15),   # Spike set 1 - 2 blocks above ground
    (380, 385, 15),   # Spike set 2
    (620, 626, 15),   # Spike set 3
    (880, 886, 15),   # Spike set 4
    (1130, 1136, 15), # Spike set 5
    (1380, 1386, 15), # Spike set 6
    (1630, 1636, 15), # Spike set 7
    (1880, 1886, 15), # Spike set 8
]

for x_start, x_end, y_pos in spikes:
    # Spikes are 2 blocks tall
    for dy in range(2):
        y = y_pos + dy
        if y < height:
            for x in range(x_start, min(x_end, width)):
                level[y][x] = 200

# Add some spikes on platforms
platform_spikes = [
    (110, 112, 12),   # On platform 1
    (350, 352, 11),   # On platform 4
    (590, 592, 10),   # On platform 7
    (850, 852, 9),    # On platform 10
    (1100, 1102, 10), # On platform 13
    (1350, 1352, 11), # On platform 16
    (1600, 1602, 10), # On platform 19
]

for x_start, x_end, y_pos in platform_spikes:
    for dy in range(2):
        y = y_pos + dy
        if y < height:
            for x in range(x_start, min(x_end, width)):
                level[y][x] = 200

# Write to CSV
with open('world.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(level)

print(f"Created level: {height} rows x {width} columns")
print(f"- {len(gaps)} gaps in ground")
print(f"- {len(platforms)} floating platforms")
print(f"- {len(spikes)} spike obstacles on ground")
print(f"- {len(platform_spikes)} spike obstacles on platforms")
