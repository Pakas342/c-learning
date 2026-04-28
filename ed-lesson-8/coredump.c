#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void helper(uintptr_t n, uintptr_t jump, char *numchar, uintptr_t org_jump,
            uintptr_t base);
uintptr_t inverse_atoi(uintptr_t n, char *numchar, uintptr_t base);

char values[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void handler(int sig, siginfo_t *info, void *uctx) {
    // done: inspect info->si_addr and info->si_code
    char mess[] = "Segmentation fault!\n";
    write(STDOUT_FILENO, mess, sizeof(mess));
    if (info->si_code == SEGV_MAPERR) {
        char mess_2[] = "Error caused by no mapping at given address\n";
        write(STDOUT_FILENO, mess_2, sizeof(mess_2));
    } else if (info->si_code == SEGV_ACCERR) {
        char mess_2[] =
            "Error caused by no permissions to read at given address\n";
        write(STDOUT_FILENO, mess_2, sizeof(mess_2));
    } else {
        char mess_2[] = "Idk what caused the error, gbh\n";
        write(STDOUT_FILENO, mess_2, sizeof(mess_2));
    }

    // done: print the faulting address
    char mess_3[] = "The address was: ";
    write(STDOUT_FILENO, mess_3, sizeof(mess_3));
    char ptr[50];
    // casting a pointer as a int long enough to get a pointer.
    // then I have to set to a HEX.
    unsigned ptr_len = inverse_atoi((uintptr_t)(info->si_addr), ptr + 2, 16);
    // hexes start with 0x;
    ptr[0] = '0';
    ptr[1] = 'x';
    ptr[ptr_len+2] = '\n';

    write(STDOUT_FILENO, ptr, ptr_len + 3);

    // Done: _exit cleanly (don't return, or the fault will re-trigger)
    _exit(0);
}

int main(void) {
    // Done: install handler with sigaction (SA_SIGINFO)
    struct sigaction sig;
    memset(&sig, 0, sizeof(sigaction));

    sig.sa_sigaction = handler;
    sig.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sig, NULL);

    // Experiment 1: NULL deref -> SEGV_MAPERR
    int *p = NULL;
    *p = 42;

    // Experiment 2: write to a read-only mmap'd page -> SEGV_ACCERR
    // char* page = mmap(NULL, getpagesize(), PROT_READ,
    //                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // page[0] = 'x';
    //
    // OR
    //
    // access unmapped memory
    // *(int*)0x400000000000 = 42;

    return 0;
}

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
