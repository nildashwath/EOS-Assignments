#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int pipefd[2];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Using the write end of the pipe to get buffer size
    int pipe_size = fcntl(pipefd[1], F_GETPIPE_SZ);
    if (pipe_size == -1) {
        perror("Failed to get pipe buffer size");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }

    printf("The size of the pipe buffer is: %d bytes\n", pipe_size);

    // Close the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}

