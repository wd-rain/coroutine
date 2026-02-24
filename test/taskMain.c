#include "../src/os.h"
#include "stdio.h"

AsycTask(task3_handler)
{
    os_task_var_def_begin();
    static int a;
    os_task_var_def_end();

    os_task_begin();

    printf("task3: call\n");
    os_task_var_init_begin();
    a = *(int *)os_arg;
    os_task_var_init_end();


    a = a * 10;
    printf("task3: %d\n", a);
    os_task_return(&a);

    os_task_end();

}

AsycTask(task2_handler)
{
    os_task_var_def_begin();
    static int a;
    os_task_var_def_end();

    os_task_begin();

    os_task_var_init_begin();
    a = *(int *)os_arg;
    os_task_var_init_end();

    a = a * 10;
    printf("task2: call");
    os_task_await(&task3_handler, &a);
    a = *(int *)os_res;

    printf("task2: %d", a);
    os_task_return(&a);

    os_task_end();
}

AsycTask(task1_handler)
{
    os_task_var_def_begin();
    static int count;
    os_task_var_def_end();

    os_task_begin();

    os_task_var_init_begin();
    count = 0;
    os_task_var_init_end();

    printf("call task1");
    os_task_yield();
    count++;
    if(count == 3)
    {
        printf("count = 3");
        os_task_await(&task2_handler, &count);
        printf("c=%d task3(task2(count)) = %d", count, *(int *)os_res);
    }
    
    if (count == 10)
    {
        os_task_return(&count);
    }

    os_task_end();
}

int main(int argc, char *argv[])
{
    Task task1;

    os_task_init(&task1, NULL, NULL, &task1_handler);

    for (int i = 0; i < 100; i++)
    {
         if (i % 5 == 0)
        {
            os_task_resume(&task1);
        }
        if (task1._return)
        {
            printf("task1 return: %d\n", *(int *)task1._return);
            task1._return = NULL;
        }
        printf("[%d]:", i);
        os_task_run(&task1);
        printf("\n");
    }
    return 0;
}
