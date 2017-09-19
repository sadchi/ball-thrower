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
#define DEQUEUE_TIMEOUT_TCK (pdMS_TO_TICKS(50))


osThreadId defaultTaskHandle;



void StartDefaultTask(void const * argument);
static void idle(void);
static void ball_ready(void);
static void arm(void);








void main_task(void *params) {
    static state_t ball_thrower_fsm_cur_state=ST_IDLE;
    event_t evt;
    BaseType_t res;

    for(;;) {
        res=xQueueReceive(eventQ, &evt, DEQUEUE_TIMEOUT_TCK);

        if(res == pdPASS)
            for(int i=0; i< (sizeof(ball_thrower_fsm)/sizeof(ball_thrower_fsm[0])); i++) {
                if(ball_thrower_fsm[i].cur_state == ball_thrower_fsm_cur_state
                        && ball_thrower_fsm[i].event == evt) {
                    ball_thrower_fsm_cur_state = ball_thrower_fsm[i].new_state;
                    ball_thrower_fsm[i].action();
                    break;
                }
            }
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

    xTaskCreate(main_task, "", configMINIMAL_STACK_SIZE, NULL, 7, NULL);

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
