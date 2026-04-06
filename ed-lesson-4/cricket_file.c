#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cricketer {
    char *first_name;
    char *last_name;
    int score;
};

#define FILE_PATH "cricket.csv"

int main() {
    struct cricketer ranking[11] = {0};
    for (int i = 0; i < 11; i++) {
        printf("\nEnter Name and Score for batter %d:", i + 1);
        char fname[100] = {0};
        char lname[100] = {0};
        int score;
        // these are already pointers as per decay
        while (scanf("%s %s %d", fname, lname, &score) != 3)
            ;
        struct cricketer cricketer = {.score = score};
        // We're doint this because cricketer only saves a pointer to the
        // string, not the string itseld, so we need to allocate some memory for
        // it, and then copy the temp string into that memory
        cricketer.first_name = malloc(sizeof(fname));
        cricketer.last_name = malloc(sizeof(lname));
        strcpy(cricketer.first_name, fname);
        strcpy(cricketer.last_name, lname);
        ranking[i] = cricketer;
    }
    printf("\n");

    FILE *cricket_file = fopen(FILE_PATH, "w");
    if (cricket_file == NULL) {
        perror("Failed to open target file\n");
        return -1;
    }

    fwrite("fname,lname,score\n", 1, strlen("fname,lname,score\n"),
           cricket_file);

    char duck[] = "🦆";
    for (int j = 0; j < 11; j++) {
        struct cricketer batter = ranking[j];
        if (batter.score < 1) {
            printf("%d. %c. %s: %s\n", j + 1, batter.first_name[0],
                   batter.last_name, duck);
        } else {
            printf("%d. %c. %s: %d\n", j + 1, batter.first_name[0],
                   batter.last_name, batter.score);
        }

        char *str;
        if (asprintf(&str, "%s,%s,%d\n", batter.first_name, batter.last_name,
                     batter.score) <= 0) {
            perror("This is an error hehhehe");
            return -1;
        }

        fwrite(str, 1, strlen(str), cricket_file);

        // Fundamental!! we need to free memory after finishing with it
        free(str);
        free(batter.first_name);
        free(batter.last_name);
    }
    fclose(cricket_file);
}
