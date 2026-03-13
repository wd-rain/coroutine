#include "coroutine.h"
#include <stdio.h>

static CoroTick _coroTick = 0;

void _coro_fn_reset(CoroFn *fn)
{
    fn->_pc = 0;
    fn->_return = NULL;
    fn->_state.u8 = 0;
    fn->_prev = NULL;
}

void _coro_return(Coro *task, void *ret)
{
    if (task->_fn->_prev)
    {
        CoroFn *fn = task->_fn;
        task->_fn = task->_fn->_prev;
        task->_fn->_return = ret;
        _coro_fn_reset(fn);
    }
    else
    {
        _coro_fn_reset(task->_fn);
        task->_fn = NULL;
        task->_return = ret;
    }
}

void _coro_fn_call(Coro *task, CoroFn *fn, void *arg)
{
    _coro_fn_reset(fn);
    fn->_arg = arg;
    fn->_prev = task->_fn;
    task->_fn = fn;
}

CoroTick _coro_defualt_get_tick(void)
{
    return _coroTick;
}

// public api
CoroTickGet _coroGetTick = _coro_defualt_get_tick;

void coro_tick_trigger(void)
{
    _coroTick++;
}

void coro_tick_init(CoroTickGet getTick)
{
    _coroGetTick = getTick;
}

void coro_reset(Coro *task)
{
    task->_return = NULL;
    task->_fn = task->_initFn;
    _coro_fn_reset(task->_fn);
}

void coro_close(Coro *task)
{
    task->_fn = NULL;
}

void coro_open(Coro *task)
{
    if (task->_fn == NULL)
    {
        coro_reset(task);
    }
}

void coro_init(Coro *task, CoroFn *fn, void *arg)
{
    task->_initFn = fn;
    coro_reset(task);
    task->_fn->_arg = arg;
}

