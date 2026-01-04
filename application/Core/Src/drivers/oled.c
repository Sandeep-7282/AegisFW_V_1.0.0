/*
 * oled.c
 *
 *  SSD1306 OLED driver (I2C).
 *  Non-critical output device for AegisFW.
 */

#include "oled.h"
#include "i2c.h"
#include "font5x7.h"
#include <stdbool.h>
#include <stdint.h>

#define OLED_ADDR  0x3D
#define OLED_PAGES 8
#define OLED_COLS  128

static bool oled_present = false;

/* Low-level helpers */
static void oled_cmd(uint8_t cmd)
{
    uint8_t buf[2] = { 0x00, cmd };
    i2c2_write(OLED_ADDR, buf, 2);
}

static void oled_data(uint8_t data)
{
    uint8_t buf[2] = { 0x40, data };
    i2c2_write(OLED_ADDR, buf, 2);
}

bool oled_is_present(void)
{
    return oled_present;
}

void oled_init(void)
{
    /* Power-up delay */
    for (volatile uint32_t i = 0; i < 100000; i++);

    oled_cmd(0xAE); /* Display OFF */

    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0x81); oled_cmd(0x7F);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);   /* Display ON */

    oled_present = true;
    oled_clear();
}

void oled_clear(void)
{
    if (!oled_present)
        return;

    for (uint8_t page = 0; page < OLED_PAGES; page++)
    {
        oled_cmd(0xB0 + page);
        oled_cmd(0x00);
        oled_cmd(0x10);

        for (uint8_t col = 0; col < OLED_COLS; col++)
            oled_data(0x00);
    }
}

void oled_print(uint8_t row, const char *str)
{
    if (!oled_present || row >= OLED_PAGES || !str)
        return;

    oled_cmd(0xB0 + row);
    oled_cmd(0x00);
    oled_cmd(0x10);

    while (*str)
    {
        char c = *str++;
        if (c < 32 || c > 122)
            c = ' ';

        const uint8_t *glyph = font5x7[c - 32];

        for (uint8_t i = 0; i < 5; i++)
            oled_data(glyph[i]);

        oled_data(0x00); /* spacing */
    }
}
