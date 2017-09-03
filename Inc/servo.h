#ifndef __SERVO_H
#define __SERVO_H

//one tick of PWM timer should be 1mks

#define SERVO_START 544
#define SERVO_END 2400

void set_servo_val(unsigned short value);

#endif

