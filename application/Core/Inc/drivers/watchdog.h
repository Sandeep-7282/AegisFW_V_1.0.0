/*
 * watchdog.h
 *
 *  Created on: Dec 31, 2025
 *      Author: sande
 */

#ifndef INC_DRIVERS_WATCHDOG_H_
#define INC_DRIVERS_WATCHDOG_H_

void Watchdog_Init(void);
void Watchdog_Feed(void);
void Watchdog_PrintLastResetCause(void);

#endif /* INC_DRIVERS_WATCHDOG_H_ */
