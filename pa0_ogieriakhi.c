/*
Name: Osaze Ogieriakhi
Task: Programming Assingnment 0
Class: CPTS360 Fall 2024
Description: This program is designed to simulate a Unix File System
*/
#include "pa0_ogieriakhi.h"




int main() {

    NODE *root = createNode("/", D);
    NODE *CWD;
	initialize();
	// other initialization as needed

    printf("*****UNIX FILING SYSTEM*****\n");

    char input[256];  // Buffer for user input

    // Infinite loop to keep the shell running
    while (1) {
        printf("Enter command: ");
        fgets(input, 256, stdin);  // Get user input

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Call the function to handle the input
        handleInput(input);
    
}
}

