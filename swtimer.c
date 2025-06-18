#include "swtimer.h"

struct swtim_t
{
    swtim_cb_t cb;
    void *user_data;
    uint32_t target_ticks;
    uint32_t period_ticks : 31;
    uint32_t state : 1;
};

static uint32_t sys_ticks;

swtim_t *swtim_create(uint32_t period_ticks, swtim_cb_t cb, void *user_data)
{
    swtim_t *timer = (swtim_t *)swtim_malloc(sizeof(swtim_t));
    if (!timer)
        return NULL;
    if (period_ticks > SWTIM_MAX_TICKS)
        period_ticks = SWTIM_MAX_TICKS;
    timer->period_ticks = period_ticks;
    timer->target_ticks = 0;
    timer->user_data = user_data;
    timer->cb = cb;
    return timer;
}

void swtim_delete(swtim_t *timer)
{
    if (!timer || timer->state)
        return;
    swtim_free(timer);
    timer = NULL;
}

void swtim_create_init(swtim_t *timer, uint32_t period_ticks, swtim_cb_t cb, void *user_data)
{
    if (!timer || timer->state)
        return;
    if (period_ticks > SWTIM_MAX_TICKS)
        period_ticks = SWTIM_MAX_TICKS;
    timer->period_ticks = period_ticks;
    timer->target_ticks = 0;
    timer->user_data = user_data;
    timer->cb = cb;
}

void swtim_start(swtim_t *timer)
{
    if (!timer || timer->state)
        return;
    timer->target_ticks = timer->period_ticks + sys_ticks;
    timer->state = 1;
}

void swtim_stop(swtim_t *timer)
{
    if (!timer)
        return;
    timer->state = 0;
}

void swtim_sys_inc(uint16_t ticks)
{
    sys_ticks += ticks;
}

void swtim_handler(swtim_t *timer)
{
    if (!timer || !timer->state)
        return;
    uint32_t now_ticks = sys_ticks;
    if ((int)now_ticks - (int)timer->target_ticks >= 0)
    {
        timer->target_ticks = now_ticks + timer->period_ticks;
        if (timer->cb)
            timer->cb(timer, timer->user_data);
    }
}

void swtim_register_callback(swtim_t *timer, swtim_cb_t cb, void *user_data)
{
    if (!timer)
        return;
    timer->user_data = user_data;
    timer->cb = cb;
}

void swtim_set_period(swtim_t *timer, uint16_t period_ticks)
{
    if (!timer)
        return;
    if (period_ticks > SWTIM_MAX_TICKS)
        period_ticks = SWTIM_MAX_TICKS;
    timer->period_ticks = period_ticks;
}