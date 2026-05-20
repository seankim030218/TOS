#ifndef MEMORY_H
#define MEMORY_H

#include "../../common/types.h"

#define TOTAL_MEMORY   8192
#define DEFAULT_MEM    1024

typedef struct {
    int  pid;
    char name[32];
    int  size;
    char filepath[64];
    int  in_use;
} MemBlock;

void memory_init();
void memory_alloc(int pid, const char *name);
void memory_free(int pid);
void memory_print();
int  memory_get_usage();

#endif