# Atari ST Raster Test - Build & Run Guide

## What are .G Files?

`.G` files are **Gulam shell scripts** for the Atari ST. Gulam is a Unix-like shell that runs on TOS (the Atari ST operating system). These scripts automate build tasks, similar to bash scripts on Linux.

### Available Scripts

#### `RUN_MAKE.G`
- **Purpose**: Clean build artifacts and compile the project
- **What it does**:
  1. Removes all `.o` (object) files
  2. Removes all `.tos` (executable) files
  3. Runs `make` to build the project
  4. Displays completion message

#### `CLEAN.G`
- **Purpose**: Remove all build artifacts (object files and executables)
- **What it does**:
  1. Removes all `.o` files
  2. Removes all `.tos` files
  3. Leaves source files untouched

---

## Building on Atari ST (Steem Emulator)

### Prerequisites
- **Steem SSE Emulator** running Atari ST TOS
- **cc68x compiler** (Alcyon C cross-compiler) installed on the Atari ST
- **Gulam shell** for running .G scripts

### File Preparation

**IMPORTANT**: The Atari ST uses TOS which has **8.3 filename limits** (8 characters for filename, 3 for extension).

#### Files:
```
raster/
├── test.c          (test program)
├── raster.h        (header file)
├── clear.s         (screen entire screen)
├── clr_reg.s       (region clear region (16 bit chunks))
├── clr_lr_reg.s    (region clear large region (32 bit chunks)) 
├── MAKEFILE        (build rules)
├── RUN_MAKE.G      (build script (RUN THIS ONE FOR EASIER BUILD))
├── FILL.C          (fills screen with black pixels (from lab, temp file)) 
└── CLEAN.G         (clean build files)

raster.h is the header file that will need to be FULLY implemented (all funciton stubs).

Current plan is to implement strategically: everything that we will actually use is to be highly optimized- the things we wont be using: nobody cares.
```

---

## Quick Reference

```bash
# Clean and build
run_make.g

# Just clean
clean.g

# Just build
make rtest.tos

# Run test
rtest.tos

```
