# Source Directory

This directory contains the C source code for the project.

## Prerequisites

Ensure the following is installed:
- **GCC Compiler**: For compiling C code
- **Make**: For running the Makefile build system
  - Linux/Mac: Usually pre-installed
  - Windows: [Installation Guide](https://www.youtube.com/watch?v=5TavcolACQY&t=227s)

## Building the Main Program
```bash
make helloworld
```

## Cleaning Build Artifacts
```bash
make clean
```

## Files and Folders

- [Makefile](Makefile): Build configuration for compiling and testing
- Other files are currently placeholders
- [tests/](tests): Contains all unit tests using the Unity testing framework
  - **test_*.c** - Individual test files (automatically discovered and compiled)
  - [tests/unity](tests/unity): Unity testing framework files

## Testing

See [TESTING.md](TESTING.md)
