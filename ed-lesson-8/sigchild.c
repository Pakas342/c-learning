#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void helper(long n, unsigned dec, char *numchar, unsigned org_dec);
unsigned inverse_atoi(long n, char *numchar);

// I need to transform an INT number into decimal
static void handler(int sig, siginfo_t *info, void *uctx) {
    // DONE: inspect info->si_code, info->si_pid, info->si_status
    char buf[32] = {};
    char mess[] = "SIGCHILD received, info:\nsi_code is: ";
    unsigned num_len = inverse_atoi(info->si_code, buf);
    buf[num_len] = '\n';
    write(STDOUT_FILENO, mess, sizeof(mess));
    write(STDOUT_FILENO, buf, num_len + 1);
    char mess_2[] = "si_pid is: ";
    num_len = inverse_atoi(info->si_pid, buf);
    buf[num_len] = '\n';
    write(STDOUT_FILENO, mess_2, sizeof(mess_2));
    write(STDOUT_FILENO, buf, num_len + 1);
    char mess_3[] = "si_status is: ";
    num_len = inverse_atoi(info->si_status, buf);
    buf[num_len] = '\n';
    write(STDOUT_FILENO, mess_3, sizeof(mess_3));
    write(STDOUT_FILENO, buf, num_len + 1);

    waitpid(info->si_pid, NULL, 0);

    // DONE: reap the child with waitpid(info->si_pid, ...)
    // This might work, but the thing is that, signals of the same type get
    // blocked while the handler is handling, they don't go recursive. This
    // means we might lose some signals if they arrive while the handler is
    // handling. A more robust approach is to waitpid with -1 (which means any
    // terminated) and give a non blocking flag so the wait returns right away
    // if no kid has terminated. That way we get to wait all kids that finished
    // since the signal was received and the handler finishes
    // 
    // killed, and thus we finished Another reason for the prev over, say,
    // allowing the handler to get call by the same signal while executing
    // (there's a flag for that) is that even if you allow that, if two
    // concurrent child end at the same time, you still lost one of them as the
    // pending signal is just a bit alert, so if two things turn it of super
    // close to each other, we'll only handle one assuming it was only one who
    // turned it on. That's way is better to think about the sigchild as "At
    // least one child finished" and not as a "one child finished"
}

int main(void) {
    // DONE: install handler with sigaction (SA_SIGINFO | SA_NOCLDSTOP)
    struct sigaction sig;
    memset(&sig, 0, sizeof(struct sigaction));
    sig.sa_sigaction = handler;
    sig.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sig, NULL)) {
        printf("error on configuring signal handler");
        return -1;
    }

    int sleeps[3] = {1, 2, 3};
    int statuses[3] = {10, 20, 30};

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // child
            sleep(sleeps[i]);
            if (i == 0) {
                abort();
            } else if (i == 1) {
                exit(statuses[i]);
            } else {
                exit(statuses[i]);
            }
        }

        // parent continues the loop to fork the next child
    }

    // parent waits for handler to reap all three
    while (1)
        pause();
    return 0;
}

unsigned inverse_atoi(long n, char *numchar) {
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = n * -1;
    }
    unsigned max_digit = n;
    unsigned dec = 0;
    while (max_digit >= 10) {
        max_digit = max_digit / 10;
        dec++;
    }
    // Malloc  is not signal safe, dumb ass
    // *numchar = malloc(sizeof(char) * (dec + 1));

    helper(n, dec, numchar, dec);
    return dec + 1;
}

void helper(long n, unsigned dec, char *numchar, unsigned org_dec) {
    unsigned dec_val = 1;
    for (int i = 0; i < dec; i++) {
        dec_val = dec_val * 10;
    }
    unsigned cur = n / dec_val;
    unsigned pending = n - (cur * dec_val);
    numchar[org_dec - dec] = cur + 48; // offset of the ascci val
    if (dec > 0) {
        helper(pending, dec - 1, numchar, org_dec);
    }
}
