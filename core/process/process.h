#ifndef PROCESS_H
#define PROCESS_H

#include "../../core/kernel/kernel.h"

void process_create(const char *name, int total_time,
                    int io_enable, int io_at, int io_wait);
void process_print_all();   // ps 명령

#endif
