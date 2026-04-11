#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main() {
    char buffer[BUFFER_SIZE];
    int seconds = 0;

    // Set stdin to non blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1) {
        perror("ffsake");
    }
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        ssize_t bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);

        // TODO: check the return value of read
        if (bytes <= 0) {
            // TODO: check the value of errno
            // 11 is the error this should print when nonblock and no data
            if (errno != 11) {
                printf("Error! %s, %d", strerror(errno), errno);
                return -1;
            }

            // output and update counter
            seconds++;
            printf("Waiting for %d seconds...\n", seconds);
            sleep(1);
            continue;
        } else if (bytes > 0) {
            buffer[bytes] = '\0';
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "exit") == 0) {
                printf("Exiting...\n");
                break;
            }

            printf("You typed: %s\n", buffer);
            seconds = 0;
        }
    }
    return 0;
}
