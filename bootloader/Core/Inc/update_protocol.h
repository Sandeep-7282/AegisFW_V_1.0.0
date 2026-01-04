/*
 * update_protocol.h
 *
 *  Created on: Jan 3, 2026
 *      Author: sande
 */

#ifndef INC_UPDATE_PROTOCOL_H_
#define INC_UPDATE_PROTOCOL_H_

#include <stdint.h>

#define FW_UPDATE_MAGIC  0xA5A55A5A

typedef struct
{
    uint32_t magic;
    uint32_t image_size;
    uint32_t image_crc;
    uint32_t version;
} fw_update_header_t;

#endif /* INC_UPDATE_PROTOCOL_H_ */
