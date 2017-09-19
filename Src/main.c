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
#include "state_machine.h"
#include "executor.h"



osThreadId defaultTaskHandle;


void StartDefaultTask(void const * argument);


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_NVIC_Init();

    init_state_blinker();
    init_buttons_debouncer();
    init_executor();
    init_state_machine();

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
