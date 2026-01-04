/*
 * reset_cause.c
 *
 *  System reset cause detection and classification.
 *
 */

#include "reset_cause.h"
#include "stm32f446xx.h"
#include "fault_record.h"

static reset_cause_t g_reset_cause = RESET_CAUSE_UNKNOWN;

void reset_cause_detect(void)
{
    uint32_t csr = RCC->CSR;

    /*
     * Priority order:
     * 1) Power-on reset
     * 2) Watchdog reset
     *    - If fault record present → HARDFAULT
     *    - Else → WATCHDOG
     * 3) Software reset
     */
    if (csr & RCC_CSR_PORRSTF)
    {
        g_reset_cause = RESET_CAUSE_POWER_ON;
    }
    else if (csr & RCC_CSR_WDGRSTF)
    {
        if (fault_record_is_valid())
            g_reset_cause = RESET_CAUSE_HARDFAULT;
        else
            g_reset_cause = RESET_CAUSE_WATCHDOG;
    }
    else if (csr & RCC_CSR_SFTRSTF)
    {
        g_reset_cause = RESET_CAUSE_SOFTWARE;
    }
    else
    {
        g_reset_cause = RESET_CAUSE_UNKNOWN;
    }

    /* Clear reset flags once, centrally */
    RCC->CSR |= RCC_CSR_RMVF;
}

reset_cause_t reset_cause_get(void)
{
    return g_reset_cause;
}

const char *reset_cause_str(reset_cause_t cause)
{
    switch (cause)
    {
        case RESET_CAUSE_POWER_ON:  return "POWER_ON";
        case RESET_CAUSE_SOFTWARE:  return "SOFTWARE_RESET";
        case RESET_CAUSE_WATCHDOG:  return "WATCHDOG";
        case RESET_CAUSE_HARDFAULT: return "HARDFAULT";
        default:                    return "UNKNOWN";
    }
}
