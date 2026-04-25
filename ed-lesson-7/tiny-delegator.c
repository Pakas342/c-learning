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

typedef int (*math_fn_t)(int, int);

struct func_map {
    char name[DEF_STR_BUF];
    math_fn_t fn;
};

struct instruction {
    char fn[DEF_STR_BUF];
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char message[] = "No file of instructions given\n";
        write(STDOUT_FILENO, message, sizeof(message));
        exit(0);
    }
    struct func_map functions[] = {{.name = "ADD", .fn = add},
                                   {.name = "SUB", .fn = sub},
                                   {.name = "MUL", .fn = mul},
                                   {.name = "DIV", .fn = divi}};
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
    for (int i = 0; i < instructions; i++) {
        unsigned next = lines_index[i].line_index;
        char instruct_buf[DEF_STR_BUF];
        strncpy(instruct_buf, file_buf + prev, (next - prev));
        instruct_buf[next - prev] = '\0';
        char ins[DEF_STR_BUF];
        char arg1[DEF_STR_BUF];
        char arg2[DEF_STR_BUF];
        sscanf(instruct_buf, "%s %s %s", ins, arg1, arg2);

        struct instruction *cur_instruction;
        cur_instruction = malloc(sizeof(struct instruction));
        if (!cur_instruction) {
            return -1;
        }

        strcpy(cur_instruction->arg1, arg1);
        strcpy(cur_instruction->arg2, arg2);
        strcpy(cur_instruction->fn, ins);
        lines_index[i].instruction = cur_instruction;
        prev = next + 1;
    }
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

        for (int j = 0; j < 4; j++) {
            if (strcmp(functions[j].name, cur_ins->fn) == 0) {
                prev_value = functions[j].fn(arg1, arg2);
            }
        }
        free(cur_ins);
    }

    char end[DEF_STR_BUF];
    sprintf(end, "The result is: %d\n", prev_value);
    write(STDOUT_FILENO, end, strlen(end));
    free(lines_index);
}
