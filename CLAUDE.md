# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
# Build
make

# Clean build artifacts
make clean

# Run (must be run from the TOS/ directory — memory files write to runtime/ relative to CWD)
./TOS
```

The `runtime/` directory must exist before running, as `memory_alloc` writes `runtime/mem_<name>.bin` files for each process.

## Shell Commands

Once inside the TOS shell (`mini-shell>`):

| Command | Description |
|---|---|
| `create <name> <time>` | Create a process with given CPU burst time |
| `create <name> <time> io <io_at> <io_wait>` | Create a process with an I/O burst |
| `set_sched fcfs` | Set FCFS scheduling |
| `set_sched rr <quantum>` | Set Round Robin with given quantum |
| `ps` | Show process states (running, ready, waiting, terminated) |
| `run` | Run all processes to completion |
| `step` | Advance one logical step (until context switch or completion) |
| `stats` | Print per-process stats and context switch count |
| `mem` | Print memory allocation table |
| `reset` | Re-initialize kernel and memory state |

## Architecture

The codebase simulates a simple OS with a tick-based scheduler. All modules share a single global `Kernel` struct (defined in `core/kernel/kernel.c`, declared `extern` in `core/kernel/kernel.h`).

### Data Flow

```
shell.c (main + command dispatch)
    ├── process_create()   → enqueues PCB into kernel.ready_q
    ├── scheduler_step()   → advances one simulation tick
    │       ├── update_waiting()  → ages wait_q, moves ready processes back to ready_q
    │       ├── preemption check  → handles quantum expiry (RR), I/O trigger, termination
    │       ├── pick_next()       → dequeues from ready_q
    │       └── memory_free()     → called on TERMINATED
    └── memory_alloc/free  → creates/removes runtime/mem_<name>.bin files
```

### Key Structs

- **`PCB`** (`common/types.h`): Process control block — tracks pid, state, timing fields (`arrival_time`, `start_time`, `end_time`, `remaining_time`, `executed_time`, `waiting_time`), I/O parameters.
- **`Kernel`** (`core/kernel/kernel.h`): Global singleton holding `proc_table[64]`, `ready_q`, `wait_q`, `running` pointer, `current_time`, `sched_type`, `quantum`, `context_switch_count`.
- **`Queue`** (`common/queue.h`): Fixed-size circular queue of `PCB*`, used for `ready_q` and `wait_q`.

### Scheduling Logic (`core/scheduler/scheduler.c`)

`scheduler_step()` runs one simulation tick in order:
1. Decrement `io_remaining` for all waiting processes; move finished I/O back to `ready_q`.
2. Check running process: trigger I/O if `executed_time == io_at`, terminate if `remaining_time <= 0`, preempt if quantum exhausted (RR/PRR).
3. If no running process, call `pick_next()` (simple dequeue — FCFS order) and context-switch.
4. Increment `executed_time`, `remaining_time--`, `quantum_used++` for the running process.
5. Increment `waiting_time` for all READY processes.

`SCHED_PRR` is defined in `SchedType` but not yet wired up in `cmd_set_sched` — it falls through to the "unknown" branch.

### Memory Simulation (`core/memory/memory.c`)

Each process gets a flat 1024 KB allocation represented as a file at `runtime/mem_<name>.bin`. There is no actual virtual address space — the file is metadata only. `memory_free` deletes the file; `memory_init` (called on `reset`) deletes all in-use files.
