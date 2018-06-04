#include "tasks/signals.h"

#include "tasks/tasks.h"

void SIGNAL_Send(Task *task, Signal signal)
{
    printf("Task %06X has received signal %X\n", task, signal);
    task->info->signals.received |= signal;
}