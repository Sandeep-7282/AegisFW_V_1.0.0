AegisFW

AegisFW is a fault-tolerant, RTOS-based embedded firmware platform with a dual-image bootloader, runtime diagnostics, and automatic 
recovery mechanisms implemented on STM32F446RE (Cortex-M4).

The project is built to demonstrate embedded firmware engineering practices I learnt:

=>Deterministic boot

=>Safe firmware update

=>Fault capture and recovery

=>Watchdog-based health supervision

=>Clean RTOS task architecture

=>No HAL is used. Only CMSIS + register-level programming.

🎯 Design Objectives

=>Safe boot with rollback capability

=>Dual-slot firmware update mechanism

=>Persistent metadata stored in EEPROM

=>Deterministic FreeRTOS task model

=>Watchdog driven by system health, not timers

🧱 High-Level Architecture
Power-On / Reset
        |
        v
+----------------------+
|      Bootloader      |
|----------------------|
| Reset Handler        |
| EEPROM Metadata Load |
| Image Validation     |
| Boot_CLI (UART)      |
| Slot&Boot Decision   |
+----------+-----------+
           |
           v
+----------------------+
|    Application FW    |
|----------------------|
| FreeRTOS Scheduler   |
| Health Supervision   |
| Fault Logging        |
| CLI (UART)           |
+----------------------+

🔩 Hardware Platform

MCU: STM32F446RE

Clock: Bootloader-controlled system clock

RTOS: FreeRTOS

Power: USB / external 5V

🔌 Hardware Connections Guide
📟 EEPROM (Critical Component)

Used by the bootloader to store persistent firmware metadata.

Signal	MCU Pin	Description
EEPROM SDA	PB7	I2C1 SDA
EEPROM SCL	PB6	I2C1 SCL
EEPROM Type	AT24Cxx	External I2C EEPROM
Address	0x50	7-bit

Usage

Stores firmware metadata

Survives resets and power cycles

Used for rollback and validation

🌡️ Internal Sensors (ADC)
Signal	Source
Temperature	ADC1 Channel 16
VREFINT	ADC1 Channel 17

Sampled via ADC + DMA

Continuous circular buffer

📺 OLED Display (SSD1306)
Signal	MCU Pin
SCL	PB10
SDA	PB11
I2C	I2C2
Address	0x3D

OLED is non-critical. System continues without it.

🧠 Sensors & Inputs
Signal	MCU Pin	Interrupt
PIR Sensor	PA0	EXTI0
Impact Sensor	PB5	EXTI5
User Button	PC13	EXTI13
🖥️ UART (CLI)
Peripheral	Usage
USART2	Bootloader + Application CLI
TX/RX	PA2 / PA3
Baud	115200
🧠 EEPROM Metadata Design & Flow
Metadata Layout (Simplified)
EEPROM
+---------------------+ 0x0000
| Slot-A Metadata     |
+---------------------+ 0x0100
| Slot-B Metadata     |
+---------------------+


Each metadata block contains:

=>Magic number

=>Firmware version

=>Image size

=>CRC checksum

=>Active slot flag

=>Boot attempt counters

=>EEPROM Redundancy Strategy

=>Two metadata copies per slot

=>Bootloader always prioritizes slot A and rollbacks to another slot on Failure

🔁 Detailed Boot Flow (Critical Section)
1. Reset Entry (Bootloader)
Reset_Handler
 ├─ Minimal clock init
 ├─ UART init
 ├─ Read reset cause
 ├─ Read EEPROM metadata
 ├─ Validate Slot-A CRC
 ├─ Validate Slot-B CRC
 └─ Decide boot path

2. Boot Decision Logic
If user forces slot (CLI) → boot forced slot
Else if active slot valid → boot active slot
Else if backup slot valid → boot backup slot
Else → stay in bootloader

3. Application Jump
- Disable interrupts
- Stop SysTick
- Set MSP from app vector
- Remap VTOR to app base
- Jump to Reset_Handler

The application never modifies VTOR.

🖥️ Bootloader CLI (UART)

The bootloader provides a full command-line interface for firmware management.

Available Bootloader Commands
help          - Show this help
info          - Show metadata summary
meta_dump     - Dump full metadata
boot          - Boot active slot
boot_a        - Force boot Slot-A
boot_b        - Force boot Slot-B
crc_reset_a   - Clear Slot-A CRC
crc_reset_b   - Clear Slot-B CRC
switch        - Toggle active slot
erase_a       - Erase Slot-A flash
erase_b       - Erase Slot-B flash
update        - Firmware update (UART)
reboot        - System reset

Bootloader CLI Use Cases

Force boot into a specific slot

Recover from bad firmware

Erase corrupted images

Upload new firmware over UART

Inspect EEPROM metadata In-field

🧵 Application Runtime Architecture
Application Startup Flow
main()
 ├─ SystemInit()
 ├─ UART + DWT init
 ├─ Reset cause detection
 ├─ GPIO / EXTI init
 ├─ ADC + DMA init
 ├─ Watchdog init
 └─ rtos_start()

🩺 Health Supervision Flow
Each Task
  └─ Sets heartbeat flag

HealthMonitor_Task (5s)
  ├─ All heartbeats OK → Feed watchdog
  └─ Missing heartbeat → Stop feeding → Reset -> Updates Reset Cause for bootloader to decide


No task or ISR feeds the watchdog directly.

