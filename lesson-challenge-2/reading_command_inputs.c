#include <stdio.h>

int main(int argc, char **argv) {
    printf("\n");

    // initializing at 1 as the first string in the array is the command itself, and we don't want to print it
    for (int i = 1; i < argc; i++) {
        if (i == argc - 1) {
            printf("%s", argv[i]);
        }
        else {
        printf("%s ", argv[i]);
        }
    }

    printf("\n");
}
