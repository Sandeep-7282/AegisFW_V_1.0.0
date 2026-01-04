/*
 * image_validate.h
 *
 *  Created on: Dec 24, 2025
 *      Author: sande
 */

#ifndef INC_IMAGE_VALIDATE_H_
#define INC_IMAGE_VALIDATE_H_

#pragma once
#include <stdint.h>

/* Image validation result */
typedef enum
{
    IMG_OK = 0,

    IMG_ERR_ZERO_VECTOR,
    IMG_ERR_MSP_RANGE,
    IMG_ERR_MSP_ALIGN,
    IMG_ERR_RESET_RANGE,
    IMG_ERR_RESET_THUMB

} img_status_t;

/* Validate Cortex-M vector table of an image */
img_status_t image_validate_vector(uint32_t app_base);

#endif /* INC_IMAGE_VALIDATE_H_ */
