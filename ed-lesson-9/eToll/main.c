#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>

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
        int len = snprintf(msg, sizeof(msg),
                           "CAR-%d:%d\n", car_id, amount);

        write(fd, msg, len);

        printf("[GEN %d] %s", id, msg);

        sleep(rand() % 3); // 0–2 sec
    }
}


/* ---------------- Line Processing ---------------- */
void process_lines(lane_t *lane) {
    char *start = lane->buf;
    char *newline;

    while ((newline = memchr(start, '\n',
           lane->len - (start - lane->buf)))) {

        *newline = '\0';

        char car[64];
        int amount;

        if (sscanf(start, "%63[^:]:%d", car, &amount) == 2) {
            printf("[LANE %d] Car %s paid %d\n",
                   lane->lane_id, car, amount);
        } else {
            printf("[LANE %d] Invalid: %s\n",
                   lane->lane_id, start);
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

    /* ---------- Create FIFOs + open readers ---------- */
    for (int i = 0; i < MAX_LANES; i++) {
        lanes[0].lane_id = i;
        //TODO

    }

    // Forks N car generator processes

    /* ---------- Fork car generators ---------- */
    for (int i = 0; i < MAX_LANES; i++) {
        //TODO
    }

    struct epoll_event events[MAX_EVENTS];

    printf("Toll booth running with %d lanes...\n", MAX_LANES);

    /* ---------- Event loop ---------- */
    while (1) {
        //TODO
    }

    close(epfd);
    return 0;
}
