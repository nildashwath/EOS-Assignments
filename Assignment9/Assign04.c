#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int arr[2]; // Pipe file descriptors
    pid_t pid1, pid2;
    int status1, status2;

    // Create a pipe
    if (pipe(arr) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Fork the first child process (writer)
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) { // First child process
        // Close the read end of the pipe
        close(arr[0]);

        // Redirect stdout to the write end of the pipe
        dup2(arr[1], STDOUT_FILENO);
        close(arr[1]);

        // Execute "who" command
        if (execlp("who", "who", NULL) == -1) {
            perror("execlp who failed");
            exit(EXIT_FAILURE);
        }
    }

    // Fork the second child process (reader)
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) { // Second child process
        // Close the write end of the pipe
        close(arr[1]);

        // Redirect stdin to the read end of the pipe
        dup2(arr[0], STDIN_FILENO);
        close(arr[0]);

        // Execute "wc" command
        if (execlp("wc", "wc", NULL) == -1) {
            perror("execlp wc failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process closes both ends of the pipe
    close(arr[0]);
    close(arr[1]);

    // Wait for both child processes to complete
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    printf("Parent: Both child processes have finished.\n");

    return 0;
}

