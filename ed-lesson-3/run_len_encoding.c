#include <stdio.h>

int encode_run(const char *sequence, unsigned len, char *buf,
               unsigned int buf_len) {
    int pos = 0;
    for (int i = 0; i < len; i++) {
        if (sequence[i] == '\0') {
            continue;
        }
        if (i == 0) {
            buf[pos] = '1';
            continue;
        }
        if (sequence[i] == sequence[i - 1]) {
            buf[pos] = buf[pos] + 1;
            continue;
        } else {
            pos++;
            buf[pos] = '1';
            continue;
        }
    }
    buf[pos + 1] = '\0';
    return 0;
}

int main() {
    char encoded_run[128];
    const char *line_run = "1122333334423";

    encode_run(line_run, 14, encoded_run, 128);
    printf("%s\n", encoded_run); // 225211
}
