#ifndef __EXECUTOR_H
#define __EXECUTOR_H

void init_executor(void);
void enqueue_action(void(*action)(void));

#endif


