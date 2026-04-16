#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define INTERRUPT_MSG "You tried to divide by zero?\n"

void sig_handler(int signo, siginfo_t *sinfo, void *context) {
    write(STDERR_FILENO, INTERRUPT_MSG, strlen(INTERRUPT_MSG));
    _exit(1);
}

int main(void) {
    // sigaction type of thing, memset sets the memory on an address for the
    // given number of bytes
    struct sigaction sig;
    memset(&sig, 0, sizeof(struct sigaction));
    sig.sa_sigaction = sig_handler;

    // constant that tells our sig what should pass to the handler
    sig.sa_flags = SA_SIGINFO;

    // the NULL is an address to where you want to save the previously linked
    // sig handler
    if (sigaction(SIGFPE, &sig, NULL)) {
        perror("Sigaction failed for some reason");
        return 0;
    }

    int a, b;
    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);
    int result = a / b;
    printf("Result: %d\n", result);

    return 0;
}
