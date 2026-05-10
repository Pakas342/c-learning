#include <asm-generic/errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_CHILDREN (5)
#define BUF_SIZE (500)

static volatile int got_sigusr1 = 0;

static void on_sigusr1(int sig) {
    (void)sig;
    got_sigusr1 = 1;
}

int main(void) {
    int pipes[N_CHILDREN][2];
    pid_t pids[N_CHILDREN];

    for (int i = 0; i < N_CHILDREN; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // --- child ---
            close(pipes[i][0]);

            int devnull = open("/dev/null", O_RDONLY);
            dup2(devnull, STDIN_FILENO);
            close(devnull);

            dup2(pipes[i][1], STDOUT_FILENO);
            dup2(pipes[i][1], STDERR_FILENO);
            close(pipes[i][1]);

            // install handler for SIGUSR1
            struct sigaction sa = {0};
            sa.sa_handler = on_sigusr1;
            sigemptyset(&sa.sa_mask);
            sigaction(SIGUSR1, &sa, NULL);

            // send one message, then wait for the parent's signal
            printf("child %d: hello from pid %d\n", i, getpid());
            fflush(stdout);

            while (!got_sigusr1)
                pause();

            _exit(0);
        }

        // --- parent ---
        pids[i] = pid;
        close(pipes[i][1]);
    }

    char buf[BUF_SIZE];
    short notified[N_CHILDREN] = {0};

    // TODO (poll version):
    //   build a struct pollfd[] from pipes[i][0], call poll() in a loop,
    //   and read from any pipe that signals POLLIN.
    //   After reading from child i's pipe, kill(pids[i], SIGUSR1) to
    //   release that child. Track which children have been signalled
    //   so each is signalled exactly once.

    // In the real thing I should do this inside the same loop, but I want to do
    // it here to keep all the implementations separated
    // struct pollfd fds[N_CHILDREN];
    // for (int i = 0; i < N_CHILDREN; i++) {
    //     fds[i].fd = pipes[i][0];
    //     fds[i].events = POLLIN;
    // }

    // // -1 in timeout sets until something happens, 0 don't wait, any other value
    // // a proper timeout

    // int ret;
    // while (1) {
    //     ret = poll(fds, N_CHILDREN, -1);
    //     if (ret > 0) {
    //         for (int j = 0; j < N_CHILDREN; j++) {
    //             // checking if the events include pollin as this is a mask
    //             if (fds[j].revents & POLLIN) {
    //                 int len = read(fds[j].fd, buf, BUF_SIZE);
    //                 write(STDOUT_FILENO, buf, len);
    //                 kill(pids[j], SIGUSR1);
    //                 notified[j] = 1;
    //             }
    //         }
    //     }
    //     int not_notified = 0;
    //     for (int j = 0; j < N_CHILDREN; j++) {
    //         if (notified[j] == 0) {
    //             not_notified++;
    //         }
    //     }
    //     if (not_notified == 0) {
    //         break;
    //     }
    // }

    // TODO (epoll version):
    //   epoll_create1(0), register each pipes[i][0] with EPOLL_CTL_ADD,
    //   then loop on epoll_wait() and read from the ready fds, sending
    //   SIGUSR1 to the corresponding child after each successful read.

    int epfd = epoll_create1(0);
    if (epfd < 0) {
        exit(1);
    }
    struct epoll_event eve, events[N_CHILDREN];
    eve.events = EPOLLIN;

    struct child {
        int fd;
        pid_t pid;
    };

    struct child chidren[N_CHILDREN] = {0};

    for (int i = 0; i < N_CHILDREN; i++) {
        chidren[i].fd = pipes[i][0];
        chidren[i].pid = pids[i];
        eve.data.ptr = chidren + i;
        epoll_ctl(epfd, EPOLL_CTL_ADD, pipes[i][0], &eve);
    }

    int n;
    memset(notified, 0, N_CHILDREN);
    while (1) {
        n = epoll_wait(epfd, events, N_CHILDREN, -1);
        for (int j = 0; j < n; j++) {
            struct epoll_event current_eve = events[j];
            struct child *cur_child = current_eve.data.ptr;
            int len = read(cur_child->fd, buf, BUF_SIZE);
            write(STDOUT_FILENO, buf, len);
            kill(cur_child->pid, SIGUSR1);
            notified[j] = 1;
        }

        int not_notified = 0;
        for (int j = 0; j < N_CHILDREN; j++) {
            if (notified[j] == 0) {
                not_notified++;
            }
        }
        if (not_notified == 0) {
            break;
        }
    }

    // --------END--------
    for (int i = 0; i < N_CHILDREN; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return 0;
}
