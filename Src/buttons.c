#include "FreeRTOS.h"
#include "buttons.h"
#include "event.h"
#include "init.h"
#include "state_machine.h"
#include "stm32f1xx_hal.h"
#include "timers.h"

#define ENQUEUE_TIMEOUT_TCK (pdMS_TO_TICKS(50))


static volatile TimerHandle_t debounce=NULL;
BaseType_t xHigherPriorityTaskWoken = pdFALSE;

void ball_ready_callback(TimerHandle_t timer) {
    if(HAL_GPIO_ReadPin(BALL_READY_GPIO_Port, BALL_READY_Pin) == GPIO_PIN_RESET) {
        HAL_GPIO_TogglePin(AUX_OUT1_GPIO_Port, AUX_OUT1_Pin);
        enqueue_event(EV_BALL_READY);
    }
}

void init_buttons_debouncer(void) {
    debounce=xTimerCreate("", pdMS_TO_TICKS(DEBOUNCE_MS), pdFALSE, NULL, ball_ready_callback);
}

void EXTI1_IRQHandler(void) {
    xTimerStartFromISR(debounce, &xHigherPriorityTaskWoken);

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
