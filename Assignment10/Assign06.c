#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHM_KEY 0x1234
#define QUEUE_CAPACITY 5

// Circular Queue structure
struct CircularQueue {
    int arr[QUEUE_CAPACITY];
    int front;
    int rear;
    int size;
};

// Function to insert an element into the circular queue
void enqueue(struct CircularQueue *queue, int value) {
    if (queue->size == QUEUE_CAPACITY) {
        printf("Queue is full, cannot enqueue.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % QUEUE_CAPACITY;
    queue->arr[queue->rear] = value;
    queue->size++;
    printf("Enqueued: %d\n", value);
}

// Function to remove an element from the circular queue
int dequeue(struct CircularQueue *queue) {
    if (queue->size == 0) {
        printf("Queue is empty, cannot dequeue.\n");
        return -1;
    }
    int value = queue->arr[queue->front];
    queue->front = (queue->front + 1) % QUEUE_CAPACITY;
    queue->size--;
    printf("Dequeued: %d\n", value);
    return value;
}

int main() {
    int shmid;
    struct CircularQueue *queue;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct CircularQueue), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach to the shared memory
    queue = (struct CircularQueue *)shmat(shmid, NULL, 0);
    if (queue == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize the circular queue
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;

    // Fork the process into parent and child
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) { // Parent process
        srand(time(0));
        while (1) {
            // Add a random number to the queue if it's not full
            if (queue->size < QUEUE_CAPACITY) {
                int random_number = rand() % 100;
                enqueue(queue, random_number);
            }
            sleep(1); // Wait before adding next number
        }
    } else { // Child process
        while (1) {
            // Remove a number from the queue if it's not empty
            if (queue->size > 0) {
                dequeue(queue);
            }
            sleep(2); // Wait before removing next number
        }
    }

    // Detach from the shared memory (this will never be reached in this example)
    shmdt(queue);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

