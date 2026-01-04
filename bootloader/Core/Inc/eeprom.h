/*
 * eeprom.h
 *
 *  Created on: Dec 24, 2025
 *      Author: sande
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <stdint.h>
#include <stm32f4xx.h>
#include <boot_metadata.h>

#define EEPROM_OK        0
#define EEPROM_ERROR    -1
#define EEPROM_I2C_ADDR        0x50
#define EEPROM_PAGE_SIZE      64
#define EEPROM_WRITE_DELAY_MS 5

int eeprom_init(void);

int eeprom_read(uint16_t mem_addr,
                uint8_t *buf,
                uint16_t len);

int eeprom_write(uint16_t mem_addr,
                 const uint8_t *buf,
                 uint16_t len);

int boot_metadata_load_or_init(boot_metadata_t *meta);


#endif /* INC_EEPROM_H_ */
