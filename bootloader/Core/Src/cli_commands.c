/*
 * cli_commands.c
 *
 *  AegisFW Bootloader CLI.
 *  Manual control and diagnostics interface.
 */

#include "cli_commands.h"
#include "boot_metadata.h"
#include "uart.h"
#include "app_jump.h"
#include "memory_map.h"
#include "image_crc.h"
#include "image_validate.h"
#include "flash_if.h"
#include "flash_ops.h"
#include "update_uart.h"
#include "stm32f4xx.h"
#include <string.h>

// Command Help

void cmd_help(void)
{
    uart_puts(
        "\r\nAvailable commands:\r\n"
        " help          - Show this help\r\n"
        " info          - Show metadata summary\r\n"
        " meta_dump     - Dump full metadata\r\n"
        " boot          - Boot active slot\r\n"
        " boot_a        - Force boot Slot-A\r\n"
        " boot_b        - Force boot Slot-B\r\n"
        " crc_reset_a   - Clear Slot-A CRC\r\n"
        " crc_reset_b   - Clear Slot-B CRC\r\n"
        " switch        - Toggle active slot\r\n"
        " erase_a       - Erase Slot-A flash\r\n"
        " erase_b       - Erase Slot-B flash\r\n"
        " update        - Firmware update (UART)\r\n"
        " reboot        - System reset\r\n"
    );
}
// Info Commands

static void cmd_info(const boot_metadata_t *m)
{
    uart_puts("\r\n[BOOT INFO]\r\n");
    uart_puts(" Active Slot : ");
    uart_putc((m->active_slot == SLOT_A) ? 'A' : 'B');
    uart_puts("\r\n");
}

static void cmd_meta_dump(boot_metadata_t *m)
{
    boot_metadata_dump(m);
}

// Boot Commands

static void cmd_boot(boot_metadata_t *m)
{
    uart_puts("Booting active slot\r\n");
    boot_jump_to_application(
        (m->active_slot == SLOT_A) ? APP_SLOT_A_BASE : APP_SLOT_B_BASE);
}

static void cmd_boot_a(void)
{
    uart_puts("Manual boot Slot-A\r\n");
    boot_jump_to_application(APP_SLOT_A_BASE);
}

static void cmd_boot_b(void)
{
    uart_puts("Manual boot Slot-B\r\n");
    boot_jump_to_application(APP_SLOT_B_BASE);
}

// Metadata Commands

static void cmd_crc_reset_a(boot_metadata_t *m)
{
    m->crcA = 0;
    boot_metadata_store_safe(m);
    uart_puts("Slot-A CRC cleared\r\n");
}

static void cmd_crc_reset_b(boot_metadata_t *m)
{
    m->crcB = 0;
    boot_metadata_store_safe(m);
    uart_puts("Slot-B CRC cleared\r\n");
}

static void cmd_switch(boot_metadata_t *m)
{
    m->active_slot = (m->active_slot == SLOT_A) ? SLOT_B : SLOT_A;
    boot_metadata_store_safe(m);
    uart_puts("Active slot switched\r\n");
}

//Flash / System Commands

static void cmd_erase_a(void)
{
    uart_puts("Erasing Slot-A\r\n");
    flash_erase_range(APP_SLOT_A_BASE, APP_SLOT_A_SIZE_BYTES);
}

static void cmd_erase_b(void)
{
    uart_puts("Erasing Slot-B\r\n");
    flash_erase_range(APP_SLOT_B_BASE, APP_SLOT_B_SIZE_BYTES);
}

static void cmd_update(boot_metadata_t *m)
{
    bl_uart_send_str("Entering update mode\r\n");
    cmd_crc_reset_a(m);
    bl_receive_and_program_image();
}

static void cmd_reboot(void)
{
    uart_puts("Rebooting system\r\n");
    NVIC_SystemReset();
}

// Command Dispatcher

void cli_commands(const char *cmd, boot_metadata_t *meta)
{
    if (!strcmp(cmd, "help"))        return cmd_help();
    if (!strcmp(cmd, "info"))        return cmd_info(meta);
    if (!strcmp(cmd, "meta_dump"))   return cmd_meta_dump(meta);
    if (!strcmp(cmd, "boot"))        return cmd_boot(meta);
    if (!strcmp(cmd, "boot_a"))      return cmd_boot_a();
    if (!strcmp(cmd, "boot_b"))      return cmd_boot_b();
    if (!strcmp(cmd, "crc_reset_a")) return cmd_crc_reset_a(meta);
    if (!strcmp(cmd, "crc_reset_b")) return cmd_crc_reset_b(meta);
    if (!strcmp(cmd, "switch"))      return cmd_switch(meta);
    if (!strcmp(cmd, "erase_a"))     return cmd_erase_a();
    if (!strcmp(cmd, "erase_b"))     return cmd_erase_b();
    if (!strcmp(cmd, "update"))      return cmd_update(meta);
    if (!strcmp(cmd, "reboot"))      return cmd_reboot();

    uart_puts("Unknown command. Type 'help'\r\n");
}
