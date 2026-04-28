#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void helper(intmax_t n, intmax_t jump, char *numchar, intmax_t org_jump,
            intmax_t base);
intmax_t inverse_atoi(intmax_t n, char *numchar, intmax_t base);

char values[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};



intmax_t inverse_atoi(intmax_t n, char *numchar, intmax_t base) {
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = n * -1;
    }
    intmax_t max_digit = n;
    intmax_t jump = 0;
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

void helper(intmax_t n, intmax_t jump, char *numchar, intmax_t org_jump,
            intmax_t base) {
    intmax_t jump_val = 1;
    for (int i = 0; i < jump; i++) {
        jump_val = jump_val * base;
    }
    intmax_t cur = n / jump_val;
    intmax_t pending = n - (cur * jump_val);
    numchar[org_jump - jump] = values[cur];
    if (jump > 0) {
        helper(pending, jump - 1, numchar, org_jump, base);
    }
}

int main(int argc, char *argv[]) {
    intmax_t val = strtoull(argv[1], NULL, 10);
    intmax_t base = strtoull(argv[2], NULL, 10);
    char str[32] = {};
    errno = 0;
    intmax_t num_len = inverse_atoi(val, str, base);
    if (errno) {
        perror("error on inverse atoir :(");
        return -1;
    }
    write(STDOUT_FILENO, str, num_len);
}
