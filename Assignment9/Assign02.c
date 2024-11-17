#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Pipe from child to parent
    int pipe2[2]; // Pipe from parent to child
    pid_t pid;

    // Create the first pipe
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    // Create the second pipe
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipe1[0]); // Close read end of first pipe (child writes)
        close(pipe2[1]); // Close write end of second pipe (child reads)

        int number1 = 10; // Example number
        int number2 = 20; // Example number

        // Send two numbers to the parent
        write(pipe1[1], &number1, sizeof(int));
        write(pipe1[1], &number2, sizeof(int));
        printf("Child: Sent numbers %d and %d to parent.\n", number1, number2);

        // Receive the result from the parent
        int result;
        read(pipe2[0], &result, sizeof(int));
        printf("Child: Received sum from parent: %d\n", result);

        // Close the pipes
        close(pipe1[1]);
        close(pipe2[0]);

        exit(0);

    } else { // Parent process
        close(pipe1[1]); // Close write end of first pipe (parent reads)
        close(pipe2[0]); // Close read end of second pipe (parent writes)

        int number1, number2;

        // Read the two numbers from the child
        read(pipe1[0], &number1, sizeof(int));
        read(pipe1[0], &number2, sizeof(int));
        printf("Parent: Received numbers %d and %d from child.\n", number1, number2);

        // Calculate the sum
        int sum = number1 + number2;

        // Send the result back to the child
        write(pipe2[1], &sum, sizeof(int));
        printf("Parent: Sent sum %d to child.\n", sum);

        // Wait for the child process to finish
        wait(NULL);

        // Close the pipes
        close(pipe1[0]);
        close(pipe2[1]);

        printf("Parent: Exiting.\n");
    }

    return 0;
}

