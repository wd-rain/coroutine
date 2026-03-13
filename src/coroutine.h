#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef struct coro Coro;
#if defined(__GNUC__)
#define LABEL_CONCAT2(a, b) a ## b
#define LABEL_CONCAT(a, b) LABEL_CONCAT2(a, b)
typedef void* Lc;
#else
typedef unsigned short Lc;
#endif
typedef void (*CoroHandlerFn)(Coro *task);
typedef void (*CoroInitFn)(void);
typedef void (*CoroDeInitFn)(void);
typedef unsigned int CoroTick;
typedef CoroTick (*CoroTickGet)(void);

typedef struct coro_fn
{
    Lc _pc;
    CoroHandlerFn _handler;
    void *_return;
    void *_arg;
    union
    {
        struct
        {
            unsigned char _started : 1;
            unsigned char _righted : 1;
            unsigned char _nc : 7;
        } flag;
        unsigned char u8;
    } _state;
    CoroTick _tick;
    struct coro_fn *_prev;
} CoroFn;

typedef struct coro
{
    void *_return;
    CoroFn *_initFn;
    CoroFn *_fn;
} Coro;

void _coro_fn_call(Coro *task, CoroFn *fn, void *arg);
void _coro_return(Coro *task, void *ret);
extern CoroTickGet _coroGetTick;
void coro_tick_trigger(void);
void coro_tick_init(CoroTickGet getTick);
void coro_reset(Coro *task);
void coro_close(Coro *task);
void coro_open(Coro *task);
void coro_return(Coro *task, void *ret);
void coro_init(Coro *task, CoroFn *fn, void *arg);
static inline void coro_run(Coro *task) {  if (task->_fn)  task->_fn->_handler(task); }
#define AsyncFn(name)              void name##_fn (Coro *task); CoroFn name = {0, name##_fn, 0, 0, {{0}}, 0}; void name##_fn (Coro *task)
#define coro_var_def_begin();    
#define coro_var_def_end();
#define coro_var_init_begin();   if(task->_fn->_state.flag._started == 0) {
#define coro_var_init_end();     task->_fn->_state.flag._started = 1; }
#define coro_return(ret)         _coro_return(task, ret); return
#define coroRes                  (task->_fn->_return)
#define coroArg                  (task->_fn->_arg)
#define coroRight                (task->_fn->_state.flag._righted)
#if defined(__GNUC__)
#define coro_begin()             do { if(task->_fn->_pc) goto *task->_fn->_pc; }while(0)
#define coro_end();              task->_fn->_pc = 0;  
#define coro_yield(why)          task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__);  LABEL_CONCAT(L, __LINE__): if(!(why)) return
#define coro_await(fn, arg)      task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); _coro_fn_call(task, fn, arg);  return; LABEL_CONCAT(L, __LINE__):
#define coro_sleep(tick)         task->_fn->_tick = _coroGetTick(); task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); LABEL_CONCAT(L, __LINE__): if((_coroGetTick() - task->_fn->_tick) < tick) return;
#define coro_timeout(why, tick)  task->_fn->_tick = _coroGetTick(); task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); LABEL_CONCAT(L, __LINE__): \
                                 if(((_coroGetTick() - task->_fn->_tick) < tick)) { if((why))  task->_fn->_state.flag._righted = 1; else return;}else task->_fn->_state.flag._righted = 0

#else
#define coro_begin();            switch (task->_fn->_pc) { case 0:
#define coro_end()               default: break; } task->_fn->_pc = 0
#define coro_yield(why)          task->_fn->_pc = __LINE__; case __LINE__: if(!(why)) return
#define coro_await(fn, arg);     task->_fn->_pc = __LINE__; _coro_fn_call(task, fn, arg);  return; case __LINE__:
#define coro_sleep(tick)         task->_fn->_tick = _coroGetTick(); task->_fn->_pc = __LINE__; case __LINE__: if((_coroGetTick() - task->_fn->_tick) < tick) return
#define coro_timeout(why, tick)  task->_fn->_tick = _coroGetTick(); task->_fn->_pc = __LINE__; case __LINE__: \
                                 if(((_coroGetTick() - task->_fn->_tick) < tick)) { if((why))  task->_fn->_state.flag._righted = 1; else return;}else task->_fn->_state.flag._righted = 0

#endif
