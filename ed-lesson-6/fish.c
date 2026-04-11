#include <stdio.h>
#include <stdlib.h>

struct fish {
    char *name;
    float length;
    int tastiness;
};

#define NUM_FISHES 10

int fish_compar(const void *fish_1, const void *fish_2) {
    struct fish *fish_1_c = (struct fish *)fish_1;
    struct fish *fish_2_c = (struct fish *)fish_2;
    if (fish_1_c->length > fish_2_c->length) {
        return 1;
    } else if (fish_1_c->length == fish_2_c->length) {
        if (fish_1_c->tastiness > fish_2_c->tastiness) {
            return 1;
        } else if (fish_1_c->tastiness == fish_2_c->tastiness) {
            if (fish_1_c->name[0] > fish_2_c->name[0]) {
                return 1;
            } else if (fish_1_c->name[0] == fish_2_c->name[0]) {
                return 0;
            }
        }
    }
    return -1;
}

int main(void) {
    struct fish fishes[NUM_FISHES] = {
        {.name = "Salmon", .length = 70.5, .tastiness = 10},
        {.name = "Tuna", .length = 120.0, .tastiness = 9},
        {.name = "Sardine", .length = 15.2, .tastiness = 4},
        {.name = "Cod", .length = 70.5, .tastiness = 6},
        {.name = "Carp", .length = 50.0, .tastiness = 2},
        {.name = "Snapper", .length = 40.0, .tastiness = 8},
        {.name = "Bass", .length = 40.0, .tastiness = 7},
        {.name = "Marlin", .length = 120.0, .tastiness = 9},
        {.name = "Guppy", .length = 3.5, .tastiness = 1},
        {.name = "Trout", .length = 50.0, .tastiness = 2}};

    qsort(fishes, NUM_FISHES, sizeof(struct fish), fish_compar);

    for (int i = 0; i < NUM_FISHES; i++) {
        printf("%-7s | %-5.1f | %d\n", fishes[i].name, fishes[i].length,
               fishes[i].tastiness);
    }

    return 0;
}
