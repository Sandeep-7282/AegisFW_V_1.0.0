################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app_jump.c \
../Core/Src/boot_decision.c \
../Core/Src/boot_metadata.c \
../Core/Src/cli_commands.c \
../Core/Src/clock.c \
../Core/Src/eeprom.c \
../Core/Src/flash_if.c \
../Core/Src/flash_ops.c \
../Core/Src/image_crc.c \
../Core/Src/image_validate.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uart.c \
../Core/Src/update_uart.c 

OBJS += \
./Core/Src/app_jump.o \
./Core/Src/boot_decision.o \
./Core/Src/boot_metadata.o \
./Core/Src/cli_commands.o \
./Core/Src/clock.o \
./Core/Src/eeprom.o \
./Core/Src/flash_if.o \
./Core/Src/flash_ops.o \
./Core/Src/image_crc.o \
./Core/Src/image_validate.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uart.o \
./Core/Src/update_uart.o 

C_DEPS += \
./Core/Src/app_jump.d \
./Core/Src/boot_decision.d \
./Core/Src/boot_metadata.d \
./Core/Src/cli_commands.d \
./Core/Src/clock.d \
./Core/Src/eeprom.d \
./Core/Src/flash_if.d \
./Core/Src/flash_ops.d \
./Core/Src/image_crc.d \
./Core/Src/image_validate.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uart.d \
./Core/Src/update_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I"D:/Firmware/STM32/AegisFW_Resources/AegisFW - Copy/common/include" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/app_jump.cyclo ./Core/Src/app_jump.d ./Core/Src/app_jump.o ./Core/Src/app_jump.su ./Core/Src/boot_decision.cyclo ./Core/Src/boot_decision.d ./Core/Src/boot_decision.o ./Core/Src/boot_decision.su ./Core/Src/boot_metadata.cyclo ./Core/Src/boot_metadata.d ./Core/Src/boot_metadata.o ./Core/Src/boot_metadata.su ./Core/Src/cli_commands.cyclo ./Core/Src/cli_commands.d ./Core/Src/cli_commands.o ./Core/Src/cli_commands.su ./Core/Src/clock.cyclo ./Core/Src/clock.d ./Core/Src/clock.o ./Core/Src/clock.su ./Core/Src/eeprom.cyclo ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/flash_if.cyclo ./Core/Src/flash_if.d ./Core/Src/flash_if.o ./Core/Src/flash_if.su ./Core/Src/flash_ops.cyclo ./Core/Src/flash_ops.d ./Core/Src/flash_ops.o ./Core/Src/flash_ops.su ./Core/Src/image_crc.cyclo ./Core/Src/image_crc.d ./Core/Src/image_crc.o ./Core/Src/image_crc.su ./Core/Src/image_validate.cyclo ./Core/Src/image_validate.d ./Core/Src/image_validate.o ./Core/Src/image_validate.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su ./Core/Src/update_uart.cyclo ./Core/Src/update_uart.d ./Core/Src/update_uart.o ./Core/Src/update_uart.su

.PHONY: clean-Core-2f-Src

