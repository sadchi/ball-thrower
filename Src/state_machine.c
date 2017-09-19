#include "state.h"
#include "actions.h"
#include "FreeRTOS.h"
#include "queue.h"

static QueueHandle_t stateQ;

typedef struct {
    state_t cur_state;
    event_t event;
    state_t new_state;
    void (*action)(void);
} fsm_node;



static fsm_node ball_thrower_fsm[]= {
    {ST_IDLE,       EV_BALL_READY,   ST_BALL_READY, inform_ball_ready},
    {ST_BALL_READY, EV_READY_TO_ARM, ST_ARMING,     arm },
    {ST_ARMING,     EV_FIRED,        ST_IDLE,       start_idle}
};



