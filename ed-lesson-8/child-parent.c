#include <bits/types/siginfo_t.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Volatile so its read from memory everytime, sig_atomic_t so is a blocking
// action at hardware level (wri/red), making it impossible for the sighandler
// to interrupt on a middle of a write, thus breaking the write to it

volatile sig_atomic_t parent;
volatile sig_atomic_t stop_counter;
volatile sig_atomic_t ppid_to_notify;

char *parent_messages[] = {
    "Have you done your homework?\n",
    "The homework that is due tomr, so you can pass COMP9017\n",
    "You are a great child, I'm so proud of you\n"};
char *child_messages[] = {
    "What are you talking about?\n",
    "Oh, that one. Of course\n",
};

void sig_handler(int signo, siginfo_t *sigino, void *context) {
    if (parent) {
        write(STDOUT_FILENO, parent_messages[stop_counter],
              strlen(parent_messages[stop_counter]));
    } else {
        write(STDOUT_FILENO, child_messages[stop_counter],
              strlen(child_messages[stop_counter]));
    }
    stop_counter++;
}

int main() {
    struct sigaction sig;
    // setting to 0 the memory space in sigaction.
    // same can be achieved with = {0}
    memset(&sig, 0, sizeof(struct sigaction));

    sig.sa_sigaction = sig_handler;
    sig.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sig, NULL)) {
        perror("error while configuring sigaction");
        return -1;
    }

    // Now we have a sigaction configured, but the sigaction doesn't know if
    // it's the parent of the child as we can't pass it values, we have to
    // define a global val for that. Also, if we do that inside child or parent
    // without blocking, that could cause a race condition where we set the
    // handler, the handle triggers right between the fork an the globl var, and
    // then the handler doesn't know if he's the parent or the child.
    //
    // For his case, we use sigprocmask, that a allow us to block a sigset_t of
    // signals. a sigset_t is just a bitmask, like the permissions for files
    // one, but posix gives us some functions to handle it easier.

    // STEP 1: configuring the mask
    sigset_t mask;
    sigset_t emptymask;
    sigemptyset(&mask);
    sigemptyset(&emptymask);
    // equivalent to: memset(&sigmask, 0, sizeof(sigset_t));
    sigaddset(&mask, SIGINT);

    // STEP 2: activating the mask with block instruction
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // way of getting parent id, call before fork
    // pid_t ppid = getpid();
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error while forking\n");
        return -1;
    } else if (pid > 0) {
        // PARENT

        parent = 1;
        ppid_to_notify = pid;

        // now that the process identified itself, we can unlock it
        sigprocmask(SIG_UNBLOCK, &mask, NULL);

        write(STDOUT_FILENO, parent_messages[0], strlen(parent_messages[0]));
        stop_counter++;

        // notify and sleep, just chilling!

        // BUT WAIT! What happens if this is not block and the sig arrives on
        // the while condition check? or even before! then you're kind of
        // fucked as the signal handler triggers, and then we wait for a signal
        // that might don't happen again as it already happen once, so block
        // again while confirming!

        sigprocmask(SIG_BLOCK, &mask, NULL);
        // notify the kid
        kill(ppid_to_notify, SIGINT);

        while (stop_counter <= 2) {
            // sleep until receiving a signal from our mask. Just chilling
            sigsuspend(&emptymask);
            kill(ppid_to_notify, SIGINT);
        }
        wait(NULL);
    } else {
        // CHILD

        parent = 0;
        // another way to get parent id
        ppid_to_notify = getppid();

        // now that the process identified itself, we can unlock it
        // sigprocmask(SIG_UNBLOCK, &mask, NULL);
        //
        // THIS WAS BUGGYYYYYYYYYYYYYY What was happening was that the parent
        // sent a signal before the child execute, so as soon as this was alloes
        // to handle, it did. We need to only unlock when we're ready to handle,
        // aka, the suspend

        while (stop_counter <= 1) {
            // sleep until receiving a signal from our mask. Just chilling
            sigsuspend(&emptymask);
            kill(ppid_to_notify, SIGINT);
        }
    }
}
