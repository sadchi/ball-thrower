#ifndef __EVENT_H
#define __EVENT_H

#include "FreeRTOS.h"
#include "queue.h"

typedef enum {
    EV_BALL_READY,
    EV_FIRED,
    EV_READY_TO_ARM
} event_t;

#endif

