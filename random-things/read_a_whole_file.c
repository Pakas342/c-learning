#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "txt.txt"
int main() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("error reading from file");
    }
    // get to the end of the file
    fseek(file, 0, SEEK_END);
    // get the size as ftell returns the current value of the file-position
    // indicator measured in bytes from the biginning of the file
    long file_size = ftell(file);
    // moves file-position indicator to start
    fseek(file, 0, SEEK_SET);

    // allocating enough space in malloc for the file
    // if I wanted to interact with this file as a str. I'll need to +1 the
    // malloc, and define buf[file_size] with the null terminator \0 as per
    // string methods expect
    char *buf = malloc(file_size);

    // reading the file bytes. Size is 1 as per 1 byte
    fread(buf, 1, file_size, file);

    // freeing and closing. ALWAYS
    free(buf);
    fclose(file);
}
