/*
 * fault_record.c
 *
 *  Persistent fault record storage (.noinit).
 *
 */

#include "fault_record.h"
#include <string.h>

/*
 * Stored in .noinit section to survive reset.
 * Contents are undefined after power-on.
 */
__attribute__((section(".noinit")))
volatile fault_record_t g_fault_record;

bool fault_record_is_valid(void)
{
    return (g_fault_record.magic == FAULT_RECORD_MAGIC);
}

void fault_record_clear(void)
{
    /* Clear entire record deterministically */
    memset((void *)&g_fault_record, 0, sizeof(g_fault_record));
}
