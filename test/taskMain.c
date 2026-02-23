#include "../src/os.h"
#include "stdio.h"



void task1_handler(Task *task)
{
    // 变量的使用
    switch (task->_fn->_pc)
    {
    case 0:
        printf("start");
        task->_fn->_pc = __LINE__; task->_fn->_state.flag._suspend = 1; case __LINE__: if(task->_fn->_state.flag._suspend) return;
        printf("end");
    default:
        break;
    }
    // task_return(task, NULL);
    task->_fn->_pc = 0;
}

int main(int argc, char *argv[])
{
    Task task1;
    TaskFn fn1;
    os_task_fn_init(&fn1, task1_handler, NULL);
    os_task_init(&task1, NULL, NULL, &fn1);

    for (int i = 0; i < 100; i++)
    {
        if(i == 50)
        {
            task1._fn->_state.flag._suspend = 0;
        }
        printf("[%d]:", i);
        os_task_run(&task1);
        printf("\n");
    }
    return 0;
}
