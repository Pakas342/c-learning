#include <stdio.h>
#include <string.h>

struct tuple {
    char value;
    int times;
};

int encode_run(const char *sequence, unsigned len, char *buf,
               unsigned int buf_len) {
    int pos = 0;
    struct tuple tuples[len];
    for (int i = 0; i < len; i++) {
        if (sequence[i] == '\0') {
            continue;
        }
        if (i == 0) {
            tuples[pos] = (struct tuple){.times = 1, .value = sequence[i]};
            continue;
        }
        if (sequence[i] == sequence[i - 1]) {
            tuples[pos].times++;
            continue;
        } else {
            pos++;
            tuples[pos] = (struct tuple){.times = 1, .value = sequence[i]};
            continue;
        }
    }
    pos++;
    for (int i = 0; i < pos; i++) {
        struct tuple tuple = tuples[i];
        char tuple_str[buf_len];
        sprintf(tuple_str, "(%c,%d)", tuple.value, tuple.times);
        buf = strcat(buf, tuple_str);
    }
    return 0;
}

int decode_run(char *encoded, char *decoded) {
    char *token = strtok(encoded, "(");
    while (token != NULL) {
        char character;
        int times;
        sscanf(token, "%c,%d)", &character, &times);
        char temp_str[times];
        for (int i = 0; i < times; i++) {
            temp_str[i] = character;
        }
        temp_str[times] = 0;
        strcat(decoded, temp_str);
        token = strtok(NULL, "(");
    }
    return 0;
}

int main() {
    char encoded_run[128] = {'\0'};
    char decoded_run[128] = {'\0'};
    const char *line_run = "1122333334423";

    encode_run(line_run, 14, encoded_run, 128);
    // (1,2) (2,2) (3,5) (4,2) (2,1) (3,1)
    printf("%s\n", encoded_run);
    decode_run(encoded_run, decoded_run);
    printf("%s\n", decoded_run);
}
