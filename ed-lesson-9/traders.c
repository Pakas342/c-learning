#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#define MAX_STOCKS (5)

struct stock {
    char name[10];
    float price;
};

struct shared_data {
    struct stock stocks[MAX_STOCKS];
    int updated;
};

void stocks_updater(struct shared_data *shared_stocks) {
    while (1) {
        float price = rand() % 1000 + 1;
        int stock_to_update = rand() % MAX_STOCKS;
        shared_stocks->stocks[stock_to_update].price = price;
        shared_stocks->updated = 1;

        sleep(rand() % 5);
    }
    exit(0); // shouldn't happen, but just in case
}

void stocks_publish(struct shared_data *shared_stocks) {
    printf("Stock update!\n");
    for (int i = 0; i< MAX_STOCKS; i++) {
        struct stock cur_stock = shared_stocks->stocks[i];
        printf("Price of %s: $%.2f\n", cur_stock.name, cur_stock.price);
    }
}

char stock_names[MAX_STOCKS][10] = {"gold", "silver", "copper", "iron", "oil"};

int main() {

    int mem_rules = PROT_READ | PROT_WRITE;
    int mem_flags = MAP_SHARED | MAP_ANONYMOUS;
    struct shared_data *shared_stocks =
        mmap(NULL, sizeof(struct shared_data), mem_rules, mem_flags, -1, 0);

    for (int i = 0; i < MAX_STOCKS; i++) {
        memcpy(shared_stocks->stocks[i].name, stock_names[i], 10);
    }

    pid_t pid = fork();
    if (pid < 0) {
        exit(-1);
    } else if (pid == 0) {
        stocks_updater(shared_stocks);
    }

    while (1) {
        if (shared_stocks->updated == 1) {
            stocks_publish(shared_stocks);
            shared_stocks->updated = 0;
        } else {
            sleep(1);
        }
    }
    exit(0); // shouldn't happen, but just in case
}
