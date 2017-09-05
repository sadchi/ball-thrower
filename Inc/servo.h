#ifndef __SERVO_H
#define __SERVO_H

//one tick of PWM timer should be 1mks

#define SERVO_START 544
#define SERVO_END 2400
#define SERVO_TIME_TO_START_MS 3000
#define SERVO_TIME_TO_END_MS 3000


void set_servo_val(unsigned short value);

#endif

