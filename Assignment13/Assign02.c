#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struct to hold the array and its size
struct array {
    int *arr;
    int size;
};

// Thread function to perform bubble sort
void* bubble_sort(void *param) {
    struct array *arr_struct = (struct array *)param;
    int *arr = arr_struct->arr;
    int n = arr_struct->size;

    // Bubble Sort Algorithm
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    return NULL;
}

int main() {
    pthread_t tid;
    struct array arr_struct;

    // Input the size of the array
    printf("Enter the number of elements in the array: ");
    scanf("%d", &arr_struct.size);

    // Dynamically allocate memory for the array
    arr_struct.arr = (int *)malloc(arr_struct.size * sizeof(int));
    if (arr_struct.arr == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // Input the elements of the array
    printf("Enter %d integers:\n", arr_struct.size);
    for (int i = 0; i < arr_struct.size; i++) {
        scanf("%d", &arr_struct.arr[i]);
    }

    // Create a thread to sort the array
    pthread_create(&tid, NULL, bubble_sort, (void *)&arr_struct);

    // Wait for the thread to complete
    pthread_join(tid, NULL);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < arr_struct.size; i++) {
        printf("%d ", arr_struct.arr[i]);
    }
    printf("\n");

    // Free the dynamically allocated memory
    free(arr_struct.arr);

    return 0;
}

