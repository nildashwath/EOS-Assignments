#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i;

    // Create 5 child processes
    for (i = 0; i < 5; i++) {
        pid = fork();

        if (pid == 0) {
            // Child process: Print count every second for 5 seconds
            for (int count = 1; count <= 5; count++) {
                printf("Child PID: %d, Count: %d\n", getpid(), count);
                sleep(1);
            }
            exit(0); // Exit after 5 seconds
        } 
        else if (pid < 0) {
            // Fork failed
            perror("Fork Failed");
            exit(1);
        }
    }

    // Parent process: Wait for all children to finish
    for (i = 0; i < 5; i++) {
        wait(NULL);
    }

    // Print when all children have completed
    printf("All child processes completed. Parent PID: %d\n", getpid());

    return 0;
}

