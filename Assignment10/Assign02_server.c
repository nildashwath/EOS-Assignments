#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "common.h"

int main() {
    struct message msg;
    int msgid;
    char buffer[MSG_SIZE];
    FILE *file;
    int fifo_fd;

    // Create message queue
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Receive the message from the client
    if (msgrcv(msgid, &msg, sizeof(msg), 0, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Received FIFO name: %s, File name: %s\n", msg.fifo_name, msg.file_name);

    // Open the requested file
    file = fopen(msg.file_name, "r");
    if (file == NULL) {
        perror("Failed to open the requested file");
        exit(EXIT_FAILURE);
    }

    // Create and open the FIFO
    mkfifo(msg.fifo_name, 0666);
    fifo_fd = open(msg.fifo_name, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Send file content through FIFO
    while (fgets(buffer, MSG_SIZE, file) != NULL) {
        write(fifo_fd, buffer, strlen(buffer));
    }

    printf("Server: File content sent to client via FIFO\n");

    // Clean up
    close(fifo_fd);
    fclose(file);
    unlink(msg.fifo_name);
    msgctl(msgid, IPC_RMID, NULL);  // Remove the message queue

    return 0;
}

