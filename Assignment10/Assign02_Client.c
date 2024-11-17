#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

int main() {
    struct message msg;
    int msgid;
    char buffer[MSG_SIZE];
    int fifo_fd;
    FILE *output_file;

    // Input FIFO and file names
    printf("Client: Enter FIFO name: ");
    scanf("%s", msg.fifo_name);
    printf("Client: Enter file name to receive from server: ");
    scanf("%s", msg.file_name);

    // Create message queue
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Prepare and send the message
    msg.msg_type = 1;
    if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    printf("Client: Sent request to server\n");

    // Open the FIFO to read file content
    fifo_fd = open(msg.fifo_name, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Save received file content to a new file
    output_file = fopen("received_file.txt", "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    // Read from FIFO and write to the output file
    while (read(fifo_fd, buffer, MSG_SIZE) > 0) {
        fputs(buffer, output_file);
    }

    printf("Client: File content received and saved to 'received_file.txt'\n");

    // Clean up
    close(fifo_fd);
    fclose(output_file);

    return 0;
}

