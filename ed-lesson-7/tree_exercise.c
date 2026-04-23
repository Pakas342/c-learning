// Notes on trees!
//
// Even tho they beg for recurssion, in C a recursion could overflow the stack, as the stack is limited (8mb in linux for all processes). that means when a unbalanced and very deep tree, recursion can bite you in the ass. If you have that concern in your mind, you could potentially use a iterative approach using a stack DS

#include "tree_exercise.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY (4)

struct tree_node *create_node(int value) {
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

void dfs(struct tree_node *root) {

    printf("I'm at the node with value: %c\n", root->value);

    for (int i = 0; i < root->children.used; i++) {
        dfs(root->children.values[i]);
    }
}

void destroy_tree(struct tree_node *root) {
    for (int i = 0; i < root->children.used; i++) {
        destroy_tree(root->children.values[i]);
    }
    free(root->children.values);
    free(root);
}

int main(int argc, char **argv) {
    // we'll be using chars, which is fine as we said we'll be saving ints, and
    // chars are smaller than ints, so it's just a underused int. It will be
    // problematic the other way around, because and int (usually 4) doesn't fit
    // in the bytes given to a char (usually 1)
    //
    // I should be checking errno, but I'm too lazy (bad bad bad)
    struct tree_node *A = create_node('A');
    struct tree_node *B = create_node('B');
    struct tree_node *C = create_node('C');
    struct tree_node *D = create_node('D');
    struct tree_node *E = create_node('E');
    struct tree_node *F = create_node('F');

    add_child(A,B);
    add_child(A,C);
    add_child(A,D);
    add_child(B,E);
    add_child(C,F);

    dfs(A);

    destroy_tree(A);
}

