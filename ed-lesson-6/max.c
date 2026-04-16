#include <stdio.h>
#include <string.h>

struct item {
    char *name;
    int count;
};

int int_value(const void *a) { return *(int *)a; }

int string_length_value(const void *a) { return strlen(*(char **)a); }

int struct_count_value(const void *a) { return ((struct item *)a)->count; }

void *max(void *base, size_t n, size_t size, int (*value)(const void *)) {
    void *current_max = base;
    int current_val = value(base);
    int max_value = current_val;
    for (int i = 1; i < n; i++) {
        // base is a pointer to the first element, but
        // is currently interpreted as a 1 byte void,
        // that's why we have to size*n and not only +n
        current_val = value(base + (size * i));
        if (current_val > max_value) {
            current_max = base + (size * i);
            max_value = current_val;
        }
    }
    return current_max;
}

int main() {
    int nums[] = {10, 55, 23, 9, 42};
    int *max_num = max(nums, 5, sizeof(int), int_value);
    printf("%d\n", *max_num);

    char *words[] = {"apple", "hippopotamus", "cat", "banana"};
    char **max_word = max(words, 4, sizeof(char *), string_length_value);
    printf("%s\n", *max_word);

    struct item items[] = {{"apple", 1}, {"banana", 500}, {"cat", 15}};
    struct item *max_item =
        max(items, 3, sizeof(struct item), struct_count_value);
    printf("%s %d\n", max_item->name, max_item->count);

    return 0;
}
