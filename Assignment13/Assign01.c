#include <stdio.h>
#include <pthread.h>

// Thread function to perform selection sort
void* selection_sort(void *param) {
    int *arr = (int *)param;
    int n = 10; // Size of the array

    // Selection Sort Algorithm
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }

    return NULL;
}

int main() {
    pthread_t tid;
    int arr[10];

    // Input 10 integers
    printf("Enter 10 integers:\n");
    for (int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }

    // Create a thread to sort the array
    pthread_create(&tid, NULL, selection_sort, (void*)arr);

    // Wait for the thread to complete
    pthread_join(tid, NULL);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

