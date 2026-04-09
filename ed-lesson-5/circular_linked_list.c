#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
    struct node *prev;
};

/*
list_init
Creates a new list by creating and head node
:: int value :: The value to be stored in the head node
Returns a pointer to the newly created node
*/
struct node *list_init(int value) {
    struct node *head = (struct node *)malloc(sizeof(struct node));
    if (!head) {
        perror("error allocating memory for the head");
        return NULL;
    }
    head->value = value;
    head->next = head;
    head->prev = head;
    return head;
}

/*
list_add
Adds a node containing a specified value to an existing list
:: int value :: The value to be stored in the new node
Does not return anything
*/
void list_add(struct node **head, int value) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (!new_node) {
        perror("error allocating memory for a node");
        return;
    }
    new_node->next = *head;
    new_node->value = value;
    struct node *current = (*head)->prev;
    current->next = new_node;
    new_node->prev = current;
    (*head)->prev = new_node;
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
    if (n->next == n) {
        free(n);
        *head = NULL;
        return;
    }
    struct node *current = *head;
    if (current == n) {
        *head = n->next;
        n->prev->next = n->next;
        n->next->prev = n->prev;
        free(n);
        return;
    }
    while (current->next != n && current->next != *head) {
        current = current->next;
    }
    if (current->next == n) {
        current->next = n->next;
        n->next->prev = current;
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
    while (next_node->next != *head) {
        struct node *temp = next_node->next;
        free(next_node);
        next_node = temp;
    }
    free(next_node);
    *head = NULL;
}
