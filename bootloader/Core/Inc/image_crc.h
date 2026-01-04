/*
 * image_crc.h
 *
 *  Created on: Dec 25, 2025
 *      Author: sande
 */

#ifndef INC_IMAGE_CRC_H_
#define INC_IMAGE_CRC_H_

#pragma once
#include <stdint.h>

/* Compute CRC32 over flash memory */
uint32_t crc32_compute_flash(uint32_t start_addr,
                             uint32_t length_bytes);
uint32_t crc32_compute(const uint8_t *data, uint32_t len);

#endif /* INC_IMAGE_CRC_H_ */
