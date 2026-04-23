#ifndef FILE_SYSTEM_H 
#define FILE_SYSTEM_H

#include <stdbool.h>

struct file {
    char name[100];
    bool directory;
};


struct stack {
    unsigned capacity;
    unsigned top_index;
    void **data;
};

#endif
