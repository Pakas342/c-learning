#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *str = malloc(sizeof(char));
    if (!str) {
        return -1;
    }
    long str_len = 0;
    if (argc == 1) {
        char c = 0;
        // VERY non optimal. system call per character hehe
        // A better approach will be to read on chunks, and realloc duplicating
        // the size of malloc each time instead of just making it 1 char bigger
        while (read(STDIN_FILENO, &c, 1) > 0) {
            if (c == 0) {
                break;
            }
            str_len++;
            // +1 for null terminator
            str = realloc(str, (sizeof(char) * str_len) + 1);
            if (!str) {
                return -1;
            }
            str[str_len - 1] = c;
        }
        str[str_len] = 0;
        write(STDOUT_FILENO, str, str_len);
    } else if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            str_len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            str = realloc(str, (sizeof(char) * str_len) + 1);
            if (!str) {
                return -1;
            }
            read(fd, str, str_len);
            close(fd);
            write(STDOUT_FILENO, str, str_len);
            // str[str_len] = 0; not required as read does that for me
        }
    }
    free(str);
}
