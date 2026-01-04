/*
 * reset_cause.h
 *
 *  Created on: Jan 2, 2026
 *      Author: sande
 */

#ifndef INC_DRIVERS_RESET_CAUSE_H_
#define INC_DRIVERS_RESET_CAUSE_H_

#include <stdint.h>

typedef enum
{
    RESET_CAUSE_UNKNOWN = 0,
    RESET_CAUSE_POWER_ON,
    RESET_CAUSE_SOFTWARE,
    RESET_CAUSE_WATCHDOG,
    RESET_CAUSE_HARDFAULT
} reset_cause_t;

void reset_cause_detect(void);
reset_cause_t reset_cause_get(void);
const char *reset_cause_str(reset_cause_t cause);

#endif /* INC_DRIVERS_RESET_CAUSE_H_ */
