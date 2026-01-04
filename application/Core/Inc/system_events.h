/*
 * system_events.h
 *
 *  Created on: Dec 27, 2025
 *      Author: sande
 */

#ifndef INC_SYSTEM_EVENTS_H_
#define INC_SYSTEM_EVENTS_H_

#include <stdint.h>

typedef enum {
    EVT_NONE = 0,

    EVT_BTN_PRESSED,
    EVT_PIR_TRIGGERED,
    EVT_IMPACT_DETECTED,

    EVT_ADC_TEMP_READY,
    EVT_DISTANCE_READY,

} event_id_t;

typedef struct {
    event_id_t id;
    uint32_t   data;
} event_msg_t;


#endif /* INC_SYSTEM_EVENTS_H_ */
