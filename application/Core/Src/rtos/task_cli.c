/*
 * task_cli.c
 *
 * Diagnostic CLI task for AegisFW application.
 * Non-critical, watchdog-aware, RX-driven.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "uart_driver.h"
#include "uart_threadsafe.h"
#include "tasks_common.h"
#include "health_manager.h"
#include "stdio.h"
#include "cli_commands.h"

#define CLI_LINE_MAX 64

/* ---------- CLI Task Entry ---------- */

void CLI_Task(void *arg)
{
    (void)arg;

    char line[CLI_LINE_MAX];
    uint32_t idx = 0;

    uart_send_str("\r\n[CLI] Ready. Type 'help'\r\n");
    uart_send_str("AegisFW> ");

    for (;;)
    {
        Heartbeat_CLI();

        char c = uart_read();   // blocking

        if (c == '\r' || c == '\n')
        {
            uart_send_str("\r\n");
            line[idx] = 0;

            if (idx > 0)
                cli_dispatch(line);
            idx = 0;
            uart_send_str("AegisFW> ");
        }
        else if ((c == '\b' || c == 127) && idx > 0)
        {
            idx--;
            uart_send_str("\b \b");
        }
        else if (idx < CLI_LINE_MAX - 1)
        {
            uart_send_byte(c);
            line[idx++] = c;
        }
    }
}
