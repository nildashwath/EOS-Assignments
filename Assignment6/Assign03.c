#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int count = 0;

    // Infinite loop to keep calling fork()
    while (1) {
        pid = fork(); // Create a new process

        if (pid < 0) {
            // Fork failed
            perror("fork failed");
            break; // Exit the loop if fork fails
        }

        count++; // Increment the count for each successful fork

        // Parent process continues looping, children do not
        if (pid > 0) {
            // Parent waits for each child to terminate
            wait(NULL);
        } else {
            // Child process exits
            break;
        }
    }

    // Output the number of processes created (total forks)
    if (pid > 0) {
        printf("Total processes created: %d\n", count);
    }

    return 0;
}

