#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// EOF will never be given to a console, fuck that.

#define CLI_BUF (5)

void end(char *line, char **argsv, int argsc) {
    free(line);
    for (int i = 0; i < argsc; i++) {
        free(argsv[i]);
    }
    free(argsv);
}

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

        while ((buf_len == CLI_BUF && !memchr(stdin_buf, '\n', buf_len))) {
            memcpy(line + (line_len - buf_len), stdin_buf, buf_len);
            buf_len = read(STDIN_FILENO, stdin_buf, CLI_BUF);
            if (buf_len <= 0) {
                break;
            }
            line_len += buf_len;
            line = realloc(line, (line_len) * sizeof(char));
            if (!line) {
                return -1;
            }
        }

        // yaaay, we got our line!
        memcpy(line + (line_len - buf_len), stdin_buf, buf_len);

        // we got the line! now let's parse args dynamically
        char **args = NULL;
        int in_word = 0;
        int words = 0;
        int word_start = 0;
        for (int i = 0; i < line_len; i++) {
            if (line[i] == ' ') {
                if (in_word) {
                    words++;
                    char *word = malloc((i - word_start + 1) * sizeof(char));
                    if (!word) {
                        return -1;
                    }
                    memcpy(word, line + word_start, i - word_start);
                    word[i - word_start] = '\0';
                    args = realloc(args, sizeof(char *) * words);
                    if (!args) {
                        return -1;
                    }
                    args[words - 1] = word;
                }
                in_word = 0;
            } else {
                if (!in_word) {
                    in_word = 1;
                    word_start = i;
                }
            }
        }

        if (in_word) {
            words++;
            // we don't need the extra space because last word ends in \n
            char *word = malloc((line_len - word_start) * sizeof(char));
            if (!word) {
                return -1;
            }
            memcpy(word, line + word_start, line_len - word_start);
            word[line_len - word_start - 1] = '\0';
            args = realloc(args, sizeof(char *) * words);
            if (!args) {
                return -1;
            }
            args[words - 1] = word;
        }

        args = realloc(args, sizeof(char *) * (words + 1));
        args[words] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            end(line, args, words);
            exit(0);
        }

        // dangerous as sscanf can overflow, but command is just the first word
        // sscanf(line, "%s", command);

        pid_t pid = fork();

        if (pid < 0) {
            char message[] = "fork failed";
            write(STDOUT_FILENO, message, sizeof(message));
            continue;
        } else if (pid > 0) {
            wait(NULL);
        } else {
            execvp(args[0], args);
        }

        end(line, args, words);
    }
}
