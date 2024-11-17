#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/uds_socket"

int main() {
    int sock;
    struct sockaddr_un addr;
    char buffer[1024];
    int num1, num2;

    // Create a UNIX domain socket
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Connect failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Get two numbers from the user
    printf("Client: Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    // Send the numbers to the server
    snprintf(buffer, sizeof(buffer), "%d %d", num1, num2);
    if (send(sock, buffer, strlen(buffer), 0) == -1) {
        perror("Send failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Client: Sent numbers %d and %d to server.\n", num1, num2);

    // Receive the sum from the server
    int bytes_read = read(sock, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Read failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Print the result
    printf("Client: Received sum from server: %s\n", buffer);

    // Close the socket
    close(sock);

    printf("Client: Exiting.\n");
    return 0;
}

