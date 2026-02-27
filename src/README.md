# Source Directory

This directory contains the C source code for the project.

## Running on STEEM

Copy the contents of this folder, src, into your appropriate STEEM folder. You do not need to include tests, nor do you need to include the .md files. Furthermore, DO NOT INCLUDE Makefile, and rename MAKEFILE_STEEM to MAKEFILE.

Then "RUN_MAKE.g" from Gulam.

Then "prog.tos".

## Prerequisites

Ensure the following is installed:
- **GCC Compiler**: For compiling C code
- **Make**: For running the Makefile build system
  - Linux/Mac: Usually pre-installed
  - Windows: [Installation Guide](https://www.youtube.com/watch?v=5TavcolACQY&t=227s)
- **Valgrind**: For running memory leak checks
  - Linux/Mac: ```sudo apt install valgrind```
  - Windows: Ideally just run WSL and follow the Linux/Mac setup, otherwise you're on your own

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
