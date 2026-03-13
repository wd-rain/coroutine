#include "coroutine.h"
#include <stdio.h>

AsyncFn(f3)
{
    coro_var_def_begin();
    static int x;
    coro_var_def_end();

    coro_begin();
    printf("f3 begin");
    coro_var_init_begin();
    x = *(int*)coroArg*10;
    coro_var_init_end();
    
    coro_sleep(3);
    printf("f3 end");
    coro_return(&x);

    coro_end();
}

AsyncFn(f2)
{
    coro_var_def_begin();
    static int x;
    coro_var_def_end();

    coro_begin();
    printf("f2 begin");
    coro_var_init_begin();
    x = *(int*)coroArg*10;
    coro_var_init_end();

    coro_await(&f3, &x);
    x = *(int*)coroRes;
    printf("f2 end");
    coro_return(&x);

    coro_end();
}



AsyncFn(f1)
{
    coro_var_def_begin();
    static int count;
    coro_var_def_end();

    coro_begin();
    coro_var_init_begin();
    count = 0;
    coro_var_init_end();

    printf("f1 begin and count++ = %d", ++count);
    coro_timeout(*(int *)coroArg % 7 == 0, 3);
    if(!coroRight)
    {
        printf("f1 timeout\n");
    }
    printf("f1 yield end");
    coro_await(&f2, &count);
    printf("f1 await end, count = %d", *(int *)coroRes);
    if(count == 3)
    {
        coro_return(&count);
    }

    coro_end();
}

int main(void)
{
    int i = 0;
    Coro task1;

    coro_init(&task1, &f1, &i);
    for (i = 0; i < 100; i++)
    {
        coro_tick_trigger();
        if(task1._return)
        {
            printf("task1 return value = %d", *(int *)task1._return);
            break;
        }
        printf("[%d]:", i);
        coro_run(&task1);
        printf("\n");
    }
}
