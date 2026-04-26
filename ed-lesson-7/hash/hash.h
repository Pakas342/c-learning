#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Type symbol for the hashmap, implementation
 * is defined by the student but must represent a hashmap
 */

struct hashmap_node;

struct hashmap;

typedef size_t (*hash_t)(struct hashmap *, void *);

typedef void (*keydel_t)(void *);

typedef void (*valdel_t)(void *);

typedef bool (*cmp_t)(void *, void *);

/**
 * Initialises your data structure with the following
 * arguments.
 *
 */
struct hashmap *hashmap_init(hash_t, keydel_t, valdel_t, cmp_t, size_t size);

void hashmap_insert(struct hashmap *map, void *key, void *value);
void *hashmap_get(struct hashmap *map, void *key);
void *hashmap_remove_value(struct hashmap *map, void *key);
void *hashmap_remove_entry(struct hashmap *map, void *key);
size_t hashmap_size(struct hashmap *map);
void hashmap_destroy(struct hashmap *map);

#endif
