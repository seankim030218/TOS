#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../core/kernel/kernel.h"
#include "../../core/process/process.h"
#include "../../core/scheduler/scheduler.h"
#include "../../core/memory/memory.h"

static void cmd_help() {
    printf("Available commands:\n");
    printf("  help\n  exit\n");
    printf("  cd <dir>\n");
    printf("  pwd\n");
    printf("  create <name> <total_time>\n");
    printf("  create <name> <total_time> io <io_at> <io_wait>\n");
    printf("  set_sched fcfs\n");
    printf("  set_sched rr <quantum>\n");
    printf("  set_sched prr <quantum>\n");
    printf("  ps\n  run\n  step\n  stats\n  reset\n");
    printf(" mem\n");
}

static void cmd_cd(char *input) {
    char path[256];
    if (sscanf(input, "cd %s", path) != 1) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(path) != 0)
        perror("cd");
}

static void cmd_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
}

static void cmd_external(char *input) {
    char *args[64];
    int argc = 0;
    char buf[256];
    strncpy(buf, input, sizeof(buf));
    char *token = strtok(buf, " \t");
    while (token && argc < 63) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }
    args[argc] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
    }
}

static void cmd_create(char *input) {
    char name[32]; int total, io_at, io_wait;
    if (sscanf(input, "create %s %d io %d %d", name, &total, &io_at, &io_wait) == 4)
        process_create(name, total, 1, io_at, io_wait);
    else if (sscanf(input, "create %s %d", name, &total) == 2)
        process_create(name, total, 0, 0, 0);
    else
        printf("Usage: create <name> <time> [io <io_at> <io_wait>]\n");
}

static void cmd_set_sched(char *input) {
    char algo[16]; int q = 1;
    sscanf(input, "set_sched %s %d", algo, &q);

    if (strcmp(algo, "fcfs") == 0) {
        kernel.sched_type = SCHED_FCFS;
        printf("Scheduler set: fcfs\n");
    }
    else if (strcmp(algo, "rr") == 0) {
        kernel.sched_type = SCHED_RR;
        kernel.quantum = q;
        printf("Scheduler set: rr (quantum=%d)\n", q);
    }
    else {
        printf("Unknown algorithm: %s\n", algo);
    }
}



static void cmd_stats() {
    const char *sched_name[] = {"FCFS", "RR", "PRR"};
    printf("Scheduling Algorithm: %s\n", sched_name[kernel.sched_type]);
    if (kernel.sched_type != SCHED_FCFS) printf("Quantum: %d\n", kernel.quantum);
    int max_end = 0;
for (int i = 0; i < kernel.proc_count; i++) {
    if (kernel.proc_table[i].end_time > max_end)
        max_end = kernel.proc_table[i].end_time;
}
    for (int i = 0; i < kernel.proc_count; i++) {
        PCB *p = &kernel.proc_table[i];
        printf("%s Start=%d End=%d Waiting=%d\n",
               p->name, p->start_time, p->end_time, p->waiting_time);
    }
    printf("Context Switch Count: %d\n", kernel.context_switch_count);
    printf("Total Context Switch Overhead: %d\n", kernel.context_switch_count * 2);
    printf("Total Completion Time: %d\n", max_end);
}

static int all_terminated() { 
    for (int i = 0; i < kernel.proc_count; i++) 
    if (kernel.proc_table[i].state != TERMINATED) return 0; return 1; } 
    static void cmd_step() { 
        int cs_before = kernel.context_switch_count; do { scheduler_step(); } 
        while (kernel.context_switch_count == cs_before && !all_terminated()); 
        const char *sched_name[] = {"FCFS", "RR", "PRR"}; printf("Algorithm: %s\n", sched_name[kernel.sched_type]); 
        if (kernel.sched_type != SCHED_FCFS) printf("Quantum: %d\n", kernel.quantum); 
        process_print_all(); 
        printf("Context Switches: %d\n", kernel.context_switch_count); 
    }

int main() {
    kernel_init();
    memory_init();
    printf("TOS - Toy Operating System\n");
    char input[256];
    while (1) {
        printf("mini-shell> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        if      (strcmp(input, "help")   == 0) cmd_help();
        else if (strcmp(input, "exit")   == 0) break;
        else if (strcmp(input, "pwd")    == 0) cmd_pwd();
        else if (strcmp(input, "ps")     == 0) process_print_all();
        else if (strcmp(input, "step")   == 0) cmd_step();
        else if (strcmp(input, "stats")  == 0) cmd_stats();
        else if (strcmp(input, "reset") == 0) { kernel_init(); memory_init(); printf("Reset done.\n"); }
        else if (strcmp(input, "run")    == 0) {
            scheduler_run();
            process_print_all();
            printf("Context Switches: %d\n", kernel.context_switch_count);
            printf("Total Overhead: %d\n",   kernel.context_switch_count * 2);
        }
        else if (strncmp(input, "cd",        2) == 0) cmd_cd(input);
        else if (strncmp(input, "create",    6) == 0) cmd_create(input);
        else if (strncmp(input, "set_sched", 9) == 0) cmd_set_sched(input);
        else if (strcmp(input, "mem") == 0) memory_print();
        else cmd_external(input);
    }
    return 0;
}
