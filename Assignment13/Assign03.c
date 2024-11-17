#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Mutex and condition variables
pthread_mutex_t mutex;
pthread_cond_t cond_var;

// Shared variable to control which thread should print
int sunbeam_turn = 1; // 1 means "SUNBEAM" thread's turn, 0 means "INFOTECH" thread's turn

// Thread function to print "SUNBEAM"
void* print_sunbeam(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait until it's SUNBEAM's turn
        while (sunbeam_turn == 0) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        // Print "SUNBEAM" and change the turn
        printf("SUNBEAM\n");
        fflush(stdout); // Ensures the output is printed immediately
        sunbeam_turn = 0; // Now it's INFOTECH's turn

        // Signal the other thread to continue
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);

        // Sleep for a while to allow the other thread to run
        usleep(500000); // 0.5 seconds
    }

    return NULL;
}

// Thread function to print "INFOTECH"
void* print_infotech(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait until it's INFOTECH's turn
        while (sunbeam_turn == 1) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        // Print "INFOTECH" and change the turn
        printf("INFOTECH\n");
        fflush(stdout); // Ensures the output is printed immediately
        sunbeam_turn = 1; // Now it's SUNBEAM's turn

        // Signal the other thread to continue
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);

        // Sleep for a while to allow the other thread to run
        usleep(500000); // 0.5 seconds
    }

    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    // Initialize mutex and condition variable
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    if (pthread_cond_init(&cond_var, NULL) != 0) {
        perror("Condition variable initialization failed");
        return 1;
    }

    // Create threads
    if (pthread_create(&tid1, NULL, print_sunbeam, NULL) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_create(&tid2, NULL, print_infotech, NULL) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    // Wait for threads to finish (infinite loop, so this won't happen)
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}

