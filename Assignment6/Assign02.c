#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void child_process_d() {
    printf("Child D (PID %d) started by process C (PID %d)\n", getpid(), getppid());
    sleep(5);  // Simulate work
    printf("Child D (PID %d) terminating\n", getpid());
    exit(0);  // Exit child D
}

void child_process_c() {
    printf("Child C (PID %d) started by process B (PID %d)\n", getpid(), getppid());
    if (fork() == 0) {
        child_process_d();  // Create Child D
    }
    sleep(5);  // Simulate work
    printf("Child C (PID %d) terminating\n", getpid());
    exit(0);  // Exit child C
}

void child_process_b() {
    printf("Child B (PID %d) started by process A (PID %d)\n", getpid(), getppid());
    if (fork() == 0) {
        child_process_c();  // Create Child C
    }
    sleep(5);  // Simulate work
    printf("Child B (PID %d) terminating\n", getpid());
    exit(0);  // Exit child B
}

int main() {
    printf("Parent A (PID %d) creating child B\n", getpid());
    if (fork() == 0) {
        child_process_b();  // Create Child B
    }
    sleep(5);  // Simulate work
    printf("Parent A (PID %d) terminating\n", getpid());
    exit(0);  // Exit Parent A
    return 0;
}

