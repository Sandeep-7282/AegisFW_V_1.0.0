/*
 * health_manager.h
 *
 *  Created on: Dec 31, 2025
 *      Author: sande
 */

#ifndef INC_DRIVERS_HEALTH_MANAGER_H_
#define INC_DRIVERS_HEALTH_MANAGER_H_


#include <stdint.h>

void Heartbeat_CLI(void);
void Heartbeat_Sensor(void);
void Heartbeat_ADC(void);
void Heartbeat_OLED(void);

void HealthMonitor_Task(void *p);

#endif /* INC_DRIVERS_HEALTH_MANAGER_H_ */
