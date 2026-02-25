#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef struct cror Cror;
#if defined(__GNUC__)
#define LABEL_CONCAT2(a, b) a ## b
#define LABEL_CONCAT(a, b) LABEL_CONCAT2(a, b)
typedef void* Lc;
#else
typedef unsigned short Lc;
#endif
typedef void (*CrorHandlerFn)(Cror *task);
typedef void (*CrorInitFn)(void);
typedef void (*CrorDeInitFn)(void);
typedef unsigned int CrorTick;
typedef CrorTick (*CrorTickGet)(void);

typedef struct cror_fn
{
    Lc _pc;
    CrorHandlerFn _handler;
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
    CrorTick _tick;
    struct cror_fn *_prev;
} CrorFn;

typedef struct cror
{
    void *_return;
    CrorFn *_initFn;
    CrorFn *_fn;
} Cror;

void _cror_fn_call(Cror *task, CrorFn *fn, void *arg);
void _cror_return(Cror *task, void *ret);
extern CrorTickGet _crorGetTick;
void cror_tick_trigger(void);
void cror_tick_init(CrorTickGet getTick);
void cror_reset(Cror *task);
void cror_close(Cror *task);
void cror_open(Cror *task);
void cror_return(Cror *task, void *ret);
void cror_init(Cror *task, CrorFn *fn, void *arg);
static inline void cror_run(Cror *task) {  if (task->_fn)  task->_fn->_handler(task); }
#define AsyncFn(name)              void name##_fn (Cror *task); CrorFn name = {0, name##_fn, 0, 0, {{0}}, 0}; void name##_fn (Cror *task)
#define cror_var_def_begin();    
#define cror_var_def_end();
#define cror_var_init_begin();   if(task->_fn->_state.flag._started == 0) {
#define cror_var_init_end();     task->_fn->_state.flag._started = 1; }
#define cror_return(ret)         _cror_return(task, ret); return
#define crorRes                  (task->_fn->_return)
#define crorArg                  (task->_fn->_arg)
#define crorRight                (task->_fn->_state.flag._righted)
#if defined(__GNUC__)
#define cror_begin()             do { if(task->_fn->_pc) goto *task->_fn->_pc; }while(0)
#define cror_end();              task->_fn->_pc = 0;  
#define cror_yield(why)          task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__);  LABEL_CONCAT(L, __LINE__): if(!(why)) return
#define cror_await(fn, arg)      task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); _cror_fn_call(task, fn, arg);  return; LABEL_CONCAT(L, __LINE__):
#define cror_sleep(tick)         task->_fn->_tick = _crorGetTick(); task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); LABEL_CONCAT(L, __LINE__): if((_crorGetTick() - task->_fn->_tick) < tick) return;
#define cror_timeout(why, tick)  task->_fn->_tick = _crorGetTick(); task->_fn->_pc = &&LABEL_CONCAT(L, __LINE__); LABEL_CONCAT(L, __LINE__): \
                                 if(((_crorGetTick() - task->_fn->_tick) < tick)) { if((why))  task->_fn->_state.flag._righted = 1; else return;}else task->_fn->_state.flag._righted = 0

#else
#define cror_begin();            switch (task->_fn->_pc) { case 0:
#define cror_end()               default: break; } task->_fn->_pc = 0
#define cror_yield(why)          task->_fn->_pc = __LINE__; case __LINE__: if(!(why)) return
#define cror_await(fn, arg);     task->_fn->_pc = __LINE__; _cror_fn_call(task, fn, arg);  return; case __LINE__:
#define cror_sleep(tick)         task->_fn->_tick = _crorGetTick(); task->_fn->_pc = __LINE__; case __LINE__: if((_crorGetTick() - task->_fn->_tick) < tick) return
#define cror_timeout(why, tick)  task->_fn->_tick = _crorGetTick(); task->_fn->_pc = __LINE__; case __LINE__: \
                                 if(((_crorGetTick() - task->_fn->_tick) < tick)) { if((why))  task->_fn->_state.flag._righted = 1; else return;}else task->_fn->_state.flag._righted = 0

#endif
