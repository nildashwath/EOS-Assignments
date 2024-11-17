#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define BUFFER_SIZE 5

// Struct to hold shared data
struct shared_data {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    pthread_mutex_t mutex;
    sem_t empty_slots;
    sem_t full_slots;
};

// Pointer to shared data
struct shared_data *shared;

// Signal handler to cleanup
void cleanup(int signum) {
    printf("\nCleaning up shared resources...\n");

    // Destroy semaphores and mutex
    sem_destroy(&shared->empty_slots);
    sem_destroy(&shared->full_slots);
    pthread_mutex_destroy(&shared->mutex);

    // Unmap and unlink shared memory
    munmap(shared, sizeof(struct shared_data));
    shm_unlink("/shm_producer_consumer");

    exit(0);
}

// Producer function
void producer() {
    int item = 0;
    while (1) {
        // Produce an item
        item = rand() % 100;

        // Wait for empty slot
        sem_wait(&shared->empty_slots);

        // Lock the mutex
        pthread_mutex_lock(&shared->mutex);

        // Add item to buffer
        shared->buffer[shared->in] = item;
        printf("Producer produced: %d\n", item);
        shared->in = (shared->in + 1) % BUFFER_SIZE;

        // Unlock the mutex
        pthread_mutex_unlock(&shared->mutex);

        // Signal that a full slot is available
        sem_post(&shared->full_slots);

        sleep(1);
    }
}

// Consumer function
void consumer() {
    int item;
    while (1) {
        // Wait for a full slot
        sem_wait(&shared->full_slots);

        // Lock the mutex
        pthread_mutex_lock(&shared->mutex);

        // Remove item from buffer
        item = shared->buffer[shared->out];
        printf("Consumer consumed: %d\n", item);
        shared->out = (shared->out + 1) % BUFFER_SIZE;

        // Unlock the mutex
        pthread_mutex_unlock(&shared->mutex);

        // Signal that an empty slot is available
        sem_post(&shared->empty_slots);

        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    // Register signal handler for cleanup
    signal(SIGINT, cleanup);

    // Create or open shared memory
    int shm_fd = shm_open("/shm_producer_consumer", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct shared_data));

    // Map shared memory
    shared = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Initialize shared buffer indices
    shared->in = 0;
    shared->out = 0;

    // Initialize mutex with PTHREAD_PROCESS_SHARED attribute
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared->mutex, &mutex_attr);

    // Initialize semaphores with non-zero key
    sem_init(&shared->empty_slots, 1, BUFFER_SIZE);
    sem_init(&shared->full_slots, 1, 0);

    if (argc > 1 && strcmp(argv[1], "consumer") == 0) {
        consumer();
    } else {
        producer();
    }

    return 0;
}

