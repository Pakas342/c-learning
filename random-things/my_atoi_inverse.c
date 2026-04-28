#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void helper(uintptr_t n, uintptr_t jump, char *numchar, uintptr_t org_jump,
            uintptr_t base);
uintptr_t inverse_atoi(uintptr_t n, char *numchar, uintptr_t base);

char values[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


uintptr_t inverse_atoi(uintptr_t n, char *numchar, uintptr_t base) {
    uintptr_t max_digit = n;
    uintptr_t jump = 0;
    while (max_digit >= base) {
        max_digit = max_digit / base;
        jump++;
    }
    // Malloc  is not signal safe, dumb ass
    // *numchar = malloc(sizeof(char) * (jump + 1));

    helper(n, jump, numchar, jump, base);
    return jump + 1;
}

void helper(uintptr_t n, uintptr_t jump, char *numchar, uintptr_t org_jump,
            uintptr_t base) {
    uintptr_t jump_val = 1;
    for (int i = 0; i < jump; i++) {
        jump_val = jump_val * base;
    }
    uintptr_t cur = n / jump_val;
    uintptr_t pending = n - (cur * jump_val);
    numchar[org_jump - jump] = values[cur];
    if (jump > 0) {
        helper(pending, jump - 1, numchar, org_jump, base);
    }
}

int main(int argc, char *argv[]) {
    uintptr_t val = strtoull(argv[1], NULL, 10);
    uintptr_t base = strtoull(argv[2], NULL, 10);
    char str[32] = {};
    errno = 0;
    uintptr_t num_len = inverse_atoi(val, str, base);
    if (errno) {
        perror("error on inverse atoir :(");
        return -1;
    }
    write(STDOUT_FILENO, str, num_len);
}
