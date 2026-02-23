#pragma once

#include "task.h"

// task API
void os_task_reset(Task *task);
void os_task_fn_init(TaskFn *fn, TaskHandlerFn handler, void *arg);
void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn);


