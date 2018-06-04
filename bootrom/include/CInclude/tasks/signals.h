#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdint.h>

typedef struct Task Task; // forward ref

typedef uint32_t signals_t; //32-bit flags

typedef enum {
    SIG_ABORT   = 1, // Task should clean up and exit.
    SIG_MESSAGE = 2, // A message is waiting in the queue.
    SIG_KILL    = 4, // Task should exit immediately.
    SIG_BUS     = 8 // Task has caused a bus error and must terminate.
    // TODO: more signals...
} SIGNAL_Signal; // System-defined signals.
typedef SIGNAL_Signal Signal;

// SIGNAL_Block gets attached to a Task structure.
// It describes the signals that a task is waiting for and the signals a task has received.
typedef struct {
    uint32_t received; // Signals that a task has received.
    uint32_t waiting;  // Signals that the task is waiting for. The task is asleep until one is received.
} SIGNAL_Block;

void SIGNAL_Send(Task *task, Signal signal);

#endif