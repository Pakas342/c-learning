#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
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
    head->next = NULL;
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
    new_node->next = NULL;
    new_node->value = value;
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

/*
 * Kth last node
 * Retrieves the k-th last node from a linked list.
 */
struct node *kth_last(struct node **head, size_t k) {
    struct node *slow = *head;
    struct node *fast = *head;
    for (size_t i = 0; i < k; i++) {
        fast = fast->next;
        if (!fast) {
            return NULL;
        }
    }
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

int main() {
    struct node *head = list_init(1);
    list_add(&head, 2);
    list_add(&head, 3);
    list_add(&head, 4);
    struct node *n = kth_last(&head, 1);
    printf("%d\n", n->value);
    list_free(&head);
}
