
/* Flash base */
#define FLASH_BASE_ADDR        0x08000000UL

/* Bootloader */
#define BL_BASE_ADDR           0x08000000UL
#define BL_SIZE_BYTES          (48UL * 1024UL)
#define BL_END_ADDR            (BL_BASE_ADDR + BL_SIZE_BYTES)

/* Application slots */
#define APP_SLOT_A_BASE        0x0800C000UL
#define APP_SLOT_A_SIZE_BYTES   (208UL * 1024UL)

#define APP_SLOT_B_BASE        0x08040000UL
#define APP_SLOT_B_SIZE_BYTES   (256UL * 1024UL)

/* SRAM */
#define SRAM_BASE_ADDR        0x20000000UL
#define SRAM_SIZE_BYTES      (128UL * 1024UL)
