#pragma once

#include "task.h"

// task API
void os_task_reset(Task *task);
void os_task_fn_init(TaskFn *fn, TaskHandlerFn handler, void *arg);
void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn);
static inline void os_task_run(Task *task) {  if (task->_fn)  task->_fn->_handler(task); }
static inline void os_task_resume(Task *task) { if(task->_fn) task->_fn->_state.flag._suspend = 0; }
#define AsycTask(name)              void name##_fn (Task *task); TaskFn name = {0, name##_fn, 0, 0, {{0}}, 0}; void name##_fn (Task *task)
#define os_task_var_def_begin();    
#define os_task_var_def_end();
#define os_task_var_init_begin();   if(task->_fn->_state.flag._started == 0) {
#define os_task_var_init_end();     task->_fn->_state.flag._started = 1; }
#define os_task_begin();            switch (task->_fn->_pc) { case 0:
#define os_task_end();              default: break; } task->_fn->_pc = 0;
#define os_task_yield()             task->_fn->_pc = __LINE__; task->_fn->_state.flag._suspend = 1; case __LINE__: if(task->_fn->_state.flag._suspend) return
#define os_task_return(ret)         task_return(task, ret); return
#define os_task_await(fn, arg)      task->_fn->_pc = __LINE__; task_fn_call(task, fn, arg);  return; case __LINE__:
#define os_res                      (task->_fn->_return)
#define os_arg                      (task->_fn->_arg)

