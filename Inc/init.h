#ifndef __INIT_H
#define __INIT_H

#include "stm32f1xx_hal.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define AUX_OUT1_Pin GPIO_PIN_14
#define AUX_OUT1_GPIO_Port GPIOC
#define SERVO_Pin GPIO_PIN_0
#define SERVO_GPIO_Port GPIOA
#define BALL_READY_Pin GPIO_PIN_1
#define BALL_READY_GPIO_Port GPIOA
#define BALL_READY_EXTI_IRQn EXTI1_IRQn

extern TIM_HandleTypeDef htim2;

void MX_GPIO_Init(void);
void SystemClock_Config(void);
void MX_NVIC_Init(void);
void MX_TIM2_Init(void);

#endif
