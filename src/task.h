#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef struct task Task;
#if defined(__GNUC__)
#define LABEL_CONCAT2(a, b) a ## b
#define LABEL_CONCAT(a, b) LABEL_CONCAT2(a, b)
typedef void* Lc;
#else
typedef unsigned short Lc;
#endif
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
            unsigned char _started : 1;
            unsigned char _suspend : 1;
            unsigned char _system  : 1;
            unsigned char _nc : 6;
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




void os_task_fn_init(TaskFn *fn, TaskHandlerFn handler, void *arg);
void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn);
void task_fn_reset(TaskFn *fn);
void os_task_reset(Task *task);
void task_return(Task *task, void *ret);
void task_fn_call(Task *task, TaskFn *fn, void *arg);
