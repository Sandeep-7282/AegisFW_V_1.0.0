/*
 * main.c
 *
 * AegisFW Application Entry Point
 *
 * Responsibilities:
 *  - Minimal early hardware bring-up
 *  - Diagnostics before RTOS
 *  - Peripheral initialization that must occur before scheduler
 *  - Transfer control to FreeRTOS
 *
 * NOTE:
 *  - Vector table is already remapped by the bootloader
 *  - Watchdog feeding is handled ONLY by HealthMonitor_Task
 */

#include "FreeRTOS.h"
#include "task.h"

#include "freertos_init.h"

#include "uart_driver.h"
#include "gpio_driver.h"
#include "gpio_exti.h"

#include "adc.h"
#include "dma.h"

#include "dwt.h"
#include "watchdog.h"
#include "reset_cause.h"

int main(void)
{
    /*
     * CMSIS system initialization
     * - Clock tree (default, bootloader-approved)
     * - FPU enable
     * - Core configuration
     *
     * VTOR is intentionally NOT modified here.
     */
    SystemInit();

    /* -------------------------------------------------
     * Early diagnostics (no RTOS, no heap usage)
     * ------------------------------------------------- */
    uart_init(115200);
    dwt_init();

    reset_cause_detect();

    uart_send_str("\r\n[AegisFW-APP] Booting...\r\n");

    /* -------------------------------------------------
     * Hardware bring-up (pre-RTOS ownership)
     * ------------------------------------------------- */

    /* Status LED */
    gpio_init_led();

    /* External interrupts (sensors, button) */
    GPIO_EXTI_Init();

    /* ADC + DMA (continuous mode, notifications later) */
    adc1_init_temp_vref_dma();
    dma2_adc1_init();

    /* Do NOT start ADC conversions here
     * ADC_Task will start conversions once RTOS is live
     */

    /*
     * Watchdog configuration only.
     * Feeding is strictly controlled by HealthMonitor_Task.
     */
    Watchdog_Init();

    uart_send_str("[AegisFW-APP] Hardware init done\r\n");

    /* -------------------------------------------------
     * Start RTOS
     * ------------------------------------------------- */
    rtos_start();

    /*
     * Execution should never reach here.
     * If it does, scheduler failed.
     */
    taskDISABLE_INTERRUPTS();
    while (1);
}
