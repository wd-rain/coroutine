#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef struct task Task;
typedef unsigned short Lc;
typedef void (*TaskHandlerFn)(Task *task);
typedef void (*TaskInitFn)(void);
typedef void (*TaskDeInitFn)(void);

typedef struct task_fn
{
    Lc _pc;
    TaskHandlerFn _handler;
    void *_return;
    void *_arg;
    union
    {
        struct
        {
            unsigned char _ended : 1;
            unsigned char _started : 1;
            unsigned char _suspend : 1;
        } flag;
        unsigned char u8;
    } _state;

    struct task_fn *_prev;
} TaskFn;

typedef struct task
{
    void *_return;
    TaskInitFn _init;
    TaskDeInitFn _deinit;
    TaskFn *_initFn;
    TaskFn *_fn;
} Task;


static inline void task_start(Task *task)
{
}


void os_task_fn_init(TaskFn *fn, TaskHandlerFn handler, void *arg);
void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn);
void task_fn_reset(TaskFn *fn);
void os_task_reset(Task *task);
void task_return(Task *task, void *ret);
void task_fn_call(Task *task, TaskFn *fn, void *arg);