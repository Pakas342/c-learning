#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// THIS IS BRITLE WITH EOF, but is a way of reading from stdin and adding that
// to a dynamic string. Not using VLAs

#define CLI_BUF (5)
#define STA_STR_BUF (500)

int main() {
    while (1) {
        char stdin_buf[CLI_BUF + 1] = {'\0'};
        char *line = NULL;

        size_t buf_len = read(STDIN_FILENO, stdin_buf, CLI_BUF);
        if (buf_len <= 0) {
            write(STDOUT_FILENO, "\n", 1);
            continue;
        }
        size_t line_len = buf_len;
        line = realloc(line, (line_len) * sizeof(char));
        if (!line) {
            return -1;
        }

        while (buf_len == CLI_BUF && !memchr(stdin_buf, '\n', buf_len)) {
            memcpy(line + (line_len - buf_len), stdin_buf, buf_len);
            buf_len = read(STDIN_FILENO, stdin_buf, CLI_BUF);
            line_len += buf_len;
            line = realloc(line, (line_len) * sizeof(char));
            if (!line) {
                return -1;
            }
        }
        memcpy(line + (line_len - buf_len), stdin_buf, buf_len);

        // if I want to pass args too, I will have to check spaces in line I'm
        // kind of exhausted, so fuck that haha

        char command[STA_STR_BUF];
        sscanf(line, "%s", command);
        char *args[] = {NULL};

        pid_t pid = fork();

        if (pid < 0) {
            char message[] = "fork failed";
            write(STDOUT_FILENO, message, sizeof(message));
            continue;
        } else if (pid > 0) {
            wait(NULL);
        } else {
            execvp(command, args);
        }
    }
}
