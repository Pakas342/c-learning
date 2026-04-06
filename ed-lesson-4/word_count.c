#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char *data = NULL;
    size_t n = 0;
    for (int i = 1; (i < argc || i == 1); i++) {
        if (argc == 1) {
            // only works because I'm reading from stdin until a '\0' is given,
            // but shouldn't be use to read an actual file as that might cause
            // issues
            getdelim(&data, &n, '\0', stdin);
        } else {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "not file found at %s\n", argv[i]);
                return -1;
            }
            fseek(file, 0, SEEK_END);
            n = ftell(file);
            fseek(file, 0, SEEK_SET);
            data = malloc(n);
            fread(data, n, 1, file);
        }

        unsigned bytes = 0;
        unsigned word = 0;
        unsigned lines = 0;
        bool in_word = false;
        for (int j = 0; j < n; j++) {
            if (data[j] == '\n') {
                lines++;
                if (in_word) {
                    in_word = false;
                }
            } else if (isspace(data[j])) {
                if (in_word) {
                    in_word = false;
                }
            } else {
                if (!in_word) {
                    word++;
                    in_word = true;
                }
            }
            bytes++;
        }

        printf("%u %u %u", lines, word, bytes);
        if (argc == 1) {
            printf("\n");
        } else {
            printf(" %s\n", argv[i]);
        }

        free(data);
    }
}
