# coroutine
---

一个模仿JavaScript协程的协程库，适用于中低端的嵌入式场景，适合使用在逻辑环境下，每个协程仅需要48个字节的栈、采用静态方式创建利于内存安全

## 功能特性
---

- 多种挂起方式：延迟、条件、超时
- 调用栈的实现
- 支持协程函数调用协程函数
- 支持协程的关闭、开启、复位

## 使用注意事项
---

- 协程不支持递归的写法，无法调用自己
- 由于每个协程仅有一个栈空间、因此每个协程仅能被一个线程调用
- 在非GNU平台下，无法在协程中的swtich内使用任何形式的挂起

## 项目结构
---

```
coroutine/
├── src/				# 源文件代码
│	├── coroutine.h		# 主头文件 
│	└── coroutine.c		# 主源文件
├──	example/			# 示例目录
│	└── taskMain.c		# 简易示例
├── makefile			# 构建脚本
└── README.md             # 说明文档
```

## API 参考
---

### 协程函数内使用API

`cror_return(ret)` 

**功能**：结束当前协程函数且返回参数

**参数**：

- `ret`: 返回值的指针



`cror_yield(judge)` 

**功能**：挂起协程直到 judge == 1的时候继续协程

**参数**：

- `judge `: 判读表达式



`cror_await(fn, arg)` 

**功能**：挂起协程直到 fn协程函数完成

**参数**：

- `fn `: 协程函数
- `arg`:传入协程函数的参数

**注意**：

- `crorArg `: 协程内传入的参数指针

- `crorRes `: 协程内调通函数返回的值

  

`cror_sleep(tick)` 



`cror_sleep(tick)` 

**功能**：挂起协程tick时间

**参数**：

- `tick `: 挂起的滴答时间



`cror_timeout(judge, tick)` 

**功能**：挂起协程直到 judge 判断或者挂起时间超过tick

**参数**：

- `judge `: 判读表达式

- `tick `: 挂起的滴答时间

**注意**：

- `crorRight `: 协程内是否超时 1：表示judge判断成功 0：表示失败




## 快速开始
---

1.  添加源文件到工程中

2.  在需要执行的协程的文件中添加coroutine.h

3. 添加获取tick的函数

   ```c
   // 以HAL库为例
   int main()
   {
       cror_tick_init(HAL_GetTick);
   }
   ```

   或者在定时器中断中加入时钟触发函数

   ```c
   void SysTick_Handler(void)
   {
       cror_tick_trigger()
   }
   ```

4. 创建协程函数，以下为协程函数模版

   ```c
   // 协程函数定义
   AsyncFn(fooName)
   {
       cror_var_def_begin();
       // 此处为协程变量定义的地方
       // 此处的变量都应该是静态变量以保证变量不值不会变化
       // eg : static int x;
       cror_var_def_end();
   
       // 此处为携程的开始标记、协程的所有代码都应该在这个标记后面
       cror_begin();
       cror_var_init_begin();
       // 此处为协程初始化处，协程每次被调用会进入次此处
       // eg : x = 0;
       cror_var_init_end();
   
       // 协程结束返回（如果没有此函数、协程函一直循环）
       cror_return(&x);
       
   	// 此处为协程标记、协程的所有代码都应该在这个标记前面
       cror_end();
   }
   ```

5. 创建且初始化协程、且在循环中调用协程

   ```c
   Cror cror1;
     
   int main()
   {
       cror_init(&cror1, &fooName, &arg);
       
       for(;;)
       {
           cror_run(&cror1);
   	}
   }
   ```

## 兼容性
---

- C99 标准
- 适用于各种微控制器平台 (STM32, Arduino, ESP32, etc.)
- 支持裸机和 RTOS 环境
- 内存占用小，适合资源受限的系统