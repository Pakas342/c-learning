#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void helper(uintptr_t n, unsigned jump, char *numchar, unsigned org_jump,
            unsigned base);

char values[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

unsigned
inverse_atoi(uintptr_t n, char *numchar, unsigned base) {
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = n * -1;
    }
    unsigned max_digit = n;
    unsigned jump = 0;
    while (max_digit >= base) {
        max_digit = max_digit / base;
        jump++;
    }
    // Malloc  is not signal safe, dumb ass
    // *numchar = malloc(sizeof(char) * (jump + 1));

    if (negative) {

        helper(n, jump, numchar + 1, jump, base);
        numchar[0] = '-';
        return jump + 2;
    } else {
        helper(n, jump, numchar, jump, base);
        return jump + 1;
    }
}

void helper(uintptr_t n, unsigned jump, char *numchar, unsigned org_jump,
            unsigned base) {
    unsigned jump_val = 1;
    for (int i = 0; i < jump; i++) {
        jump_val = jump_val * base;
    }
    unsigned cur = n / jump_val;
    unsigned pending = n - (cur * jump_val);
    numchar[org_jump - jump] = values[cur]; 
    if (jump > 0) {
        helper(pending, jump - 1, numchar, org_jump, base);
    }
}

int main(int argc, char *argv[]) {
    int val = atoi(argv[1]);
    unsigned base = atoi(argv[2]);
    char str[32] = {};
    errno = 0;
    unsigned num_len = inverse_atoi(val, str, base);
    if (errno) {
        perror("error on inverse atoir :(");
        return -1;
    }
    write(STDOUT_FILENO, str, num_len);
}
