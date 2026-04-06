#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *str = NULL;
    size_t n = 0;
    if (argc == 1) {
        getdelim(&str, &n, EOF, stdin);
        printf("%s", str);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE *file_ptr = fopen(argv[i], "r");
            if (file_ptr == NULL) {
                perror("Reading file error");
            }
            getdelim(&str, &n, EOF, file_ptr);
            printf("%s", str);
            fclose(file_ptr);
        }
    }

    free(str);
}
