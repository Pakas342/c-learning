#include <signal.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 90
#define TRACKED_SIGNALS 3
#define FIRST_MESSAGE "Kill Sequence Stage 1 Primed\n"
#define SECOND_MESSAGE "Kill Sequence Stage 2 Primed\n"
#define THIRD_MESSAGE "Kill Sequence Complete, Program Exit\n"
#define WRONG_MESSAGE "Kill Sequence Aborted\n"

enum steps { first, second, third };

int SIGNALS[TRACKED_SIGNALS] = {SIGINT, SIGTSTP, SIGQUIT};

volatile enum steps step = first;

void manual_fibonacci(int *n1, int *n2) {
    *n2 = *n1 + *n2;
    *n1 = *n2 - *n1;
    return;
}

void sig_handler(int signo, siginfo_t *sinfo, void *context) {
    switch (step) {
    case (first): {
        if (signo == SIGNALS[step]) {
            write(STDOUT_FILENO, FIRST_MESSAGE, strlen(FIRST_MESSAGE));
            step = second;
            break;
        } else {
            write(STDOUT_FILENO, WRONG_MESSAGE, strlen(WRONG_MESSAGE));
            step = first;
            break;
        }
    }
    case (second): {
        if (signo == SIGNALS[step]) {
            write(STDOUT_FILENO, SECOND_MESSAGE, strlen(SECOND_MESSAGE));
            step = third;
            break;
        } else {
            write(STDOUT_FILENO, WRONG_MESSAGE, strlen(WRONG_MESSAGE));
            step = first;
            break;
        }
    }
    case (third): {
        if (signo == SIGNALS[step]) {
            write(STDOUT_FILENO, THIRD_MESSAGE, strlen(THIRD_MESSAGE));
            _exit(0);
            break;
        } else {
            write(STDOUT_FILENO, WRONG_MESSAGE, strlen(WRONG_MESSAGE));
            step = first;
            break;
        }
    }
    }
}

int main(int argc, char **argv) {
    struct sigaction sig;
    memset(&sig, 0, sizeof(struct sigaction));

    sig.sa_sigaction = sig_handler;
    sig.sa_flags = SA_SIGINFO;

    for (int i = 0; i < TRACKED_SIGNALS; i++) {
        if (sigaction(SIGNALS[i], &sig, NULL)) {
            perror("Error allocating sigaction");
            return -1;
        }
    }
    int prev1 = 1;
    int prev2 = 0;
    int resp;
    while (1) {
        manual_fibonacci(&prev1, &prev2);
        char str[BUFF_SIZE] = {0};
        snprintf(str, BUFF_SIZE, "%d\n", prev2);

        write(STDOUT_FILENO, str, BUFF_SIZE);
        sleep(5);
    }
}
