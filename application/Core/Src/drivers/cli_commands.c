/*
 * cli_commands.c
 *
 * Application CLI for AegisFW.
 *
 * Simple command parser:
 *  - One command per line
 *  - String compare → handler dispatch
 *  - No dynamic allocation
 *  - RTOS-safe (task context only)
 */

#include "cli_commands.h"

#include "uart_driver.h"
#include "uart_threadsafe.h"

#include "fault_record.h"
#include "reset_cause.h"

#include "adc.h"
#include "oled.h"
#include "tasks_common.h"
#include "task_sensors.h"
#include "task_oled.h"
#include "freertos_init.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------------- Helpers ---------------- */

static void cli_print_help(void)
{
    uart_send_str(
        "\r\nAvailable commands:\r\n"
        "--------------------\r\n"
        "help                     - Show this help\r\n"
        "sys                      - System info\r\n"
        "tasks                    - RTOS task list\r\n"
        "heap                     - Free heap size\r\n"
        "uptime                   - System uptime(running time from last reset)\r\n"
        "\r\n"
        "fault                    - Dump last hardfault\r\n"
        "fclear                   - Clear fault record\r\n"
        "reset                    - Reset cause info\r\n"
        "\r\n"
        "adc                      - Read temperature & vref\r\n"
        "oled clear               - Clear OLED display\r\n"
        "oled msg <row> <text>    - Print text on OLED\r\n"
        "\r\n"
        "sensor pir               - Inject PIR event\r\n"
        "sensor impact            - Inject impact event\r\n"
        "sensor button            - Inject button event\r\n"
        "\r\n"
    );
}

/* ---------------- Core commands ---------------- */

static void cmd_sys(void)
{
    char buf[128];

    snprintf(buf, sizeof(buf),
             "\r\n[SYS]\r\n"
             "Uptime     : %lu sec\r\n"
             "Free Heap  : %u bytes\r\n"
             "Tasks      : %lu\r\n",
             xTaskGetTickCount() / configTICK_RATE_HZ,
             xPortGetFreeHeapSize(),
             uxTaskGetNumberOfTasks());

    uart_send_str(buf);
}

static void cmd_tasks(void)
{
    uart_send_str("\r\n[TASKS]\r\n");
    uart_send_str("Use FreeRTOS trace tools for full details\r\n");
}

static void cmd_heap(void)
{
    char buf[64];
    snprintf(buf, sizeof(buf),
             "Free Heap: %u bytes\r\n",
             xPortGetFreeHeapSize());
    uart_send_str(buf);
}

static void cmd_uptime(void)
{
    char buf[64];
    snprintf(buf, sizeof(buf),
             "Uptime: %lu sec\r\n",
             xTaskGetTickCount() / configTICK_RATE_HZ);
    uart_send_str(buf);
}

/* ---------------- Fault / reset ---------------- */

static void cmd_fault_dump(void)
{
    if (!fault_record_is_valid())
    {
        uart_send_str("No fault record present\r\n");
        return;
    }

    fault_record_t rec = g_fault_record;
    char buf[64];

    uart_send_str("\r\n===== HARDFAULT =====\r\n");

    snprintf(buf, sizeof(buf), "PC   : 0x%08lX\r\n", rec.pc);   uart_send_str(buf);
    snprintf(buf, sizeof(buf), "LR   : 0x%08lX\r\n", rec.lr);   uart_send_str(buf);
    snprintf(buf, sizeof(buf), "CFSR : 0x%08lX\r\n", rec.cfsr); uart_send_str(buf);
    snprintf(buf, sizeof(buf), "HFSR : 0x%08lX\r\n", rec.hfsr); uart_send_str(buf);

    uart_send_str("=====================\r\n");
}

static void cmd_fault_clear(void)
{
    fault_record_clear();
    uart_send_str("Fault record cleared\r\n");
}

static void cmd_reset_info(void)
{
    char buf[64];
    reset_cause_t c = reset_cause_get();

    snprintf(buf, sizeof(buf),
             "Reset Cause: %s\r\n",
             reset_cause_str(c));
    uart_send_str(buf);
}

/* ---------------- Peripheral commands ---------------- */

static void cmd_adc(void)
{
    extern volatile uint16_t adc_dma_buffer[];

    uint16_t t = adc_dma_buffer[0];
    uint16_t v = adc_dma_buffer[1];

    float vref = adc_compute_vref(v);
    float temp = adc_compute_temperature(t, vref);

    char buf[96];
    snprintf(buf, sizeof(buf),
             "ADC: Temp=%.2f C  Vref=%.2f V\r\n",
             temp, vref);

    uart_send_str(buf);
}

static void cmd_oled(char *args)
{
    if (!args)
        return;

    if (strncmp(args, "clear", 5) == 0)
    {
        oled_clear();
        uart_send_str("OLED cleared\r\n");
        return;
    }

    if (strncmp(args, "msg", 3) == 0)
    {
        int row;
        char text[32];

        if (sscanf(args, "msg %d %[^\n]", &row, text) == 2)
        {
            OLED_PrintAsync(row, text);
            uart_send_str("OLED message queued\r\n");
        }
        else
        {
            uart_send_str("Usage: oled msg <row> <text>\r\n");
        }
    }
}

static void cmd_sensor(char *args)
{
    if (!args)
        return;

    sensor_event_t ev = SENSOR_NONE;

    if (strcmp(args, "pir") == 0)       ev = SENSOR_PIR;
    else if (strcmp(args, "impact") == 0) ev = SENSOR_IMPACT;
    else if (strcmp(args, "button") == 0) ev = SENSOR_BUTTON;

    if (ev == SENSOR_NONE)
    {
        uart_send_str("Invalid sensor\r\n");
        return;
    }

    xQueueSend(q_sensors, &ev, 0);
    uart_send_str("Sensor event injected\r\n");
}

/* ---------------- Command dispatcher ---------------- */

void cli_dispatch(char *line)
{
    char *cmd = strtok(line, " ");
    char *args = strtok(NULL, "");

    if (!cmd)
        return;

    if (strcmp(cmd, "help") == 0)          cli_print_help();
    else if (strcmp(cmd, "sys") == 0)      cmd_sys();
    else if (strcmp(cmd, "tasks") == 0)    cmd_tasks();
    else if (strcmp(cmd, "heap") == 0)     cmd_heap();
    else if (strcmp(cmd, "uptime") == 0)   cmd_uptime();

    else if (strcmp(cmd, "fault") == 0)    cmd_fault_dump();
    else if (strcmp(cmd, "fclear") == 0)   cmd_fault_clear();
    else if (strcmp(cmd, "reset") == 0)    cmd_reset_info();

    else if (strcmp(cmd, "adc") == 0)      cmd_adc();
    else if (strcmp(cmd, "oled") == 0)     cmd_oled(args);
    else if (strcmp(cmd, "sensor") == 0)   cmd_sensor(args);

    else
        uart_send_str("Unknown command. Type 'help'\r\n");
}
