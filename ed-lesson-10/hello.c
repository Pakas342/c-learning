#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREADS (40) // no of threads

// create a function that returns void* and takes in void* arg
void *hello_world(void *arg) {
    printf("Hello world!\n");
    return NULL;
}

int main() {
    pthread_t threads[NTHREADS];
    // this can also be created on the heap as well
    // eg. pthread_t* threads = malloc(sizeof(pthread_t) * NTHREADS);

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(threads + i, NULL, hello_world, NULL) != 0) {
            exit(-1);
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
        // required as threads can be zombies too
        pthread_join(threads[i], NULL);
    }
}
