#include <stdio.h>
int main() {
    int i;
    printf("I'm a pointer as an int!: %ld", (long)&i);
    printf("I'm a pointer as an str!: %s", (char *)&i);
}
