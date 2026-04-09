// I'll go with the union implementation as I already used the void * one in my
// assignment 1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum TYPE {
    INT,
    CHAR,
    FLOAT,
};

struct node {
    union {
        int int_v;
        char char_v;
        float float_v;
    };
    enum TYPE type;
    struct node *next;
};

/*
list_init
Creates a new list by creating and head node
:: int value :: The value to be stored in the head node
Returns a pointer to the newly created node
*/
struct node *list_init(void *value, enum TYPE type) {
    struct node *head = (struct node *)malloc(sizeof(struct node));
    if (!head) {
        perror("error allocating memory for the head");
        return NULL;
    }
    if (type == INT) {
        head->int_v = *(int *)value;
        head->type = INT;

    } else if (type == CHAR) {
        head->char_v = *(char *)value;
        head->type = CHAR;

    } else if (type == FLOAT) {
        head->float_v = *(float *)value;
        head->type = FLOAT;
    }
    head->next = NULL;
    return head;
}

/*
list_add
Adds a node containing a specified value to an existing list
:: int value :: The value to be stored in the new node
Does not return anything
*/
void list_add(struct node **head, void *value, enum TYPE type) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (!new_node) {
        perror("error allocating memory for a node");
        return;
    }
    new_node->next = NULL;
    if (type == INT) {
        new_node->int_v = *(int *)value;
        new_node->type = INT;

    } else if (type == CHAR) {
        new_node->char_v = *(char *)value;
        new_node->type = CHAR;

    } else if (type == FLOAT) {
        new_node->float_v = *(float *)value;
        new_node->type = FLOAT;
    }
    struct node *current = *head;
    while (current->next) {
        current = current->next;
    }
    current->next = new_node;
    return;
}

/*
list_delete
Removes the specified node from the list and updates the list
accordingly
:: struct node* n :: The pointer to the node to be deleted
Does not return anything
*/
void list_delete(struct node **head, struct node *n) {
    struct node *current = *head;
    if (current == n) {
        *head = n->next;
        free(n);
        return;
    }
    while (current && current->next != n) {
        current = current->next;
    }
    if (current) {
        current->next = n->next;
        free(n);
    }
    return;
}

/*
list_next
Returns a pointer to the next node in the list
:: const struct node* n :: The node
Returns a pointer to the next node
*/
struct node *list_next(const struct node *n) { return n->next; }

/*
list_free
Frees all existing nodes in a list
:: struct node** head :: The pointer to the pointer that is
pointing to the head node of the list
Does not return anything
*/
void list_free(struct node **head) {
    struct node *next_node = *head;
    while (next_node) {
        struct node *temp = next_node->next;
        free(next_node);
        next_node = temp;
    }
    *head = NULL;
}
