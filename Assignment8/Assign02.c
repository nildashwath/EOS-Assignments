#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Signal handler to reap zombie processes
void handle_sigchld(int sig) {
    // Reap all dead child processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Function to execute commands
void execute_command(char *command, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        char *args[] = {"/bin/sh", "-c", command, NULL};
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        if (!background) {
            // Wait for the child process if not in background
            waitpid(pid, NULL, 0);
        } else {
            printf("Started background process with PID: %d\n", pid);
        }
    }
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

        // Check if the command should be executed in the background
        int background = 0;
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '&') {
            background = 1;
            command[len - 1] = '\0';  // Remove the '&' character
        }

        // Execute the command
        execute_command(command, background);
    }
}

int main() {
    // Register the SIGCHLD handler to clean up zombie processes
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    printf("Welcome to my shell! Type commands, and use '&' for background execution.\n");
    shell();

    return 0;
}

