#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEF_STR_BUF (100)

/*
 * ADD - Addition
 * SUB - Subtraction
 * MUL - Multiple
 * DIV - Divide
 */

enum FUNC {
    ADD,
    SUB,
    MUL,
    DIV,
};

typedef int (*math_fn_t)(int, int);

struct instruction {
    math_fn_t fn;
    char arg1[DEF_STR_BUF];
    char arg2[DEF_STR_BUF];
};

union line_index {
    unsigned line_index;
    struct instruction *instruction;
};

int add(int a, int b) { return (a + b); }
int sub(int a, int b) { return (a - b); }
int mul(int a, int b) { return (a * b); }
int divi(int a, int b) { return (a / b); }

math_fn_t a = add;
math_fn_t b = sub;
math_fn_t c = mul;
math_fn_t d = divi;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char message[] = "No file of instructions given\n";
        write(STDOUT_FILENO, message, sizeof(message));
        exit(0);
    }
    math_fn_t functions[] = {a, b, c, d};
    int fd = open(argv[1], O_RDONLY);
    off_t file_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *file_buf = malloc(file_len * sizeof(char));
    if (!file_buf) {
        errno = 12;
        return -1;
    }

    read(fd, file_buf, file_len);
    close(fd);
    union line_index *lines_index = NULL;
    unsigned instructions = 0;
    for (int i = 0; i < file_len; i++) {
        if (file_buf[i] == '\n' || file_buf[i] == 0) {
            instructions++;
            lines_index =
                realloc(lines_index, instructions * sizeof(union line_index));
            if (!lines_index) {
                errno = 12;
                return -1;
            }
            lines_index[instructions - 1].line_index = i;
        }
    }

    unsigned prev = 0;
    struct instruction *prev_int = NULL;
    for (int i = 0; i < instructions; i++) {
        unsigned next = lines_index[i].line_index;
        char instruct_buf[DEF_STR_BUF];
        strncpy(instruct_buf, file_buf + prev, (next - prev));
        instruct_buf[next-prev] = '\0';
        char ins[DEF_STR_BUF];
        char arg1[DEF_STR_BUF];
        char arg2[DEF_STR_BUF];
        sscanf(instruct_buf, "%s %s %s", ins, arg1, arg2);
        enum FUNC func;
        if (strcmp(ins, "ADD") == 0)
            func = ADD;
        if (strcmp(ins, "SUB") == 0)
            func = SUB;
        if (strcmp(ins, "MUL") == 0)
            func = MUL;
        if (strcmp(ins, "DIV") == 0)
            func = DIV;

        struct instruction *cur_instruction;
        cur_instruction = malloc(sizeof(struct instruction));
        if (!cur_instruction) {
            return -1;
        }

        *cur_instruction = (struct instruction){.fn = functions[func]};
        strcpy(cur_instruction->arg1, arg1);
        strcpy(cur_instruction->arg2, arg2);
        lines_index[i].instruction = cur_instruction;
        prev = next;
    }
    close(fd);
    free(file_buf);

    int prev_value = 0;
    for (int i = 0; i < instructions; i++) {
        struct instruction *cur_ins = (lines_index[i].instruction);
        int arg1;
        int arg2;
        if (cur_ins->arg1[0] == '%') {
            arg1 = prev_value;
        } else {
            arg1 = atoi(cur_ins->arg1);
        }

        if (cur_ins->arg2[0] == '%') {
            arg2 = prev_value;
        } else {
            arg2 = atoi(cur_ins->arg2);
        }

        prev_value = cur_ins->fn(arg1, arg2);
        free(cur_ins);
    }

    char end[DEF_STR_BUF];
    sprintf(end, "The result is: %d\n", prev_value);
    write(STDOUT_FILENO, end, strlen(end));
    free(lines_index);
}
