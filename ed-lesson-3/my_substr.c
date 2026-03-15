#include <stdio.h>
#include <string.h>

int substring(const char *line, const char *substr) {
    int line_len = strlen(line);
    int substr_len = strlen(substr);
    for (int i = 0; i < line_len; i++) {
        int temp = i;
        for (int j = 0; j < substr_len; j++) {
            if (line[temp] != substr[j]) {
                break;
            }
            if (j == substr_len - 1) {
                return i;
            }
            temp++;
        }
    }
    return -1;
}

int recursive_comparasion(const char *a, const char *b, int len, int i) {
    if (len == (i + 1)) {
        return a[i] == b[i];
    } else {
        if (a[i] != b[i]) {
            return 0;
        }
        return recursive_comparasion(a, b, len, i + 1);
    }
}

int recursive_substring(const char *line, const char *substr) {
    int substr_len = strlen(substr);
    int line_len = strlen(line);
    for (int i = 0; i < line_len; i++) {
        if (recursive_comparasion(substr, line + i, substr_len, 0)) {
            return i;
        }
    }
    return -1;
}

int main() {
    printf("%d\n", recursive_substring("racecar", "car")); // 4
    printf("%d\n", substring("telephone", "one"));         // 6
    printf("%d\n", substring("monkey", "cat"));            //-1
    return 0;
}
