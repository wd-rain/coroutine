# os

以数据与信号为核心的操作系统，尤其适合使用在计算要求低但业务复杂且变动大的场景中



# OS

## API

1. ### os调度器 

```c
/// @brief 运行任务调度
/// @note 需要在循环中执行
void os_run(void);

// 使用方式
int main(int argc, char *argv[])
{
    while(1)
    {
        os_run();
	}
}
```



# Task

## API

### 协程API

1. 协程创建

```c
// 协程函数定义
AsycTask(name)
{
    os_task_var_def_begin();
  	// 此处为协程变量定义的地方
    // 此处的变量都应该是静态变量以保证变量不值不会变化
    os_task_var_def_end();
    
    // 此处为携程的开始标记、协程的所有代码都应该在这个标记后面
    os_task_begin();
    
	// 此处为协程变量初始化处，协程每次被调用会进入次此处
    os_task_var_init_begin();
    count = 0;
    // 协程变量初始化结束标记
    os_task_var_init_end();
    


    // 此处为ie标记、协程的所有代码都应该在这个标记前面
    os_task_end();
}
```



### 基础API

1.  协程的初始

   ```c
   
   /// @brief 协程的初始
   /// @param task 任务句柄
   /// @param init 任务初始化函数
   /// @param deinit 任务去初始化任务
   /// @param fn 任务的起始协程函数
   void os_task_init(Task *task, TaskInitFn init, TaskDeInitFn deinit, TaskFn *fn);
   
   // 使用方式
   int main()
   {
       
   }
   ```

2. 



## 注意事项

- 如果不是GUN环境下，不可以在task内中使用switch的时候使用挂起功能`

  ```c
  AsycTask(task_func)
  {
      os_task_var_def_begin();
      static int a;
      os_task_var_def_end();
      
      os_task_begin();
      os_task_var_init_begin();
      a = *(int *)os_arg;
      os_task_var_init_end();
  	switch(a)
      {
          case 0:
              os_task_yield(); // 在switch中挂起了会导致协程挂起失败，下次进入协程会直接重启协程
              break;
  	}
      os_task_end();
      
  }
  ```

  