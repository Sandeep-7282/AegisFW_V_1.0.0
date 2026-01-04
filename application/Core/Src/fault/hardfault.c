/*
 * hardfault.c
 *
 *  Cortex-M HardFault capture for AegisFW.
 *
 *  Captures full CPU context and resets immediately.
 */

#include <stdint.h>
#include "fault_record.h"
#include "stm32f446xx.h"

/* Forward declaration */
static void hardfault_c(uint32_t *stack_ptr);

/*
 * HardFault_Handler
 *
 * Determines active stack (MSP / PSP) and forwards
 * correct stack frame to C handler.
 */
__attribute__((naked))
void HardFault_Handler(void)
{
    __asm volatile (
        "tst lr, #4            \n"
        "ite eq                \n"
        "mrseq r0, msp         \n"
        "mrsne r0, psp         \n"
        "b hardfault_c         \n"
    );
}

static void hardfault_c(uint32_t *stack)
{
    /* Initialize or increment sequence safely */
    if (g_fault_record.magic != FAULT_RECORD_MAGIC)
    {
        g_fault_record.seq = 1;
    }
    else
    {
        g_fault_record.seq++;
    }

    g_fault_record.magic = FAULT_RECORD_MAGIC;

    /* Stacked core registers */
    g_fault_record.r0  = stack[0];
    g_fault_record.r1  = stack[1];
    g_fault_record.r2  = stack[2];
    g_fault_record.r3  = stack[3];
    g_fault_record.r12 = stack[4];
    g_fault_record.lr  = stack[5];
    g_fault_record.pc  = stack[6];
    g_fault_record.psr = stack[7];

    /* System fault status */
    g_fault_record.cfsr  = SCB->CFSR;
    g_fault_record.hfsr  = SCB->HFSR;
    g_fault_record.mmfar = SCB->MMFAR;
    g_fault_record.bfar  = SCB->BFAR;

    /* Ensure compiler and CPU commit all writes */
    __asm volatile ("" ::: "memory");
    __DSB();
    __ISB();

    /* Force system reset */
    NVIC_SystemReset();

    /* Should never reach here */
    while (1);
}
