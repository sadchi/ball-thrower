#include "state_blinker.h"
#include "init.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#define bin(a,b,c,d,e,f,g,h) ((a<<7)|(b<<6)|(c<<5)|(d<<4)|(e<<3)|(f<<2)|(g<<1)|h)
#define mcode(len,code) ((len<<8)|code)


const unsigned char char_base = 97;
const unsigned short dot_duration = 100;
const unsigned short dash_duration = dot_duration * 4;
const unsigned short sym_gap_duration = dot_duration * 3;
const unsigned short word_gap_duration = dot_duration * 7;
const unsigned short blink_interval=2000;


const char states_code_list[]= {'e', 't', 'i', 'a'};


const unsigned short morseCodes[]= {
    mcode(2,bin(0,0,0,0,0,0,1,0)), // A
    mcode(4,bin(0,0,0,0,0,0,0,1)), // B
    mcode(4,bin(0,0,0,0,0,1,0,1)), // C
    mcode(3,bin(0,0,0,0,0,0,0,1)), // D
    mcode(1,bin(0,0,0,0,0,0,0,0)), // E
    mcode(4,bin(0,0,0,0,0,1,0,0)), // F
    mcode(3,bin(0,0,0,0,0,0,1,1)), // G
    mcode(4,bin(0,0,0,0,0,0,0,0)), // H
    mcode(2,bin(0,0,0,0,0,0,0,0)), // I
    mcode(4,bin(0,0,0,0,1,1,1,0)), // J
    mcode(3,bin(0,0,0,0,0,1,0,1)), // K
    mcode(4,bin(0,0,0,0,0,0,1,0)), // L
    mcode(2,bin(0,0,0,0,0,0,1,1)), // M
    mcode(2,bin(0,0,0,0,0,0,0,1)), // N
    mcode(3,bin(0,0,0,0,0,1,1,1)), // O
    mcode(4,bin(0,0,0,0,0,1,1,0)), // P
    mcode(4,bin(0,0,0,0,1,0,1,1)), // Q
    mcode(3,bin(0,0,0,0,0,0,1,0)), // R
    mcode(3,bin(0,0,0,0,0,0,0,0)), // S
    mcode(1,bin(0,0,0,0,0,0,0,1)), // T
    mcode(3,bin(0,0,0,0,0,1,0,0)), // U
    mcode(4,bin(0,0,0,0,1,0,0,0)), // V
    mcode(3,bin(0,0,0,0,0,1,1,0)), // W
    mcode(4,bin(0,0,0,0,1,0,0,1)), // X
    mcode(4,bin(0,0,0,0,1,1,0,1)), // Y
    mcode(4,bin(0,0,0,0,0,0,1,1))  // Z
};

static QueueHandle_t stateQ;

__weak void led_on(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

__weak void led_off(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}


static void blink_morse_sym(char c) {
    unsigned short code;
    unsigned char seq;

    code=morseCodes[c - char_base];
    seq = code & 0xFF;

    for(int i=0; i< (code>>8); i++) {
        led_on();
        if( seq & 1) osDelay(dash_duration);
        else osDelay(dot_duration);
        led_off();
        osDelay(dot_duration);
        seq>>=1;
    }
    osDelay(sym_gap_duration);
}

static void blinker_task(void *params) {
    TickType_t last_run;
    state_t state=IDLE;

    last_run=xTaskGetTickCount();
    while(1) {
        xQueueReceive(stateQ, &state, 0);
        blink_morse_sym(states_code_list[state]);
        osDelayUntil(&last_run, blink_interval);
    }
}

void init_state_blinker(void) {
    stateQ=xQueueCreate(4, sizeof(state_t));
    xTaskCreate(blinker_task, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void enqueue_state(state_t state, short delay_ms) {
    xQueueSend(stateQ, &state, pdMS_TO_TICKS(delay_ms));
}

