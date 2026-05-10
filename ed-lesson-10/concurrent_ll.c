#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

// may need to add synchonisation parts inside struct.
struct node {
    int data;
    struct node* next;
};

struct ll {
    struct node* head;
};

struct ll_iterator {
    struct ll* owner;
    struct node* curr;
};

// create the function declarations.
struct ll* ll_init();
void ll_append(struct ll* list, int value);
void ll_insert(struct ll* list, int index, int value);
void ll_remove(struct ll* list, int index);
// declarations for iterator
struct ll_iterator* ll_iterator_init(struct ll* list);
bool ll_iterator_has_next(struct ll_iterator* iterator);
int ll_iterator_next(struct ll_iterator* iterator);
int ll_iterator_remove(struct ll_iterator* iterator);

// YOUR JOB: apply synchronisation primitives to these.
// If iterator A has already begun and not finished iterating,
// and iterator B tries to call ll_iterator_remove,
// then iterator B would throw an exception (i.e. error message + pthread_exit())
bool ll_iterator_has_next(struct ll_iterator* iterator) {
    return iterator->curr != NULL;
}

int ll_iterator_next(struct ll_iterator* iterator) {
    int value = iterator->curr->data;
    iterator->curr = iterator->curr->next;
    return value;
}
int ll_iterator_remove(struct ll_iterator* iterator) {
    assert(ll_iterator_has_next(iterator));

    struct node* temp = iterator->owner->head;
    struct node* prev = NULL;
    // traverse to node before
    while (temp != NULL && temp != iterator->curr) {
        prev = temp;
        temp = temp->next;
    }
    assert (temp != NULL); // shouldn't happen
    // stich lines
    if (prev == NULL) {
        iterator->owner->head = temp->next;
    } else {
        prev->next = temp->next;
    }
    // update iterator and remove node
    struct node* next_node = iterator->curr->next;
    int value = iterator->curr->data;
    free(iterator->curr);
    iterator->curr = next_node;
    return value;
}

// sample test code.
static struct ll* list;
// thread A: starts traversing the list using iterator.
void* thread_A_worker(void* arg){
    struct ll_iterator* iterator = ll_iterator_init(list);
    int idx = 0;
    while (ll_iterator_has_next(iterator)) {
        if (idx != 1){
            int value = ll_iterator_next(iterator);
            printf("Thread A: %d\n", value);
        } else { // removing from itself should be okay.
            int value = ll_iterator_remove(iterator);
            printf("Thread A remove: %d\n", value);;
        }
        idx++;
        // delay before iterating next
        sleep(1);
    }
    free(iterator);
    return NULL;
}
// thread B: wait for a little while, create iterator, then remove
void* thread_B_worker(void* arg) {
    sleep(2); // wait for thread A to start iterating

    struct ll_iterator* iterator = ll_iterator_init(list);
    ll_iterator_remove(iterator); // ConcurrentModificationException!!
    // thread should be killed. this shouldn't happen.
    assert(false);
    return NULL;
}

int main(){
    list = ll_init();
    ll_append(list, 1);
    ll_append(list, 2);
    ll_append(list, 3);
    ll_append(list, 4);

    // run the threads
    pthread_t threadA, threadB;
    pthread_create(&threadA, NULL, thread_A_worker, NULL);
    pthread_create(&threadB, NULL, thread_B_worker, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    // after this, ll should have 3 elements
    return 0;
}

// IMPLS for list
// list init
struct ll* ll_init() {
    struct ll* list = (struct ll*)malloc(sizeof(struct ll));
    list->head = NULL;
    return list;
}
// list append value
void ll_append(struct ll* list, int value) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        struct node* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}
// list insert value
void ll_insert(struct ll* list, int index, int value) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = value;
    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
        return;
    }
    struct node* temp = list->head;
    for (int i = 0; i < index - 1; i++) {
        if (temp == NULL) {
            assert(false);
        }
        temp = temp->next;
    }
    if (temp == NULL) {
        assert(false);
    }
    new_node->next = temp->next;
    temp->next = new_node;
}
// list remove index
void ll_remove(struct ll* list, int index) {
    if (list->head == NULL) {
        return;
    }
    struct node* temp = list->head;
    if (index == 0) {
        list->head = temp->next;
        free(temp);
        return;
    }
    struct node* prev = NULL;
    for (int i = 0; i < index; i++) {
        if (temp == NULL) {
            assert(false);
        }
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        assert(false);
    }

    prev->next = temp->next;
    free(temp);
}

// IMPLS for iterator
struct ll_iterator* ll_iterator_init(struct ll* list) {
    struct ll_iterator* iterator = (struct ll_iterator*)malloc(sizeof(struct ll_iterator));
    iterator->owner = list;
    iterator->curr = list->head;
    return iterator;
}
