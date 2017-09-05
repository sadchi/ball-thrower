#include "init.h"
#include "main.h"
#include "buttons.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "state_blinker.h"
#include "buttons.h"
#include "servo.h"
#include "state.h"
#include "event.h"


#define ENQUEUE_TIMEOUT_MS 50
#define ENQUEUE_TIMEOUT_TCK (pdMS_TO_TICKS(ENQUEUE_TIMEOUT_MS))

osThreadId defaultTaskHandle;

typedef struct {
    state_t cur_state;
    event_t event;
    state_t new_state;
    void (*action)(void);
} fsm_node;


void StartDefaultTask(void const * argument);
static void idle(void);
static void ball_ready(void);
static void arm(void);

static fsm_node ball_thrower_fsm[]= {
    {ST_IDLE,       EV_BALL_READY,   ST_BALL_READY, ball_ready},
    {ST_BALL_READY, EV_READY_TO_ARM, ST_ARMING,     arm },
    {ST_ARMING,     EV_FIRED,        ST_IDLE,       idle}
};


static state_t ball_thrower_fsm_cur_state=ST_IDLE;

static QueueHandle_t eventQ;


static void idle(void) {
    enqueue_state(ST_IDLE, ENQUEUE_TIMEOUT_MS);
}

static void ball_ready(void) {
    static event_t evt=EV_READY_TO_ARM;

    enqueue_state(ST_BALL_READY, ENQUEUE_TIMEOUT_MS);
    xQueueSend(eventQ, &evt, ENQUEUE_TIMEOUT_TCK);

}

static void arm(void) {
    static event_t evt=EV_FIRED;

    enqueue_state(ST_ARMING, ENQUEUE_TIMEOUT_MS);

    set_servo_val(SERVO_START);
    osDelay(SERVO_TIME_TO_START_MS);
    set_servo_val(SERVO_END);
    osDelay(SERVO_TIME_TO_END_MS);

    xQueueSend(eventQ, &evt, ENQUEUE_TIMEOUT_TCK);
}



void TogglePin(void *params) {
    TickType_t last_run;

    last_run=xTaskGetTickCount();

    for(;;) {
        HAL_GPIO_WritePin(AUX_OUT1_GPIO_Port, AUX_OUT1_Pin, GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(AUX_OUT1_GPIO_Port, AUX_OUT1_Pin, GPIO_PIN_RESET);
        osDelayUntil(&last_run, 1000);
    }
}


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_NVIC_Init();

    init_state_blinker();
    init_buttons_debouncer();

    eventQ=xQueueCreate(10, sizeof(event_t));

    // xTaskCreate(TogglePin, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    set_servo_val(SERVO_END);

    osKernelStart();

    while (1);
}


void StartDefaultTask(void const * argument) {

    for(;;) {
        osDelay(1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        HAL_IncTick();
    }
}

void _Error_Handler(char * file, int line) {
    while(1);
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line) {
}

#endif
