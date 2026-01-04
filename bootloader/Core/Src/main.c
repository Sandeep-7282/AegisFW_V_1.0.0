/*
 * main.c
 *  AegisFW Bootloader Entry
 */

#include "stm32f4xx.h"
#include "clock.h"
#include "uart.h"
#include "eeprom.h"
#include "boot_metadata.h"
#include "boot_decision.h"
#include "cli_commands.h"
#include "image_validate.h"
#include "image_crc.h"
#include "memory_map.h"
#include "app_jump.h"
#include "string.h"

static boot_metadata_t meta;
static boot_decision_t decision;

static void platform_init(void)
{
    clock_init_minimal();   /* HSI @ 16 MHz, no PLL */
    uart_init();            /* Blocking UART only */
    eeprom_init();          /* Persistent metadata access */
}

static void boot_banner(void)
{
    uart_puts("\r\n====================================\r\n");
    uart_puts("        AEGISFW BOOTLOADER            \r\n");
    uart_puts("====================================\r\n");
}

static void image_sanity_report(void)
{
    img_status_t a = image_validate_vector(APP_SLOT_A_BASE);
    img_status_t b = image_validate_vector(APP_SLOT_B_BASE);

    uart_puts("Image sanity:\r\n");
    uart_puts("  Slot A : "); uart_puts(a == IMG_OK ? "OK\r\n" : "INVALID\r\n");
    uart_puts("  Slot B : "); uart_puts(b == IMG_OK ? "OK\r\n" : "INVALID\r\n");
}

static void handle_boot_decision(boot_decision_t d)
{
    switch (d)
    {
        case BOOT_DECISION_BOOT:
            uart_puts("Booting active slot...\r\n");
            boot_jump_to_application(slot_base(meta.active_slot));
            break;

        case BOOT_DECISION_ROLLBACK:
            uart_puts("Rollback triggered. Switching slot.\r\n");
            boot_metadata_store_safe(&meta);
            boot_jump_to_application(slot_base(meta.active_slot));
            break;

        case BOOT_DECISION_STAY:
        default:
            uart_puts("Staying in bootloader.\r\n");
            break;
    }
}

static void bootloader_cli(void)
{
    char cmd[64];
    int idx = 0;

    uart_puts("\r\n--- Bootloader CLI ---\r\n");
    cmd_help();
    uart_puts("Press ENTER to boot.\r\n> ");

    while (1)
    {
        char c = uart_getc();

        if (c == '\r' || c == '\n')
        {
            cmd[idx] = 0;

            if (idx == 0)
            {
                decision = boot_decide(&meta);
                handle_boot_decision(decision);
            }
            else
            {
                cli_commands(cmd, &meta);
            }

            idx = 0;
            uart_puts("\r\n> ");
        }
        else
        {
            uart_putc(c);
            if (idx < (int)sizeof(cmd) - 1)
                cmd[idx++] = c;
        }
    }
}

int main(void)
{
    platform_init();
    boot_banner();

    boot_metadata_load_or_init(&meta);
    boot_metadata_dump(&meta);

    image_sanity_report();
    bootloader_cli();

    /* Should never reach here */
    while (1);
}
