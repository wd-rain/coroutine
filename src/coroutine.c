#include "coroutine.h"
#include <stdio.h>

static CrorTick _crorTick = 0;

void _cror_fn_reset(CrorFn *fn)
{
    fn->_pc = 0;
    fn->_return = NULL;
    fn->_state.u8 = 0;
    fn->_prev = NULL;
}

void _cror_return(Cror *task, void *ret)
{
    if (task->_fn->_prev)
    {
        CrorFn *fn = task->_fn;
        task->_fn = task->_fn->_prev;
        task->_fn->_return = ret;
        _cror_fn_reset(fn);
    }
    else
    {
        _cror_fn_reset(task->_fn);
        task->_fn = NULL;
        task->_return = ret;
    }
}

void _cror_fn_call(Cror *task, CrorFn *fn, void *arg)
{
    _cror_fn_reset(fn);
    fn->_arg = arg;
    fn->_prev = task->_fn;
    task->_fn = fn;
}

CrorTick _cror_defualt_get_tick(void)
{
    return _crorTick;
}

// public api
CrorTickGet _crorGetTick = _cror_defualt_get_tick;

void cror_tick_trigger(void)
{
    _crorTick++;
}

void cror_tick_init(CrorTickGet getTick)
{
    _crorGetTick = getTick;
}

void cror_reset(Cror *task)
{
    task->_return = NULL;
    task->_fn = task->_initFn;
    _cror_fn_reset(task->_fn);
}

void cror_close(Cror *task)
{
    task->_fn = NULL;
}

void cror_open(Cror *task)
{
    if (task->_fn == NULL)
    {
        cror_reset(task);
    }
}

void cror_init(Cror *task, CrorFn *fn, void *arg)
{
    task->_initFn = fn;
    cror_reset(task);
    task->_fn->_arg = arg;
}

