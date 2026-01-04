/*
 * flash_ops.h
 *
 *  Created on: Dec 26, 2025
 *      Author: sande
 */

#ifndef INC_FLASH_OPS_H_
#define INC_FLASH_OPS_H_

#pragma once
#include <stdint.h>

void flash_erase_range(uint32_t address, uint32_t size);

#endif /* INC_FLASH_OPS_H_ */
