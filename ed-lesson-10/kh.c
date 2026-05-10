#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_MEALS 4
int meals_ready = 0;
int done_wine = 0;
int done_stock = 0;
int done_herbs = 0;

void prep_wine() {
    sleep(1);
    // sleep 1 second
}
void prep_stock() {
    // sleep 2 second
    sleep(2);
}
void prep_herbs() {
    // sleep 1 second
    sleep(1);
}

struct sous_chef_order {
    int i_required;
    void (*prep)();
    char name[10];
    int *counter;
};

void *sous_chef(void *arg) {
    // pass in the string of what they are preparing in arg and print it out
    struct sous_chef_order *order = (struct sous_chef_order *)arg;
    for (int i = 0; i < order->i_required; i++) {
        order->prep();
        printf("%s chef: %s ready!\n", order->name, order->name);
        (*(order->counter))++;
    }
    pthread_exit(NULL);
}

void *head_chef(void *arg) {
    struct sous_chef_order orders[3] = {{.i_required = NUM_MEALS,
                                         .prep = prep_herbs,
                                         .name = "Herbs",
                                         .counter = &done_herbs},
                                        {.i_required = NUM_MEALS,
                                         .prep = prep_stock,
                                         .name = "Stock",
                                         .counter = &done_stock},
                                        {.i_required = NUM_MEALS,
                                         .prep = prep_wine,
                                         .name = "Wine",
                                         .counter = &done_wine}};
    pthread_t souchefs[3];

    pthread_create(souchefs, NULL, sous_chef, orders);
    pthread_create(souchefs + 1, NULL, sous_chef, orders + 1);
    pthread_create(souchefs + 2, NULL, sous_chef, orders + 2);

    while (meals_ready < NUM_MEALS) {
        if (done_herbs > 0 && done_stock > 0 && done_wine > 0) {
            printf("Head Chef: One lamb sauce ready!\n");
            meals_ready++;
            done_wine--;
            done_stock--;
            done_herbs--;
        }
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(souchefs[i], NULL);
    }

    pthread_exit(NULL);
}

void *rordon(void *arg) {
    pthread_t *main_chef;
    pthread_create(main_chef, NULL, head_chef, NULL);
    sleep(1);
    while (meals_ready < NUM_MEALS) {
        printf("WHERE IS THE LAMB SAUCE?\n");
        sleep(1);
    }
    pthread_join(*main_chef, NULL);
    pthread_exit(NULL);
    // creates head chef, and waits until there are NUM_MEALS
    // must print WHERE IS THE LAMB SAUCE until meals_ready is NUM_MEALS
}

int main() {
    pthread_t rordon_thread;
    pthread_create(&rordon_thread, NULL, rordon, NULL);
    pthread_join(rordon_thread, NULL);
}
