// Header file for input output functions
#include <stdio.h>
#include "helloworld.h"

int add_numbers(int x, int y) {
    return x + y;
}

// Main function: entry point for execution
int main() {

    // Writing print statement to print hello world
    int value = add_numbers(12,3);
    printf("Hello World");
    printf("Value from add: %d", value);

    return 0;
}