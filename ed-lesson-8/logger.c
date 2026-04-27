#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough args!");
    }

    char *command_start = memchr(argv[1], '=', 10);

    FILE *f = popen(command_start + 1, "r");

    // fseek to know how big the file is doesn't work as pipes can't be seek.
    // Pretty dumb then to use a file descriptor with them, but meh

    char *buf = NULL;
    unsigned buf_size = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        buf_size++;
        buf = realloc(buf, buf_size);
        if (!buf) {
            return -1;
        }
        buf[buf_size-1] = c;
    }

    pclose(f);

    char *output_start = memchr(argv[2], '=', 10);

    mode_t mode = 0666; // rw for all
    // open a file descriptor so we can either overwrite o create the output
    // file
    int fd = open(output_start + 1, O_WRONLY | O_CREAT | O_TRUNC, mode);

    write(fd, buf, buf_size);
    close(fd);
    free(buf);
}
