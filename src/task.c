#include "task.h"

void task_fn_reset(TaskFn *fn)
{
    fn->_pc = 0;
    fn->_return = NULL;
    fn->_state.u8 = 0;
    fn->_prev = NULL;
 }

void os_task_reset(Task *task)
{
    task->_return = NULL;
    task->_fn = task->_initFn;
    task_fn_reset(task->_fn);
    
}

void task_return(Task *task, void *ret)
{
    if(task->_fn->_prev)
    {
        TaskFn *fn = task->_fn;
        task->_fn = task->_fn->_prev;
        task->_fn->_return = ret;
        task->_fn->_state.flag._suspend = 0;
        task_fn_reset(fn);
    }
    else
    {
        task_fn_reset(task->_fn);
        task->_fn = NULL;
        task->_return = ret;
    }
}


void os_task_fn_init(TaskFn *fn, TaskHandlerFn handler, void *arg)
{
    fn->_handler = handler;
    fn->_arg = arg;
    task_fn_reset(fn);
}

void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn)
{
    task->_init = init;
    task->_deinit = deinit;
    task->_initFn = fn;
    os_task_reset(task);
}

void task_fn_call(Task *task, TaskFn *fn, void *arg)
{
    task_fn_reset(fn);
    fn->_arg = arg;
    fn->_prev = task->_fn;
    task->_fn = fn;
}