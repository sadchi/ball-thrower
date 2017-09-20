#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "event.h"
#include "servo.h"
#include "state.h"
#include "state_blinker.h"
#include "state_machine.h"

void start_idle(void) {
    enqueue_state(ST_IDLE);
}

void inform_ball_ready(void) {
    enqueue_state(ST_BALL_READY);
    enqueue_event(EV_READY_TO_ARM);
}

void arm(void) {
    enqueue_state(ST_ARMING);
    set_servo_val(SERVO_START);
    osDelay(SERVO_TIME_TO_START_MS);
    set_servo_val(SERVO_END);
    osDelay(SERVO_TIME_TO_END_MS);
    enqueue_event(EV_FIRED);
}
