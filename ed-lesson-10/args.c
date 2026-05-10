#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10 // no of threads

void* routine(void* args){
    int* id = args;
    printf("Hello From Thread %d!\n", *id);
}

int main(){
    pthread_t threads[NTHREADS];
    // this can also be created on the heap as well
    // eg. pthread_t* threads = malloc(sizeof(pthread_t) * NTHREADS);

    for(int i = 0; i < NTHREADS; i++){
        //create the threads using the create function
        pthread_create(threads + i, NULL, routine, &i);
    }

    for(int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
}
