#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int grapes = 100;
pthread_mutex_t plate = PTHREAD_MUTEX_INITIALIZER;

void eat_grape() {
    grapes--;
    sleep(1);
}

void *joey(void *arg) {
    // TO DO (keep eating till there are no grapes)
    pthread_mutex_lock(&plate);
    while (grapes > 0) {
        printf("Joey: I'll eat all of them!!!!\n");
        grapes--;
    }
    // After the logic
    printf("Joey: JOEY DOESN'T SHARE FOOD!\n");
    pthread_mutex_unlock(&plate);
    return NULL;
}

void *emma(void *arg) {
    // TO DO eat one grape and let go of the plate
    while (grapes > 0) {
        printf("Emma: I'll take one grape :)\n");
        pthread_mutex_lock(&plate);
        grapes--;
        pthread_mutex_unlock(&plate);
    }
    return NULL;
}

int main() {
    // init the threads and spawn joey and emma, and join
    pthread_t emma_pt;
    pthread_t joey_pt;

    pthread_create(&emma_pt, NULL, emma, NULL);
    pthread_create(&joey_pt, NULL, joey, NULL);

    pthread_join(emma_pt, NULL);
    pthread_join(joey_pt, NULL);
}
