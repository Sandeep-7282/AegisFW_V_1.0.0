/*
 * fault_record.h
 *
 *  Created on: Jan 2, 2026
 *      Author: sande
 */

#ifndef INC_FAULT_FAULT_RECORD_H_
#define INC_FAULT_FAULT_RECORD_H_

#include "stdint.h"
#include "stdbool.h"
/*
 * Persistent HardFault record
 * Lives in .noinit (survives reset, lost on power cycle)
 */
typedef struct
{
    uint32_t magic;
    uint32_t seq;      /* increments on every fault */
    /* Cortex-M stacked registers */
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;

    /* Fault status registers */
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t mmfar;
    uint32_t bfar;

    uint32_t reserved;
} fault_record_t;

#define FAULT_RECORD_MAGIC  0xDEADBEEF

bool fault_record_is_valid(void);
void fault_record_clear(void);

extern volatile fault_record_t g_fault_record;

#endif /* INC_FAULT_FAULT_RECORD_H_ */
