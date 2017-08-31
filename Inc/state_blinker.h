#ifndef __STATE_BLINKER_H
#define __STATE_BLINKER_H

typedef enum {IDLE=0, BALL_READY, ARMING, ERR} state_t;
__weak void led_on(void);
__weak void led_off(void);
void init_state_blinker(void);
void enqueue_state(state_t state, short delay_ms);

#endif
