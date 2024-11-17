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

    // Create the FIFOs if they do not exist
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("Server: Waiting for client...\n");

    // Open FIFO1 for reading
    fd1 = open(FIFO1, O_RDONLY);
    if (fd1 < 0) {
        perror("Error opening FIFO1");
        exit(EXIT_FAILURE);
    }

    // Read two integers from FIFO1
    read(fd1, &num1, sizeof(int));
    read(fd1, &num2, sizeof(int));
    printf("Server: Received numbers %d and %d from client.\n", num1, num2);

    // Calculate the sum
    sum = num1 + num2;

    // Open FIFO2 for writing
    fd2 = open(FIFO2, O_WRONLY);
    if (fd2 < 0) {
        perror("Error opening FIFO2");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    // Write the sum to FIFO2
    write(fd2, &sum, sizeof(int));
    printf("Server: Sent sum %d to client.\n", sum);

    // Close the FIFOs
    close(fd1);
    close(fd2);

    // Remove the FIFOs
    unlink(FIFO1);
    unlink(FIFO2);

    printf("Server: Exiting.\n");
    return 0;
}

