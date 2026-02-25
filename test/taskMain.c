#include "coroutine.h"
#include <stdio.h>

AsyncFn(f3)
{
    cror_var_def_begin();
    static int x;
    cror_var_def_end();

    cror_begin();
    printf("f3 begin");
    cror_var_init_begin();
    x = *(int*)crorArg*10;
    cror_var_init_end();
    
    cror_sleep(3);
    printf("f3 end");
    cror_return(&x);

    cror_end();
}

AsyncFn(f2)
{
    cror_var_def_begin();
    static int x;
    cror_var_def_end();

    cror_begin();
    printf("f2 begin");
    cror_var_init_begin();
    x = *(int*)crorArg*10;
    cror_var_init_end();

    cror_await(&f3, &x);
    x = *(int*)crorRes;
    printf("f2 end");
    cror_return(&x);

    cror_end();
}



AsyncFn(f1)
{
    cror_var_def_begin();
    static int count;
    cror_var_def_end();

    cror_begin();
    cror_var_init_begin();
    count = 0;
    cror_var_init_end();

    printf("f1 begin and count++ = %d", ++count);
    cror_timeout(*(int *)crorArg % 7 == 0, 3);
    if(!crorRight)
    {
        printf("f1 timeout\n");
    }
    printf("f1 yield end");
    cror_await(&f2, &count);
    printf("f1 await end, count = %d", *(int *)crorRes);
    if(count == 3)
    {
        cror_return(&count);
    }

    cror_end();
}

int main(void)
{
    int i = 0;
    Cror task1;

    cror_init(&task1, &f1, &i);
    for (i = 0; i < 100; i++)
    {
        cror_tick_trigger();
        if(task1._return)
        {
            printf("task1 return value = %d", *(int *)task1._return);
            break;
        }
        printf("[%d]:", i);
        cror_run(&task1);
        printf("\n");
    }
}
