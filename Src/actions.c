#include "state.h"
#include "state_blinker.h"
#include "servo.h"


void start_idle(void) {
    enqueue_state(ST_IDLE, ENQUEUE_TIMEOUT_MS);
}

static void inform_ball_ready(void) {
    static event_t evt=EV_READY_TO_ARM;

    enqueue_state(ST_BALL_READY, ENQUEUE_TIMEOUT_MS);
    xQueueSend(eventQ, &evt, ENQUEUE_TIMEOUT_TCK);

}

static void arm(void) {
    const event_t evt=EV_FIRED;

    enqueue_state(ST_ARMING, ENQUEUE_TIMEOUT_MS);

    set_servo_val(SERVO_START);
    osDelay(SERVO_TIME_TO_START_MS);
    set_servo_val(SERVO_END);
    osDelay(SERVO_TIME_TO_END_MS);

    while(xQueueSend(eventQ, &evt, ENQUEUE_TIMEOUT_TCK) != pdPASS);
}
