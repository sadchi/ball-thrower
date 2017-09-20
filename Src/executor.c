#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

static QueueHandle_t executorQ;

void executor_task(void* params);

void init_executor(void) {
    executorQ=xQueueCreate(20, sizeof(void*));
    xTaskCreate(executor_task, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void enqueue_action(void(*action)(void)) {
    xQueueSend(executorQ, &action, portMAX_DELAY);
}

void executor_task(void* params) {
    void (*action)(void);

    while(1) {
        if(xQueueReceive(executorQ, &action, portMAX_DELAY) == pdTRUE) (*action)();
    }
}

