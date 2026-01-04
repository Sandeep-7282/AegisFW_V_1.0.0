/*
 * app_jump.h
 *
 *  Created on: Dec 25, 2025
 *      Author: sande
 */

#ifndef INC_APP_JUMP_H_
#define INC_APP_JUMP_H_

#pragma once
#include <stdint.h>
#include "memory_map.h"

void boot_jump_to_application(uint32_t app_base);


#endif /* INC_APP_JUMP_H_ */
