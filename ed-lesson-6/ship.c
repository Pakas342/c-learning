#include <stdio.h>
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

struct ship *filter_ships(struct ship *head, int(predicate)(struct ship *, int),
                          int predicate_arg) {
    struct ship *current_ship = head;
    struct ship *prev = NULL;
    struct ship *current_head = head;
    while (current_ship) {
        if (!predicate(current_ship, predicate_arg)) {
            if (prev) {
                prev->next = current_ship->next;
            } else {
                current_head= current_ship->next;
                current_ship = current_ship->next;
                continue;
            }
        }
        prev = current_ship;
        current_ship = current_ship->next;
    }

    return  current_head;
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
    fleet = filter_ships(fleet, &fuel_predicate, 50);
    print_ships(fleet);

    // filter out ships that are less than 100 distance
    fleet = filter_ships(fleet, &distance_predicate, 100);
    print_ships(fleet);

    return 0;
}
