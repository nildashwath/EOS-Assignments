#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Signal handler function for SIGINT
void handle_sigint(int sig) {
    printf("\nCaught signal %d (SIGINT). Press Ctrl+D to exit the shell.\n", sig);
}

// Function to display a simple shell prompt
void shell() {
    char command[1024];

    while (1) {
        printf("my_shell> ");  // Shell prompt
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }
        // Remove newline character from the input
        command[strcspn(command, "\n")] = '\0';

        // Check if the command is "exit" to terminate the shell
        if (strcmp(command, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Execute the command using system()
        if (system(command) == -1) {
            perror("Error executing command");
        }
    }
}

int main() {
    // Register the SIGINT handler
    signal(SIGINT, handle_sigint);

    printf("Welcome to my shell! Press Ctrl+C to see the signal handler in action.\n");
    shell();

    return 0;
}

