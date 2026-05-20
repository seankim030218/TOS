#ifndef KERNEL_H
#define KERNEL_H

#include "../../common/types.h"
#include "../../common/queue.h"

typedef struct {
    PCB      proc_table[MAX_PROCS]; // 전체 프로세스 목록
    int      proc_count;            // 생성된 프로세스 수
    PCB     *running;               // 현재 실행 중인 프로세스
    Queue    ready_q;               // 레디 큐
    Queue    wait_q;                // 웨이팅 큐
    int      current_time;          // 시뮬레이션 현재 시각
    int      context_switch_count;  // 컨텍스트 스위치 횟수
    SchedType sched_type;
    int      quantum;               // RR용 타임 퀀텀
    int      current_quantum_used;  // 현재 프로세스가 쓴 퀀텀
} Kernel;

extern Kernel kernel;   // 전역으로 사용

void kernel_init();

#endif
