#include "FreeRTOS.h"
#include "actions.h"
#include "event.h"
#include "executor.h"
#include "queue.h"
#include "state.h"
#include "task.h"


static QueueHandle_t eventQ;

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

void enqueue_event(event_t event) {
    xQueueSend(eventQ, &event, portMAX_DELAY);
}


void state_machine_task(void* params) {
    event_t evt;
    static state_t current_state=ST_IDLE;

    while(1) {
        if(xQueueReceive(eventQ, &evt, portMAX_DELAY) == pdTRUE) {
            for(int i=0; i< (sizeof(ball_thrower_fsm)/sizeof(ball_thrower_fsm[0])); i++) {
                if(ball_thrower_fsm[i].cur_state == current_state
                        && ball_thrower_fsm[i].event == evt) {
                    current_state = ball_thrower_fsm[i].new_state;
                    enqueue_action(ball_thrower_fsm[i].action);
                    break;
                }
            }
        }
    }
}

void init_state_machine(void) {
    eventQ=xQueueCreate(20, sizeof(event_t));
    xTaskCreate(state_machine_task, "", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}



