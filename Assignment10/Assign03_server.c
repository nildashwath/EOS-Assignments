#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/uds_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[1024];
    int num1, num2, sum;

    // Create a UNIX domain socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for client connection...\n");

    // Accept a connection from the client
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: Client connected.\n");

    // Read data from the client
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Read failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Parse the two numbers from the buffer
    sscanf(buffer, "%d %d", &num1, &num2);
    printf("Server: Received numbers %d and %d from client.\n", num1, num2);

    // Calculate the sum
    sum = num1 + num2;

    // Send the result back to the client
    snprintf(buffer, sizeof(buffer), "%d", sum);
    if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
        perror("Send failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: Sent sum %d back to client.\n", sum);

    // Close the client and server sockets
    close(client_fd);
    close(server_fd);

    // Remove the UNIX socket file
    unlink(SOCKET_PATH);

    printf("Server: Exiting.\n");
    return 0;
}

