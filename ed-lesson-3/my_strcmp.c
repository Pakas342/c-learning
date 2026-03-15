#include <stdio.h>

int string_compare(const char *w1, const char *w2, unsigned int len) {
    for (int i = 0; i < len; i++) {
        if (w1[i] != w2[i]) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int result;
    result = string_compare("Hello", "Hello", 5);
    printf("%d\n", result);
    result = string_compare("Hello", "byebye", 5);
    printf("%d\n", result);
    result = string_compare("Hello?", "Hello?", 5);
    printf("%d\n", result);
    result = string_compare("Hello? someone there", "Hello? someone there", 10);
    printf("%d\n", result);
    result = string_compare("Hello?", "Hello?", 5);
    printf("%d\n", result);
}
