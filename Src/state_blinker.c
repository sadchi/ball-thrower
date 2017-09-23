#include "state_blinker.h"
#include "init.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#define bin(a,b,c,d,e) ((a<<4)|(b<<3)|(c<<2)|(d<<1)|e)
#define mcode(len,code) ((len<<5)|code)


const unsigned short dot_duration = 100;
const unsigned short dash_duration = dot_duration * 4;
const unsigned short sym_gap_duration = dot_duration * 3;
const unsigned short word_gap_duration = dot_duration * 7;
const unsigned short blink_interval=1000;


typedef struct {
    state_t state;
    unsigned char code;
} states_code;

const states_code states_code_list[]= {
    {ST_IDLE, mcode(1, bin(0,0,0,0,0))},
    {ST_BALL_READY, mcode(1, bin(0,0,0,0,1))},
    {ST_ARMING, mcode(2, bin(0,0,0,0,0))},
    {ST_ERR, mcode(2, bin(0,0,0,1,0))}
};

static QueueHandle_t stateQ;

__weak void led_on(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

__weak void led_off(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}


static void blink_morse_sym(unsigned char code) {
    unsigned char seq;

    seq = code & 0x1F;

    for(int i=0; i< (code>>5); i++) {
        led_on();
        if( seq & 1) osDelay(dash_duration);
        else osDelay(dot_duration);
        led_off();
        osDelay(dot_duration);
        seq>>=1;
    }
    osDelay(sym_gap_duration);
}

unsigned char state_to_code(state_t state) {
    for(int i=0; i< (sizeof(states_code_list)/sizeof(states_code_list[0])); i++) {
        if (states_code_list[i].state == state) return states_code_list[i].code;
    }

    return 0;
}

static void blinker_task(void *params) {
    TickType_t last_run;
    static state_t state=ST_IDLE;
    unsigned char code=state_to_code(state);

    last_run=xTaskGetTickCount();
    while(1) {
        if(xQueueReceive(stateQ, &state, 0)) {
            code=state_to_code(state);
        }
        blink_morse_sym(code);
        osDelayUntil(&last_run, blink_interval);
    }
}

void init_state_blinker(void) {
    stateQ=xQueueCreate(4, sizeof(state_t));
    xTaskCreate(blinker_task, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void enqueue_state(state_t state) {
    xQueueSend(stateQ, &state, portMAX_DELAY);
}

