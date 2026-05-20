#ifndef TYPES_H
#define TYPES_H

#include <sys/types.h>

// ── 프로세스 상태 ──────────────────────────
typedef enum {
    NEW, READY, RUNNING, WAITING, TERMINATED
} ProcessState;

// ── PCB 구조체 ────────────────────────────
typedef struct {
    int   pid;
    pid_t os_pid;
    char  name[32];
    char  command[128];
    char *argv[16];
    ProcessState state;

    int arrival_time;
    int start_time;
    int end_time;
    int total_work_time;
    int remaining_time;
    int executed_time;
    int waiting_time;
    int priority;

    int io_enable;
    int io_at;
    int io_wait_time;
    int io_remaining;
} PCB;

// ── 스케줄링 알고리즘 타입 ─────────────────
typedef enum {
    SCHED_FCFS,
    SCHED_RR,
    SCHED_PRR
} SchedType;

#define MAX_PROCS 64

#endif
