#include <stdio.h>

int main(int argc, char **argv) {
    char userName[10];
    printf("\nWhat is your name? ");
    fgets(userName, sizeof(userName), stdin);
    printf("Yo! %s", userName);
}
