#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <wait.h>

// Define a structure for the message
struct message {
    long msg_type; // Message type
    int number1;   // First number
    int number2;   // Second number
    int result;    // Sum of two numbers (sent from parent to child)
};

int main() {
    key_t key;          // Unique key for the message queue
    int msgid;          // Message queue ID
    pid_t pid;          // Process ID for fork()

    // Create a unique key
    key = ftok("progfile", 65);

    // Create a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("Failed to create message queue\n");
        return 1;
    }

    // Create a child process
    pid = fork();

    if (pid == 0) { // Child process
        struct message msg;
        msg.msg_type = 1;  // Message type 1 for sending numbers

        // Assign two numbers to send
        msg.number1 = 5;   // First number
        msg.number2 = 10;  // Second number

        printf("Child: Sending numbers %d and %d to parent.\n", msg.number1, msg.number2);

        // Send the numbers to the parent process
        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        // Receive the sum from the parent process
        msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 2, 0);

        // Print the result
        printf("Child: Received sum from parent: %d\n", msg.result);
        
        // Exit the child process
        exit(0);

    } else { // Parent process
        struct message msg;

        // Receive the numbers from the child process
        msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);
        printf("Parent: Received numbers %d and %d from child.\n", msg.number1, msg.number2);

        // Calculate the sum
        msg.result = msg.number1 + msg.number2;
        msg.msg_type = 2;  // Message type 2 for sending back the result

        // Send the result back to the child
        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        // Wait for the child process to finish
        wait(NULL);

        // Delete the message queue
        msgctl(msgid, IPC_RMID, NULL);

        printf("Parent: Exiting.\n");
    }

    return 0;
}