⚠️ Fault Handling Flow
HardFault
 ├─ Capture CPU registers
 ├─ Store in .noinit RAM
 ├─ Force reset
 └─ CLI dump after reboot

🧵 FreeRTOS Tasks
Task	Role	Priority
HealthMonitor	Watchdog control	Highest
CLI	User interface	High
Sensors	EXTI + Timer events	Medium
ADC	DMA processing	Medium
OLED	Display output	Low
Start_Peripherals	Deferred init	Medium
🖥️ Application CLI Commands
| Command            | Description             |
| ------------------ | ----------------------- |
| `help`             | List available commands |
| `sysinfo`          | Heap, task count        |
| `uptime`           | Application uptime      |
| `reset`            | Last reset cause        |
| `fault`            | Dump HardFault record   |
| `fault_clear`      | Clear stored fault      |
| `adc`              | Latest ADC values       |
| `tasks`            | RTOS task list          |
| `oled <row> <msg>` | Print to OLED           |

📂 Repository Structure
AegisFW/
├── bootloader/
│   ├── Core/
│   |──  Linker/
├── application/
│   ├── Core/
│   └── Linker/
├── common/
└── README.md

🔐 Reliability Philosophy

Failures are expected.

This firmware:

Detects failures

Records diagnostics

Recovers safely

Never bricks the device

Release: v1.0.0

🐞Challenges & Design Changes
🚧 Major Flash Partitioning Challenge (Critical Learning)
❗ Initial Design Mistake: Equal Slot Partitioning

Original Design

Slot-A and Slot-B were each assigned 200 KB

Both slots started contiguously from:

Slot-A base: 0x0800C000
Slot-B base: Slot-A + 200 KB

💥 Observed Failures

This design caused severe and misleading failures, including:

=>Random breakpoints during debugging

=>Execution jumps into invalid addresses

=>HardFaults triggered without consistent patterns

=>CRC failures on otherwise valid images

=>Flash erase operations corrupting adjacent code

=>These failures occurred intermittently, making them extremely difficult to diagnose.

🔍 Root Cause Analysis

The STM32F446 does NOT use uniform flash sectors.

Flash layout (simplified):

Sector	Size	Address Range
0	     16 KB	0x08000000
1	     16 KB	0x08004000
2      16 KB	0x08008000
3      16 KB	0x0800C000
4	     64 KB	0x08010000
5	     128 KB	0x08020000
6	     128 KB	0x08040000
7	     128 KB	0x08060000

Problem

Slot-A’s 200 KB allocation spanned multiple sector types

Slot-B’s start address landed inside Sector 5

Sector-erase operations for Slot-B partially erased Slot-A

This created silent corruption that only surfaced during execution.

🔧 Final Correct Design (Sector-Aligned)

After identifying the issue, slots were redesigned to be sector-aligned:

Slot-A → Sectors 3, 4, 5
Slot-B → Sectors 6, 7


After Fix:

=>No overlapping erase regions

=>Deterministic flash operations

=>Reliable CRC validation

=>Elimination of random HardFaults

=>Debugger behavior stabilized immediately

🚧 Major Challenge 2: RTOS + Interrupt Priority Misconfiguration
❗ Symptom: Random System Freezes & Missed Events

Observed Behavior

EXTI interrupts sometimes stopped triggering tasks

DMA ADC notifications occasionally never reached the ADC task

System appeared “alive” (UART working) but logic stalled

Watchdog resets occurred without obvious faults

These failures were non-deterministic and worsened as more peripherals were enabled.

🔍 Root Cause Analysis

The issue was caused by incorrect NVIC priority configuration in an RTOS environment.

Key mistakes:

Some ISRs were configured with higher priority than FreeRTOS allows

ISR-safe APIs (xQueueSendFromISR, vTaskNotifyGiveFromISR) were called from invalid priority levels

This violated the FreeRTOS rule:

Any ISR that calls RTOS APIs must have a priority numerically equal to or lower than configMAX_SYSCALL_INTERRUPT_PRIORITY.

🔧 Fix Applied

Re-mapped all ISR priorities:

DMA

EXTI

TIM2

Ensured no ISR calling RTOS APIs violated priority rules

Verified behavior under stress conditions

🧠 Key Lesson

RTOS correctness is not just about tasks —
it is equally about interrupt discipline.

This fundamentally improved system stability and predictability.

🚧 Major Challenge 3: Watchdog Reset With No Root Cause Visibility
❗ Symptom: Repeated Resets With No Diagnostics

Observed Behavior

System reset repeatedly via watchdog

No fault message printed

No clear indication of which task failed

Reset cause only showed “Watchdog”

Initially, the watchdog looked useless from a debugging standpoint.

🔍 Root Cause Analysis

Problems identified:

Watchdog was being fed unconditionally

No task-level health visibility

No differentiation between:

startup delays

runtime failures

task deadlocks

This meant the watchdog acted as a blind reset mechanism, not a diagnostic tool.

🔧 Architectural Fix

A Health Monitoring Architecture was introduced:

Each critical task sets a heartbeat flag

HealthMonitor_Task evaluates system liveness

Watchdog is fed only if all heartbeats are valid

Warm-up cycles added to avoid startup false positives

This transformed the watchdog into a controlled recovery mechanism.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
👤 Author

Sandeep N
Embedded Firmware Engineer
Focus: Bootloaders, RTOS, reliability engineering
