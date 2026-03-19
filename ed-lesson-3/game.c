#include <stdio.h>

#define ITEMS 8
#define STATUSES 4

struct character_state {
    unsigned char inventory;  // to allocate 8 bits,
    unsigned char health : 4; // uses the first 4 bits of a char (1 byte)
    unsigned char status : 4; // uses the last 4 bits of a char (1 byte)
};

// A enum definition doesn't allocate memory, even tho each elements 
// saves an intrinsice value. only after declaring a value with this enum type
// actual memory is allocated
enum ITEM {
    SWORD, // 0
    SHIELD, // 1
    POTION, // 2
    KEY, // 3
    PICKAXE, // 4
    WAND, // 5
    SANDWICH, // 6
    JUICE, // 7
};

enum STATUS {
    IS_POISON,
    IS_BURNING,
    IS_FROZEN,
    IS_CURSED
};

int add_item(struct character_state *state, enum ITEM item) {
    if (item < 0 || item > 7) {
        fprintf(stderr, "Invalid item in item addition\n");
        return 1;
    }
    state->inventory = state->inventory | 1 << item;
    return 0;
}

int apply_status(struct character_state *state, enum STATUS status) {
    if (status < 0 || status > 3) {
        fprintf(stderr, "Invalid status in apply status\n");
        return 1;
    }
    state->status = state->status | (1 << status);
    return 0;
}

int use_potion(struct character_state *state, int potion_power) {
    if ((potion_power + state->health) > 15) {
        state->health = 15;
    } else {
        state->health = potion_power + state->health;
    }
    return 0;
}

int print_status(struct character_state *state) {
    for (int i = 0; i < ITEMS; i++) {
        if ((state->inventory & 1 << i) == 1 << i) {
            printf("Item %d in inventory!\n", i);
        }
    }
    for (int i = 0; i < STATUSES; i++) {
        if ((state->status & 1 << i) == 1 << i) {
            printf("Status %d applied :(\n", i);
        }
    }
    printf("The curent health is: %d\n", state->health);
    return 0;
}

int main() {
    struct character_state character = {
        0}; // we do this to clean the garbage in our struct
    add_item(&character, SWORD);
    add_item(&character, 10);
    apply_status(&character, 10);
    apply_status(&character, IS_BURNING);
    use_potion(&character, 20);
    use_potion(&character, 1);
    print_status(&character);
}
