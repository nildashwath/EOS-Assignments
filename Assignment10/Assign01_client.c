#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main() {
    int num1, num2, sum;
    int fd1, fd2;

    // Input two integers from the user
    printf("Client: Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    // Open FIFO1 for writing
    fd1 = open(FIFO1, O_WRONLY);
    if (fd1 < 0) {
        perror("Error opening FIFO1");
        exit(EXIT_FAILURE);
    }

    // Write two integers to FIFO1
    write(fd1, &num1, sizeof(int));
    write(fd1, &num2, sizeof(int));
    printf("Client: Sent numbers %d and %d to server.\n", num1, num2);

    // Open FIFO2 for reading
    fd2 = open(FIFO2, O_RDONLY);
    if (fd2 < 0) {
        perror("Error opening FIFO2");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    // Read the sum from FIFO2
    read(fd2, &sum, sizeof(int));
    printf("Client: Received sum from server: %d\n", sum);

    // Close the FIFOs
    close(fd1);
    close(fd2);

    printf("Client: Exiting.\n");
    return 0;
}

