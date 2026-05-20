#include "kernel.h"
#include <string.h>

Kernel kernel;

void kernel_init() {
    memset(&kernel, 0, sizeof(Kernel));
    queue_init(&kernel.ready_q);
    queue_init(&kernel.wait_q);
    kernel.running  = NULL;
    kernel.sched_type = SCHED_FCFS;
    kernel.quantum  = 1;
}
