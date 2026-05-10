#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define MAX_EVENTS 16
#define BUF_SIZE 1024
#define MAX_LANES 4

typedef struct {
    int fd;
    int lane_id;
    char buf[BUF_SIZE];
    size_t len;
} lane_t;

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* ---------------- Car Generator ---------------- */
void car_generator(const char *fifo_name, int id) {
    signal(SIGPIPE, SIG_IGN); // prevent crash if reader disappears

    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open writer");
        exit(1);
    }

    srand(getpid());

    while (1) {
        int car_id = rand() % 1000;
        int amount = (rand() % 20) + 1;

        char msg[64];
        int len = snprintf(msg, sizeof(msg), "CAR-%d:%d\n", car_id, amount);

        write(fd, msg, len);

        printf("[GEN %d] %s", id, msg);

        sleep(rand() % 3); // 0–2 sec
    }
}

/* ---------------- Line Processing ---------------- */
void process_lines(lane_t *lane) {
    char *start = lane->buf;
    char *newline;

    while ((newline = memchr(start, '\n', lane->len - (start - lane->buf)))) {

        *newline = '\0';

        char car[64];
        int amount;

        if (sscanf(start, "%63[^:]:%d", car, &amount) == 2) {
            printf("[LANE %d] Car %s paid %d\n", lane->lane_id, car, amount);
        } else {
            printf("[LANE %d] Invalid: %s\n", lane->lane_id, start);
        }

        start = newline + 1;
    }

    size_t remaining = lane->buf + lane->len - start;
    memmove(lane->buf, start, remaining);
    lane->len = remaining;
}

/* ---------------- Main ---------------- */
int main() {
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(1);
    }

    lane_t lanes[MAX_LANES];

    struct epoll_event eve;
    eve.events = EPOLLIN;

    /* ---------- Create FIFOs + open readers ---------- */
    for (int i = 0; i < MAX_LANES; i++) {
        char fifo_name[10] = {0};
        sprintf(fifo_name, "fifo_%d", i);
        // octal number of 3 digist. 6 = 110 aka RW no execute (x)
        mode_t mode = 0666;
        mkfifo(fifo_name, mode);

        // This is important! without it, if epoll triggers, and something reads
        // before me, then I'm stuck :(
        int fd = open(fifo_name, O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            perror("open reader");
            exit(1);
        }

        lanes[i] = (lane_t){
            .fd = fd,
            .lane_id = i,
            .buf = {0},
        };

        eve.data.ptr = lanes + i;
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &eve);
    }

    // Forks N car generator processes

    /* ---------- Fork car generators ---------- */
    for (int i = 0; i < MAX_LANES; i++) {
        lane_t lane = lanes[i];
        char fifo_name[10] = {0};
        sprintf(fifo_name, "fifo_%d", lane.lane_id);
        pid_t pid = fork();
        if (pid < 0) {
            exit(-1);
        } else if (pid == 0) {
            // child
            car_generator(fifo_name, i);
            exit(-1); // this should never be reached as while (1) on car gen
        }
    }

    struct epoll_event events[MAX_EVENTS];

    /* ---------- Event loop ---------- */
    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n; i++) {
            lane_t *lane = (lane_t *)events[i].data.ptr;
            lane->len += read(lane->fd, lane->buf, BUF_SIZE);
            process_lines(lane);
        }
    }

    // I should wait for all my car children, wait for all my processing lines
    // children, close all the file descriptors, do a bunch of stuff, but nah
    close(epfd);
    return 0;
}
