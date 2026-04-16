#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ship {
    char *name;
    int fuel;
    float distance;
    struct ship *next;
};

// todo: create predicate functions here
int fuel_predicate(struct ship *ship, int fuel) {
    if (ship->fuel < fuel) {
        return 0;
    } else
        return 1;
}

int distance_predicate(struct ship *ship, int distance) {
    if (ship->distance < distance) {
        return 0;
    } else
        return 1;
}

struct ship *filter_ships(struct ship *head, int (predicate)(struct ship*, int)) {
    // TODO
    return // todo ;
}

void print_ships(struct ship *head) {
    while (head) {
        printf("%s %d %f\n", head->name, head->fuel, head->distance);
        head = head->next;
    }
    printf("\n");
}

int main(void) {
    struct ship s1 = {"Falcon", 50, 12.0, NULL};
    struct ship s2 = {"Voyager", 10, 900.0, &s1};
    struct ship s3 = {"Enterprise", 85, 120.5, &s2};
    struct ship s4 = {"Apollo", 15, 4.2, &s3};
    struct ship *fleet = &s4;

    // filter out ships that have less than 50 fuel
    fleet = filter_ships(/*todo*/);
    print_ships(fleet);

    // filter out ships that are less than 100 distance
    fleet = filter_ships(/*todo*/);
    print_ships(fleet);

    return 0;
}
