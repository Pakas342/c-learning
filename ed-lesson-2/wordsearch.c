#include <stdio.h>
#include <string.h>

int recursive_comparison(char a[], char b[], int len, int i);

int main(int argc, char **argv) {
    if (!argv[1]) {
        printf("None word to search given \n");
        return 0;
    }
    char *word = argv[1];

    while (1) {
        char current_line[400] = {'\0'};
        if (fgets(current_line, sizeof(current_line), stdin) == NULL) {
            break;
        }
        for (int i = 0; i < strlen(current_line); i++) {
            if (recursive_comparison(word, current_line + i, strlen(word), 0)) {
                printf("Found: %s", current_line);
                break;
            }
        }
    }
}

int recursive_comparison(char *a, char *b, int len, int i) {
    if (len == (i + 1)) {
        return a[i] == b[i];
    } else {
        if (a[i] != b[i]) {
            return 0;
        }
        return recursive_comparison(a, b, len, i + 1);
    }
}
