#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "init.h"

TIM_HandleTypeDef* servo_timer = &htim2;

