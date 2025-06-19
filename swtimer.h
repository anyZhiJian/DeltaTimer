#ifndef __SWTIMER_H
#define __SWTIMER_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define swtim_malloc(size) malloc(size)
#define swtim_free(ptr) free(ptr)
#define SWTIM_MAX_TICKS (((uint32_t)-1) >> 1)

typedef struct swtim_t swtim_t;
typedef void (*swtim_cb_t)(swtim_t *timer, void *user_data);

swtim_t *swtim_create(uint32_t period_ticks, swtim_cb_t cb, void *user_data);
void swtim_delete(swtim_t *timer);
void swtim_start(swtim_t *timer);
void swtim_stop(swtim_t *timer);
void swtim_systick_inc(uint16_t ticks);
void swtim_handler(swtim_t *timer);
void swtim_register_callback(swtim_t *timer, swtim_cb_t cb, void *user_data);
void swtim_set_period(swtim_t *timer, uint32_t period_ticks);

#ifdef __cplusplus
}
#endif

#endif