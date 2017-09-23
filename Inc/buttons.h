#ifndef __BUTTONS_H
#define __BUTTONS_H

#define DEBOUNCE_MS 50

void EXTI1_IRQHandler(void);
void init_buttons_debouncer(void);

#endif

