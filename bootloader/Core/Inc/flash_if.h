/*
 * flash_if.h
 *
 *  Created on: Jan 3, 2026
 *      Author: sande
 */

#ifndef INC_FLASH_IF_H_
#define INC_FLASH_IF_H_

#include <stdint.h>
#include <stdbool.h>

bool flash_erase_slot_a(void);
bool flash_program_buffer(uint32_t addr, const uint8_t *data, uint32_t len);

#endif /* INC_FLASH_IF_H_ */
