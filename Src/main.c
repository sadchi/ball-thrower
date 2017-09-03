#include "init.h"
#include "main.h"
#include "buttons.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "state_blinker.h"
#include "buttons.h"


osThreadId defaultTaskHandle;


void StartDefaultTask(void const * argument);


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
    // xTaskCreate(TogglePin, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

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
