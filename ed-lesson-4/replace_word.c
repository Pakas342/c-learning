#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum word_relation { bigger, smaller, equal };

int main(int argc, char **argv) {
    if (argc != 5) {
        perror("Wrong amount of params given");
        return -1;
    }
    FILE *org_file_ptr = fopen(argv[1], "r");
    FILE *new_file_ptr = fopen(argv[2], "w");
    if (org_file_ptr == NULL || new_file_ptr == NULL) {
        if (org_file_ptr)
            fclose(org_file_ptr);
        else
            fclose(new_file_ptr);
        perror("error reading files");
        return -1;
    }
    char *data = NULL;
    size_t n;
    if (getdelim(&data, &n, EOF, org_file_ptr) < 0) {
        perror("error reading original file");
    };

    int diff = strlen(argv[3]) - strlen(argv[4]);
    enum word_relation relation;
    if (diff == 0) {
        relation = equal;
    } else if (diff < 0) {
        relation = smaller;
    } else {
        relation = bigger;
    }

    // id fine that it gets recalculated everytime, as data will change inside
    // the loop
    for (int i = 0; i < strlen(data); i++) {
        if (strncmp(&data[i], argv[3], strlen(argv[3])) == 0) {
            switch (relation) {
            case equal: {
                for (int j = 0; j < strlen(argv[3]); j++) {
                    data[i + j] = argv[4][j];
                }
                break;
            }
            case smaller: {
                // as smaller, diff will be negative, and this is
                // allocating MORE space
                char *second_str = malloc(strlen(data) - diff);
                if (second_str == NULL) {
                    perror("aaaaaaaaaah");
                }
                data = realloc(data, strlen(data) - diff + 1);
                data[i] = 0;
                strcpy(second_str, argv[4]);
                strcat(second_str, &data[i + strlen(argv[3])]);
                data = strcat(data, second_str);
                free(second_str);
                break;
            }
            case bigger: {
                char *second_str = malloc(strlen(data) - diff);
                if (second_str == NULL) {
                    perror("aaaaaaaaaah");
                }
                data[i] = 0;
                strcpy(second_str, argv[4]);
                strcat(second_str, &data[i + strlen(argv[3])]);
                data = strcat(data, second_str);
                data = realloc(data, strlen(data));
                free(second_str);
                break;
            }
            }
        }
    }
    fwrite(data, strlen(data), 1, new_file_ptr);
    fclose(org_file_ptr);
    fclose(new_file_ptr);
    free(data);
}
