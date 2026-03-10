#include <stdio.h>

int main() {
    while (1) {
        char str[100] = {'\0'};
        if (fgets(str, sizeof(str), stdin) == NULL) {
            break;
        }

        int array_size = 0;

        // the simpler approach will be to use strlen (I forgot about it haha)
        for (char *str_point = str; *str_point; str_point++) {
            array_size++;
        }

        if (str[array_size-1] == '\n') {
            array_size--;
        }

        for (int i = (array_size - 1); i >= 0; i--) {
            printf("%c", str[i]);
        }
        printf("\n");
    }
}
