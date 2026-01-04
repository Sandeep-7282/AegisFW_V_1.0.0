# AegisFW – Flash & RAM Memory Map

MCU: STM32F446RE  
Flash Size: 512 KB  
SRAM Size: 128 KB

## Flash Layout

| Region        | Start Addr  | Size     | Purpose |
|--------------|------------|----------|--------|
| Bootloader   | 0x08000000 | 48 KB    | Reset, decision, update |
| App Slot A   | 0x0800C000 | 208 KB   | Primary application |
| App Slot B   | 0x08040000 | 256 KB   | Fallback application |
| Reserved     | 0x08070800 | Remainder| Future use |

## Rules

- Bootloader must never write to its own flash region
- Application must never link below 0x0800C000
- Metadata region is writable only by bootloader
- Application RAM may use `.noinit`, but must not clear it