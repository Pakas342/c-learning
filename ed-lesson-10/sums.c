#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000

int *get_nums() {
    srand(time(NULL));

    int *arr = malloc(sizeof(int) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 10000; // keeping the range 0-10000
    }

    return arr;
}

struct sum_input {
    int *array;
    int len;
};
void *sum_until(void *array) {
    long long *result = calloc(1, sizeof(long long));
    struct sum_input *input = (struct sum_input *)array;
    for (int i = 0; i < input->len; i++) {
        *result += *(input->array + i);
    }
    pthread_exit(result);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("give me a number dude\n");
        exit(-1);
    }
    int thread_count = atoi(argv[1]);
    pthread_t threads[thread_count];
    int *large_array = get_nums();
    long long sum = 0; // add all the numbers in large_array sum should have it.
    struct sum_input inputs[thread_count];
    for (int i = 0; i < thread_count; i++) {
        int nums_to_process = SIZE / thread_count;
        int offset = SIZE / thread_count;
        if (i == thread_count - 1) {
            // giving the last thread the leftovers
            nums_to_process += SIZE % thread_count;
        }
        // this should work because I'm creating it on every loop, so a new
        // value and address should be created into main thread stack, thus
        // children threads should be able to read it
        inputs[i] = (struct sum_input){.array = large_array + offset * i,
                                       .len = nums_to_process};
        if (pthread_create(threads + i, NULL, sum_until, inputs + i) != 0) {
            exit(-1);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        void *returned;
        pthread_join(threads[i], &returned);
        sum += *(long long *)returned;
        free(returned);
    }

    printf("%lld\n", sum);

    free(large_array);
}
