#include "stm32f1xx_hal.h"

extern void _Error_Handler(char *, int);

void HAL_MspInit(void) {
    __HAL_RCC_AFIO_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

    /**NOJTAG: JTAG-DP Disabled and SW-DP Enabled
    */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm) {

    if(htim_pwm->Instance==TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }

}


void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm) {

    if(htim_pwm->Instance==TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
    }

}
