#include "deltatimer.h"

struct dtim_t {
    dtim_cb_t cb;
    void *user_data;
    uint32_t target_ticks;
    uint32_t period_ticks : 31;
    uint32_t state : 1;
};

volatile static uint32_t dtim_systicks;

dtim_t *dtim_create(uint32_t period_ticks, dtim_cb_t cb, void *user_data) {
    dtim_t *timer = (dtim_t *)dtim_malloc(sizeof(dtim_t));
    if (!timer)
        return NULL;
    if (period_ticks > DTIM_MAX_TICKS)
        period_ticks = DTIM_MAX_TICKS;
    timer->period_ticks = period_ticks;
    timer->target_ticks = 0;
    timer->user_data = user_data;
    timer->cb = cb;
    return timer;
}

void dtim_delete(dtim_t *timer) {
    if (!timer)
        return;
    dtim_free(timer);
    timer = NULL;
}

void dtim_start(dtim_t *timer) {
    if (!timer || timer->state)
        return;
    timer->target_ticks = timer->period_ticks + dtim_systicks;
    timer->state = 1;
}

void dtim_stop(dtim_t *timer) {
    if (!timer)
        return;
    timer->state = 0;
}

void dtim_systick_inc(uint16_t ticks) { dtim_systicks += ticks; }

void dtim_handler(dtim_t *timer) {
    if (!timer || !timer->state)
        return;
    uint32_t now_ticks = dtim_systicks;
    if ((int32_t)(now_ticks - timer->target_ticks) >= 0) {
        timer->target_ticks = now_ticks + timer->period_ticks;
        if (timer->cb)
            timer->cb(timer, timer->user_data);
    }
}

void dtim_register_callback(dtim_t *timer, dtim_cb_t cb, void *user_data) {
    if (!timer)
        return;
    timer->user_data = user_data;
    timer->cb = cb;
}

void dtim_set_period(dtim_t *timer, uint32_t period_ticks) {
    if (!timer)
        return;
    if (period_ticks > DTIM_MAX_TICKS)
        period_ticks = DTIM_MAX_TICKS;
    timer->period_ticks = period_ticks;
}