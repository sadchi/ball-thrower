#include "stm32f1xx_hal.h"
#include "init.h"
#include "buttons.h"
#include "FreeRTOS.h"
#include "timers.h"


static volatile TimerHandle_t debounce=NULL;
BaseType_t xHigherPriorityTaskWoken = pdFALSE;

void ball_ready_callback(TimerHandle_t timer) {
    HAL_GPIO_TogglePin(AUX_OUT1_GPIO_Port, AUX_OUT1_Pin);
}

void init_buttons_debouncer(void) {
    debounce=xTimerCreate("", pdMS_TO_TICKS(DEBOUNCE_MS), pdFALSE, NULL, ball_ready_callback);
}

void EXTI1_IRQHandler(void) {
    if(HAL_GPIO_ReadPin(BALL_READY_GPIO_Port, BALL_READY_Pin) == GPIO_PIN_SET)
        xTimerStartFromISR(debounce, &xHigherPriorityTaskWoken);
    else
        xTimerStopFromISR(debounce, &xHigherPriorityTaskWoken);

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
