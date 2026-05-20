CC     = gcc
CFLAGS = -Wall -std=c11

SRCS = user/shell/shell.c        \
       core/kernel/kernel.c      \
       core/process/process.c    \
       core/scheduler/scheduler.c \
       core/memory/memory.c \
       common/queue.c

TARGET = TOS

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
