# Test Drivers

This directory contains **test driver programs** that validate model implementation through interactive state tracing.

Unlike the Unity unit tests in `../unity/`, these drivers place objects in specific scenarios and use `printf()` to output object state changes after each action. This allows verification that events (movement, gravity, jumping, collision) affect the model correctly.

## Files

- **`geo_driver.c`** - Tests Geo character movement, gravity, and jumping mechanics
- **`spike_driver.c`** - Tests spike placement and properties
- **`lava_driver.c`** - Tests lava pool placement and properties
- **`world_driver.c`** - Tests World struct and collision detection
- **`level_driver.c`** - Tests level structure and entity management

## Building and Running Drivers

### Requirements

- **C89 compatible compiler** (gcc, clang, etc.)
- All source files in the parent directory (`../../`)

### Compile and Run All Drivers (Recommended)

From the `src/` directory:

```bash
make test-drivers
```

This will:

1. Compile all drivers (geo, spike, lava, level, world)
2. Run each driver in order
3. Display output for each test

### Compile Only (Without Running)

```bash
make clean
```

Then manually compile each:

```bash
gcc -std=c89 -Wall -Wextra tests/drivers/geo_driver.c geo.c -o tests/drivers/geo_driver
gcc -std=c89 -Wall -Wextra tests/drivers/spike_driver.c spike.c -o tests/drivers/spike_driver
gcc -std=c89 -Wall -Wextra tests/drivers/lava_driver.c lava.c -o tests/drivers/lava_driver
gcc -std=c89 -Wall -Wextra tests/drivers/level_driver.c level.c block.c spike.c lava.c -o tests/drivers/level_driver
gcc -std=c89 -Wall -Wextra tests/drivers/world_driver.c world.c level.c block.c spike.c lava.c geo.c -o tests/drivers/world_driver
```

### Run Individual Drivers

Once compiled:

```bash
./tests/drivers/geo_driver
./tests/drivers/spike_driver
./tests/drivers/lava_driver
./tests/drivers/level_driver
./tests/drivers/world_driver
```

### Steem Compilation

For Steem/cross-platform builds, use the included `RUN_DRIVERS.G` script in the `src/` directory:

```
RUN_DRIVERS.G
```

This cleans object files and builds all test drivers using the Makefile.

## What Each Driver Tests

### Geo Driver (`geo_driver.c`)

**Tests:**

1. Basic gravity and horizontal movement
2. Jump from rest position
3. Jump apex (velocity reaches 0)
4. Horizontal movement during jump
5. Continuous movement with falling

**Key Output:** Position and velocity changes each tick

---

### Spike Driver (`spike_driver.c`)

**Tests:**

1. Spike creation at origin
2. Spike creation at arbitrary positions
3. Multiple independent spike instances
4. Boundary positions (screen edges)
5. Spike pattern (obstacle course)
6. Sprite pointer consistency

**Key Output:** Position verification and sprite pointer validation

---

### Lava Driver (`lava_driver.c`)

**Tests:**

1. Lava creation at origin
2. Lava creation at arbitrary positions
3. Lava pools at different heights
4. Lava pit at screen bottom
5. Scattered lava pools
6. Lava corridor (narrow passages)
7. Lava trench (jump obstacle)
8. Sprite pointer consistency
9. Memory layout verification

**Key Output:** Position tracking and hazard zone identification

---

### Level Driver (`level_driver.c`)

**Tests:**

1. Manual level creation with arrays
2. Block data accessibility
3. Spike data accessibility
4. Lava data accessibility
5. Load Level 1 (get_level1)
6. Load Level 2 (get_level2)
7. Load all levels (get_levels)
8. Entity counter initialization
9. Placeholder function
10. Level differentiation (Level 1 vs Level 2)

**Key Output:** Level structure verification and accessibility

---

### World Driver (`world_driver.c`)

**Tests:**

1. World creation via get_world()
2. Initial Geo state verification
3. Block collision - landing on top
4. Block collision - hit bottom (death)
5. Spike collision detection
6. Lava collision detection
7. Multiple safe positions
8. Levels accessibility from World

**Key Output:** Collision detection results and Geo state changes

---

## Test Output Format

Each driver follows this pattern:

```
===== [DRIVER NAME] TEST DRIVER =====
Testing [what is being tested]

--- TEST 1: [Description] ---
[Before state]
[Action]
[After state]
  Status or Expected results
```

### Example: Geo Driver Output

```
===== GEO CHARACTER TEST DRIVER =====
Testing movement, gravity, and jumping mechanics

--- TEST 1: Gravity and Horizontal Movement ---
Initial state (at rest on ground):
  Position: x=100, y=200
  Velocity: dx=1, dy=0
  Acceleration: ddy=-1

Applying 5 gravity ticks (free fall):
  Tick 1: x=101, y=200, dy=-1
  Tick 2: x=102, y=200, dy=-2
  ...
```

## Debugging with Drivers

### Adding Print Statements

The drivers are designed so you can easily add debugging output:

```c
/* In a driver, during a test loop */
printf("  Frame %d: x=%u, y=%u, state=%d\n", frame, obj.x, obj.y, obj.state);
```

### Modifying Test Parameters

Change values in struct initialization:

```c
/* Test at different positions */
Geo geo_test = (Geo){ GEO_DDY, GEO_DX, 0, 0, 200, 150, GEO_SIZE, GEO_SPRITE };
/* Or */
Geo geo_test = (Geo){ GEO_DDY, GEO_DX, 0, 0, 200, 50, GEO_SIZE, GEO_SPRITE };
```

## C89 Compatibility

All drivers are **C89 compatible**:

- Variable declarations at start of blocks (not in for loops)
- No variable-length arrays
- C89 comment style (`/* */`)
- Fixed-size array initialization (not dynamic)

```c
/* C89 - Correct */
int i;
for (i = 0; i < 10; i++) { }

/* C99 - Not compatible */
for (int i = 0; i < 10; i++) { }
```

## Integration with Main Game Loop

Once testing is complete, the same struct patterns and function calls will be used in `main.c`:

```c
Geo player = create_geo(100, 224, 32);
geo_jump(&player);
geo_move(&player);
```

The drivers provide confidence that these operations work correctly before integration.

## When to Use Test Drivers vs Unit Tests

| Aspect       | Unit Tests (`../tests/`)         | Test Drivers (`drivers/`)             |
| ------------ | -------------------------------- | ------------------------------------- |
| **Tool**     | Unity testing framework          | Simple printf() output                |
| **Scope**    | Single function                  | Multiple functions / scenarios        |
| **Output**   | PASS/FAIL count                  | Detailed state trace                  |
| **Use case** | Regression testing               | Behavior verification                 |
| **Example**  | "Does `geo_move()` add dy to y?" | "Does Geo fall correctly after jump?" |

## Notes

- Drivers use `#ifndef TESTING` guards allow them to have their own `main()` function
- They link directly with implementation files (e.g., `geo.c`, not the Unity test)
- Output is designed for human reading to manually verify correctness
- These can be extended as game logic is implemented
