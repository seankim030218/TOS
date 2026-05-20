#include "scheduler.h"
#include "../../core/memory/memory.h"
#include <stdio.h>

static void update_waiting() {
    Queue tmp; queue_init(&tmp);
    while (!queue_empty(&kernel.wait_q)) {
        PCB *p = dequeue(&kernel.wait_q);
        p->io_remaining--;
        p->remaining_time--;
        if (p->io_remaining <= 0) {
            p->state = READY;
            enqueue(&kernel.ready_q, p);
        } else {
            enqueue(&tmp, p);
        }
    }
    kernel.wait_q = tmp;
}

static PCB *pick_next() {
    if (queue_empty(&kernel.ready_q)) return NULL;
    return dequeue(&kernel.ready_q);
}

void scheduler_step() {
    update_waiting();

    PCB *cur = kernel.running;

    if (cur) {
        if (cur->io_enable && cur->executed_time == cur->io_at) {
            cur->state        = WAITING;
            cur->io_remaining = cur->io_wait_time;
            enqueue(&kernel.wait_q, cur);
            kernel.running    = NULL;
            cur = NULL;
        }
        else if (cur->remaining_time <= 0) {
            cur->state    = TERMINATED;
            cur->end_time = kernel.current_time;
            memory_free(cur->pid);
            kernel.running = NULL;
            cur = NULL;
        }
        else if ((kernel.sched_type == SCHED_RR || kernel.sched_type == SCHED_PRR)
                 && kernel.current_quantum_used >= kernel.quantum) {
            cur->state = READY;
            enqueue(&kernel.ready_q, cur);
            kernel.running              = NULL;
            kernel.current_quantum_used = 0;
            cur = NULL;
        }
    }

    if (!kernel.running) {
        PCB *next = pick_next();
        if (next) {
            kernel.running = next;
            next->state    = RUNNING;
            if (next->start_time == -1)
                next->start_time = kernel.current_time;
            kernel.current_quantum_used = 0;
            kernel.context_switch_count++;
        }
    }

    if (kernel.running) {
        kernel.running->executed_time++;
        kernel.running->remaining_time--;
        kernel.current_quantum_used++;
    }

    for (int i = 0; i < kernel.proc_count; i++) {
        PCB *p = &kernel.proc_table[i];
        if (p->state == READY) p->waiting_time++;
    }

    kernel.current_time++;
}

void scheduler_run() {
    int max_iter = 10000;
    while (max_iter--) {
        int done = 1;
        for (int i = 0; i < kernel.proc_count; i++)
            if (kernel.proc_table[i].state != TERMINATED) { done = 0; break; }
        if (done) break;
        scheduler_step();
    }
}
