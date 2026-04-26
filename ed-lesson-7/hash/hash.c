#include "hash.h"
#include <errno.h>
#include <stdlib.h>

#define DEF_LIMIT_FACTOR (0.75)

struct hashmap_node {
    void *key;
    void *value;
    struct hashmap_node *next;
};

struct hashmap {
    hash_t hash;
    keydel_t keydel;
    valdel_t valdel;
    cmp_t cmp;
    size_t size;
    float limit_factor;
    unsigned elements;
    struct hashmap_node **data;
};

void hashmap_resize(struct hashmap *map, size_t new_size) {
    struct hashmap_node **new_data =
        calloc(new_size, sizeof(struct hashmap_node *));
    if (!new_data) {
        errno = 12;
        return;
    }

    // rehashing all prev nodes into new data
    for (int i = 0; i < map->size; i++) {
        struct hashmap_node *curr_node = map->data[i];
        while (curr_node) {
            size_t hashed = map->hash(map, curr_node->key);
            size_t mod_hash = hashed % new_size;
            struct hashmap_node *new_curr = new_data[mod_hash];
            struct hashmap_node *new_prev = NULL;
            while (new_curr) {
                new_prev = new_curr;
                new_curr = new_curr->next;
            }
            if (new_prev) {
                new_prev->next = curr_node;
            } else {
                new_data[mod_hash] = curr_node;
            }
            struct hashmap_node *old_curr_node = curr_node;
            curr_node = curr_node->next;
            old_curr_node->next = NULL;
        }
    }

    free(map->data);
    map->data = new_data;
    map->size = new_size;
}

struct hashmap *hashmap_init(hash_t hash, keydel_t keydel, valdel_t valdel,
                             cmp_t cmp, size_t size) {
    struct hashmap *map = malloc(sizeof(struct hashmap));
    if (!map) {
        errno = 12;
        return NULL;
    }

    map->keydel = keydel;
    map->valdel = valdel;
    map->cmp = cmp;
    map->size = size;
    map->limit_factor = DEF_LIMIT_FACTOR;
    map->elements = 0;
    map->hash = hash;

    struct hashmap_node **data = calloc(size, sizeof(struct hashmap_node *));
    if (!data) {
        errno = 12;
        free(map);
        return NULL;
    }
    map->data = data;

    return map;
}

void hashmap_insert(struct hashmap *map, void *key, void *value) {
    float current_factor = (float)(map->elements + 1) / (map->size);
    if (current_factor > map->limit_factor) {
        hashmap_resize(map, map->size * 2);
    }
    size_t hashed = map->hash(map, key);
    size_t mod_hash = hashed % map->size;
    struct hashmap_node *curr_node = map->data[mod_hash];
    struct hashmap_node *prev_node = NULL;
    while (curr_node && !map->cmp(key, curr_node->key)) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    if (curr_node) {
        curr_node->value = value;
    } else {
        struct hashmap_node *new_node = calloc(1, sizeof(struct hashmap_node));
        new_node->value = value;
        new_node->key = key;

        if (prev_node) {
            prev_node->next = new_node;
        } else {
            map->data[mod_hash] = new_node;
        }
        map->elements++;
    }
}

void *hashmap_get(struct hashmap *map, void *key) {
    size_t hashed = map->hash(map, key);
    size_t mod_hash = hashed % map->size;
    struct hashmap_node *curr_node = map->data[mod_hash];
    while (curr_node && !map->cmp(key, curr_node->key)) {
        curr_node = curr_node->next;
    }
    if (curr_node) {
        return curr_node->value;
    } else {
        return NULL;
    }
}

void *hashmap_remove_value(struct hashmap *map, void *key) {
    size_t hashed = map->hash(map, key);
    size_t mod_hash = hashed % map->size;
    struct hashmap_node *curr_node = map->data[mod_hash];
    struct hashmap_node *prev_node = NULL;
    while (curr_node && !map->cmp(key, curr_node->key)) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    if (curr_node) {
        struct hashmap_node *next_node = curr_node->next;
        void *value = curr_node->value;
        curr_node->value = NULL;
        return value;
    }
    return NULL;
}

void *hashmap_remove_entry(struct hashmap *map, void *key) {
    size_t hashed = map->hash(map, key);
    size_t mod_hash = hashed % map->size;
    struct hashmap_node *curr_node = map->data[mod_hash];
    struct hashmap_node *prev_node = NULL;
    while (curr_node && !map->cmp(key, curr_node->key)) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    if (curr_node) {
        struct hashmap_node *next_node = curr_node->next;
        void *value = curr_node->value;
        free(curr_node);
        if (prev_node) {
            prev_node->next = next_node;
        } else {
            map->data[mod_hash] = next_node;
        }
        map->elements--;
        return value;
    }
    return NULL;
}

size_t hashmap_size(struct hashmap *map) { return map->elements; };

void hashmap_destroy(struct hashmap *map) {
    for (int i = 0; i < map->size; i++) {
        struct hashmap_node *curr_node = map->data[i];
        while (curr_node) {
            map->keydel(curr_node->key);
            map->valdel(curr_node->value);
            struct hashmap_node *next_node = curr_node->next;
            free(curr_node);
            curr_node = next_node;
        }
    }
    free(map->data);
    free(map);
}
