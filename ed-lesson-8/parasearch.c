#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enough args");
        return -1;
    }

    FILE *children[argc - 2];

    for (int i = 2; i < argc; i++) {
        char command[200] = {0};
        strcpy(command, "grep ");
        strcat(command, argv[1]);
        strcat(command, " ");
        strcat(command, argv[i]);
        FILE *f = popen(command, "r");
        children[i - 2] = f;
    }

    for (int i = 2; i < argc; i++) {
        FILE *f = children[i - 2];
        int c = fgetc(f);
        if (c != EOF) {
            printf("%s found\n", argv[i]);
        } else {
            printf("%s not found\n", argv[i]);
        }
        pclose(f);
    }
    printf("process complete\n");
}
