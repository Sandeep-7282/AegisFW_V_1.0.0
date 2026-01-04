#pragma once
#include <stdint.h>

/* ---- Constants ---- */

#define AEGIS_METADATA_MAGIC   0xA5E15F5AUL
#define AEGIS_METADATA_VERSION 0x00010001UL

typedef enum
{
    IMAGE_SLOT_A = 0,
    IMAGE_SLOT_B = 1
} image_slot_t;

typedef enum
{
    IMAGE_STATE_INVALID = 0,
    IMAGE_STATE_VALID   = 1,
    IMAGE_STATE_PENDING = 2,
    IMAGE_STATE_FAILED  = 3
} image_state_t;

/* ---- Image Info ---- */

typedef struct
{
    uint32_t        version;
    uint32_t        crc32;
    uint32_t        size_bytes;
    image_state_t   state;
    uint32_t        boot_attempts;
    uint32_t        fault_count;
} image_info_t;

/* ---- Global Metadata ---- */

typedef struct
{
    uint32_t        magic;
    uint32_t        struct_version;

    image_slot_t    active_slot;
    image_slot_t    last_booted_slot;

    image_info_t    slot_a;
    image_info_t    slot_b;

    uint32_t        last_reset_reason;
    uint32_t        metadata_crc;

} aegis_metadata_t;
