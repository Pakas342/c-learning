#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define INITIAL_CAPACITY 5

struct dyn_array {
    size_t capacity;
    size_t size;
    int contents[];
};

/*
dyn_array_init
Creates a new dynamic array with a default allocation
*/
struct dyn_array *dyn_array_init(int value) {
    struct dyn_array *array =
        malloc(sizeof(struct dyn_array) + (sizeof(int) * INITIAL_CAPACITY));
    if (!array) {
        perror("Error allocating memory for array");
        return NULL;
    }
    array->capacity = INITIAL_CAPACITY;
    *(array->contents) =
        value; // doing (array->contents)[0] will have a similar effect here
    array->size = 1;
    return array;
}

/*
dyn_array_add
Adds an element to the list
:: struct dyn_array* dyn :: The pointer to the dynamic array
Does not return anything
*/
void dyn_array_add(struct dyn_array **dyn_ptr, int value) {
    struct dyn_array *dyn = *dyn_ptr;
    dyn->size++;
    if (dyn->size == dyn->capacity) {
        // duplicating the capacity
        *dyn_ptr =
            realloc(dyn, sizeof(*dyn) + (sizeof(int) * dyn->capacity * 2));
        dyn = *dyn_ptr;
        if (!dyn) {
            perror("Realloc failed :(");
            return;
        }
        dyn->capacity += dyn->capacity;
    }
    dyn->contents[dyn->size - 1] = value;
}

/*
dyn_array_delete
Removes an element from the list and updates the list accordingly
:: struct dyn_array* :: The pointer to the dynamic array
:: int index :: index of element to remove
Does not return anything
*/
void dyn_array_delete(struct dyn_array *dyn, int index) {
    for (size_t i = index; i < dyn->size - 1; i++) {
        dyn->contents[i] = dyn->contents[i + 1];
    }
    dyn->size--;
}

/*
dyn_array_get
:: struct dyn_array* dyn :: The dynamic array
:: int index :: the index at the specific array
Return the int at the specified index
*/
// THE NULL RETURN IS WRONG, but the signature should allow me to write the
// value in an address, and use the return to return an error. NULL return is
// returning 0 as if it was a valid value
int dyn_array_get(struct dyn_array *dyn, int index) {
    if (index >= dyn->size) {
        perror("out of bounds index");
        return NULL;
    }
    return dyn->contents[index];
}

/*
dyn_array_free
Frees the current array allocation
:: struct dyn_array* dyn :: The dynamic array
Does not return anything
*/
void dyn_array_free(struct dyn_array *dyn) { free(dyn); }
