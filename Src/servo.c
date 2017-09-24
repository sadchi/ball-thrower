#include "servo.h"
#include "init.h"
#include "servo_periph.h"


void set_servo_val(unsigned short value) {
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(servo_timer, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(servo_timer, TIM_CHANNEL_1);
}
