#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define min_ascii '!'
#define max_ascii '~'

int main(int argc, char **argv) {
    int lenght = atoi(argv[1]);
    int valid_interval = max_ascii - min_ascii;
    srand(time(NULL));

    for (int i=0; i<lenght; i++){
         char random_char  = rand() % valid_interval + 1 + min_ascii;
        printf("%c", random_char);
    }
    printf("\n");
}
