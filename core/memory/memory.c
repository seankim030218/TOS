#include "memory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEM_BLOCKS 64

static MemBlock mem_table[MAX_MEM_BLOCKS];
static int      mem_count = 0;

void memory_init() {
    for (int i = 0; i < mem_count; i++)
        if (mem_table[i].in_use)
            remove(mem_table[i].filepath);
    memset(mem_table, 0, sizeof(mem_table));
    mem_count = 0;
}

void memory_alloc(int pid, const char *name) {
    if (mem_count >= MAX_MEM_BLOCKS) {
        printf("Memory table full!\n");
        return;
    }
    MemBlock *b = &mem_table[mem_count++];
    b->pid    = pid;
    b->size   = DEFAULT_MEM;
    b->in_use = 1;
    strncpy(b->name, name, 31);
    snprintf(b->filepath, sizeof(b->filepath), "runtime/mem_%s.bin", name);

    FILE *f = fopen(b->filepath, "wb");
    if (f) {
        fprintf(f, "PID=%d NAME=%s SIZE=%dKB\n", pid, name, b->size);
        fclose(f);
        printf("Memory allocated: %dKB -> %s\n", b->size, b->filepath);
    }
}

void memory_free(int pid) {
    for (int i = 0; i < mem_count; i++) {
        if (mem_table[i].pid == pid && mem_table[i].in_use) {
            remove(mem_table[i].filepath);
            mem_table[i].in_use = 0;
            printf("Memory freed: %s\n", mem_table[i].filepath);
            return;
        }
    }
}

int memory_get_usage() {
    int total = 0;
    for (int i = 0; i < mem_count; i++)
        if (mem_table[i].in_use)
            total += mem_table[i].size;
    return total;
}

void memory_print() {
    printf("Memory Usage:\n");
    printf("%-6s %-8s %-8s %s\n", "PID", "Name", "Size", "File");
    printf("---------------------------------------------\n");
    int found = 0;
    for (int i = 0; i < mem_count; i++) {
        if (mem_table[i].in_use) {
            printf("%-6d %-8s %-6dKB  %s\n",
                   mem_table[i].pid,
                   mem_table[i].name,
                   mem_table[i].size,
                   mem_table[i].filepath);
            found = 1;
        }
    }
    if (!found) printf("(empty)\n");
    printf("---------------------------------------------\n");
    printf("Total: %dKB / %dKB used\n", memory_get_usage(), TOTAL_MEMORY);
}
