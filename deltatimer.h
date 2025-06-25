#ifndef __DELTA_TIMER_H
#define __DELTA_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define dtim_malloc(size) malloc(size)
#define dtim_free(ptr) free(ptr)
#define DTIM_MAX_TICKS (((uint32_t)-1) >> 1)

typedef struct dtim_t dtim_t;
typedef void (*dtim_cb_t)(dtim_t *timer, void *user_data);

dtim_t *dtim_create(uint32_t period_ticks, dtim_cb_t cb, void *user_data);
void dtim_delete(dtim_t *timer);
void dtim_start(dtim_t *timer);
void dtim_stop(dtim_t *timer);
void dtim_systick_inc(uint16_t ticks);
void dtim_handler(dtim_t *timer);
void dtim_register_callback(dtim_t *timer, dtim_cb_t cb, void *user_data);
void dtim_set_period(dtim_t *timer, uint32_t period_ticks);

#ifdef __cplusplus
}
#endif

#endif