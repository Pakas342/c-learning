#include <stdio.h>

// single quotes give ascii value
#define a_ascii_value 'a' 
#define z_ascii_value 'z'
#define a_A_offset 32

void recursive_shout(char *str) {
    if (*str == '\0') {
        return;
    } else {
        if (*str >= a_ascii_value && *str <= z_ascii_value) {
            printf("%c", *str - a_A_offset);
        } else {
            printf("%c", *str);
        }
        recursive_shout(str + 1);
    }
}

int main() {
    while (1) {
        char txt[100] = {'\0'};
        if (fgets(txt, sizeof(txt), stdin) == NULL) {
            break;
        }
        recursive_shout(txt);
    }
}
