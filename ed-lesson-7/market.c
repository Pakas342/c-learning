#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define INPUT_BUFF (100)

struct purchase_lines {
    char *item;
    unsigned quiantity;
    unsigned cost;
};

// This should be implemented as a priority queue. I'll check on insert
struct purchase {
    struct purchase_lines **items;
};

struct client {
    char *name;
    char *phone;
    time_t checkout;
    void *purchase;
};

int main() {
    // str buffer
    char *str = malloc(sizeof(char) * INPUT_BUFF);
    if (!str) {
        return -1;
    }
    long str_len = 0;
    while ((str_len = read(STDIN_FILENO, str, INPUT_BUFF)) > 0) {
        write(STDOUT_FILENO, str, str_len);
    }
}
