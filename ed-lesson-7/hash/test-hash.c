#include "hash.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

size_t djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c = *str;

    // Null on null terminator
    while (c) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        str++;
        c = *str;
    }

    return hash;
}

size_t string_hash(struct hashmap *map, void *key) {
    return djb2((unsigned char *)key);
}

void string_keydel(void *key) {
    free(key);
    return;
}

void int_valdel(void *key) { return; }

bool string_comp(void *str1, void *str2) {
    int equal = strcmp(str1, str2);
    if (equal == 0) {
        return true;
    } else
        return false;
}

int main() {
    struct hashmap *my_map =
        hashmap_init(string_hash, string_keydel, int_valdel, string_comp, 8);

    char *one = malloc(sizeof(char) * 10);
    strcpy(one, "one");
    int one_val = 1;
    char *two = malloc(sizeof(char) * 10);
    strcpy(two, "two");
    int two_val = 2;
    char *three = malloc(sizeof(char) * 10);
    strcpy(three, "three");
    int three_val = 3;
    char *four = malloc(sizeof(char) * 10);
    strcpy(four, "four");
    int four_val = 4;

    hashmap_insert(my_map, one, &one_val);
    hashmap_insert(my_map, two, &two_val);
    hashmap_insert(my_map, three, &three_val);
    hashmap_insert(my_map, four, &four_val);

    int *search_one = hashmap_get(my_map, one);
    assert(search_one == &one_val);

    int *test_tree = hashmap_remove_value(my_map, three);
    assert(test_tree == &three_val);
    int *search_three = hashmap_get(my_map, three);
    assert(search_three == NULL);

    hashmap_remove_entry(my_map, four);
    int *search_four = hashmap_get(my_map, four);
    assert(search_four == NULL);
    free(four);

    assert(hashmap_size(my_map) == 8);

    hashmap_destroy(my_map);
}
