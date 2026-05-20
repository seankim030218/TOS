#include "process.h"
#include "../../core/memory/memory.h"
#include <stdio.h>
#include <string.h>

static int next_pid = 1;

void process_create(const char *name, int total_time,
                    int io_enable, int io_at, int io_wait) {
    if (kernel.proc_count >= MAX_PROCS) {
        printf("Process table full!\n");
        return;
    }
    PCB *p = &kernel.proc_table[kernel.proc_count++];
    memset(p, 0, sizeof(PCB));

    p->pid             = next_pid++;
    p->os_pid          = -1;
    strncpy(p->name, name, 31);
    p->state           = READY;
    p->arrival_time    = kernel.current_time;
    p->start_time      = -1;
    p->total_work_time = total_time;
    p->remaining_time  = total_time;
    p->io_enable       = io_enable;
    p->io_at           = io_at;
    p->io_wait_time    = io_wait;
    p->io_remaining    = 0;
    p->priority        = 0;

    enqueue(&kernel.ready_q, p);
    printf("Process %s created (pid=%d)\n", p->name, p->pid);

    memory_alloc(p->pid, p->name);
}

void process_print_all() {
    printf("Running: ");
    if (kernel.running) printf("%s\n", kernel.running->name);
    else printf("None\n");

    printf("Ready Queue:\n");
    queue_print(&kernel.ready_q);

    printf("Waiting Queue:\n");
    queue_print(&kernel.wait_q);

    printf("Terminated:\n");
    int found = 0;
    for (int i = 0; i < kernel.proc_count; i++) {
        if (kernel.proc_table[i].state == TERMINATED) {
            printf("%s ", kernel.proc_table[i].name);
            found = 1;
        }
    }
    if (!found) printf("Empty");
    printf("\n");
}
