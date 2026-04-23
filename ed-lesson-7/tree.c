// Notes on trees!
//
// Even tho they beg for recurssion, in C a recursion could overflow the stack,
// as the stack is limited (8mb in linux for all processes). that means when a
// unbalanced and very deep tree, recursion can bite you in the ass. If you have
// that concern in your mind, you could potentially use a iterative approach
// using a stack DS

#include "tree.h"
#include <errno.h>
#include <stdlib.h>

#define INITIAL_CAPACITY (4)

struct tree_node *create_node(void *value) {
    struct tree_node *new_node = malloc(sizeof(struct tree_node));
    if (!new_node) {
        errno = 12;
        return NULL;
    }

    // Allocating a children array with a capacity of 4. We save the pointers!
    struct tree_node **children_nodes =
        malloc(sizeof(struct tree_node *) * INITIAL_CAPACITY);
    if (!children_nodes) {
        errno = 12;
        return NULL;
    }
    // this is wrong, as I'm creating this var in the stack, this will get lost
    // on retun. I have to do it directly on the allocated memory in my heap:
    // struct children_list children_list = (struct children_list){
    //     .capacity = INITIAL_CAPACITY, .used = 0, .values = children_nodes};

    // allocated on the heap memory directly
    *new_node = (struct tree_node){
        .value = value,
        .children = (struct children_list){
            .capacity = INITIAL_CAPACITY, .used = 0, .values = children_nodes}};
    return new_node;
}

void add_child(struct tree_node *parent, struct tree_node *child) {
    // expanding the array if necessary
    if (parent->children.used >= parent->children.capacity - 1) {
        struct tree_node **new_values =
            realloc(parent->children.values,
                    sizeof(struct tree_node *) * parent->children.capacity * 2);
        if (!new_values) {
            errno = 12;
            return;
        } else {
            parent->children.values = new_values;
        }
        parent->children.capacity = parent->children.capacity * 2;
    }

    unsigned index = parent->children.used;

    // adding the child pointer to the list of values in parent
    parent->children.values[index] = child;
    parent->children.used++;
}

void dfs(struct tree_node *root, node_action_t action) {

    // generic action to do in each node while dfsing
    action(root);

    for (int i = 0; i < root->children.used; i++) {
        dfs(root->children.values[i], action);
    }
}

void destroy_tree(struct tree_node *root, destroy_value_t destroy) {
    for (int i = 0; i < root->children.used; i++) {
        destroy_tree(root->children.values[i], destroy);
    }
    free(root->children.values);
    destroy(root->value);
    free(root);
}

void remove_child(struct tree_node *parent, struct tree_node *child_rm,
                  destroy_value_t destroy) {
    for (int i = 0; i < parent->children.used; i++) {
        struct tree_node *child = parent->children.values[i];
        if (child == child_rm) {
            destroy_tree(child_rm, destroy);
            for (int j = i; j < parent->children.used; j++) {
                parent->children.values[j] = parent->children.values[j + 1];
            }
            parent->children.used--;
            return;
        }
    }
}
