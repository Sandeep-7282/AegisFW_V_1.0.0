################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/I2c.c \
../Core/Src/drivers/adc.c \
../Core/Src/drivers/cli_commands.c \
../Core/Src/drivers/dma.c \
../Core/Src/drivers/dwt.c \
../Core/Src/drivers/font_5x7.c \
../Core/Src/drivers/gpio_driver.c \
../Core/Src/drivers/gpio_exti.c \
../Core/Src/drivers/oled.c \
../Core/Src/drivers/reset_cause.c \
../Core/Src/drivers/timer2.c \
../Core/Src/drivers/uart_driver.c \
../Core/Src/drivers/watchdog.c 

OBJS += \
./Core/Src/drivers/I2c.o \
./Core/Src/drivers/adc.o \
./Core/Src/drivers/cli_commands.o \
./Core/Src/drivers/dma.o \
./Core/Src/drivers/dwt.o \
./Core/Src/drivers/font_5x7.o \
./Core/Src/drivers/gpio_driver.o \
./Core/Src/drivers/gpio_exti.o \
./Core/Src/drivers/oled.o \
./Core/Src/drivers/reset_cause.o \
./Core/Src/drivers/timer2.o \
./Core/Src/drivers/uart_driver.o \
./Core/Src/drivers/watchdog.o 

C_DEPS += \
./Core/Src/drivers/I2c.d \
./Core/Src/drivers/adc.d \
./Core/Src/drivers/cli_commands.d \
./Core/Src/drivers/dma.d \
./Core/Src/drivers/dwt.d \
./Core/Src/drivers/font_5x7.d \
./Core/Src/drivers/gpio_driver.d \
./Core/Src/drivers/gpio_exti.d \
./Core/Src/drivers/oled.d \
./Core/Src/drivers/reset_cause.d \
./Core/Src/drivers/timer2.d \
./Core/Src/drivers/uart_driver.d \
./Core/Src/drivers/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/%.o Core/Src/drivers/%.su Core/Src/drivers/%.cyclo: ../Core/Src/drivers/%.c Core/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/rtos -I../Core/Inc/drivers -I../Core/Inc/fault -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers

clean-Core-2f-Src-2f-drivers:
	-$(RM) ./Core/Src/drivers/I2c.cyclo ./Core/Src/drivers/I2c.d ./Core/Src/drivers/I2c.o ./Core/Src/drivers/I2c.su ./Core/Src/drivers/adc.cyclo ./Core/Src/drivers/adc.d ./Core/Src/drivers/adc.o ./Core/Src/drivers/adc.su ./Core/Src/drivers/cli_commands.cyclo ./Core/Src/drivers/cli_commands.d ./Core/Src/drivers/cli_commands.o ./Core/Src/drivers/cli_commands.su ./Core/Src/drivers/dma.cyclo ./Core/Src/drivers/dma.d ./Core/Src/drivers/dma.o ./Core/Src/drivers/dma.su ./Core/Src/drivers/dwt.cyclo ./Core/Src/drivers/dwt.d ./Core/Src/drivers/dwt.o ./Core/Src/drivers/dwt.su ./Core/Src/drivers/font_5x7.cyclo ./Core/Src/drivers/font_5x7.d ./Core/Src/drivers/font_5x7.o ./Core/Src/drivers/font_5x7.su ./Core/Src/drivers/gpio_driver.cyclo ./Core/Src/drivers/gpio_driver.d ./Core/Src/drivers/gpio_driver.o ./Core/Src/drivers/gpio_driver.su ./Core/Src/drivers/gpio_exti.cyclo ./Core/Src/drivers/gpio_exti.d ./Core/Src/drivers/gpio_exti.o ./Core/Src/drivers/gpio_exti.su ./Core/Src/drivers/oled.cyclo ./Core/Src/drivers/oled.d ./Core/Src/drivers/oled.o ./Core/Src/drivers/oled.su ./Core/Src/drivers/reset_cause.cyclo ./Core/Src/drivers/reset_cause.d ./Core/Src/drivers/reset_cause.o ./Core/Src/drivers/reset_cause.su ./Core/Src/drivers/timer2.cyclo ./Core/Src/drivers/timer2.d ./Core/Src/drivers/timer2.o ./Core/Src/drivers/timer2.su ./Core/Src/drivers/uart_driver.cyclo ./Core/Src/drivers/uart_driver.d ./Core/Src/drivers/uart_driver.o ./Core/Src/drivers/uart_driver.su ./Core/Src/drivers/watchdog.cyclo ./Core/Src/drivers/watchdog.d ./Core/Src/drivers/watchdog.o ./Core/Src/drivers/watchdog.su

.PHONY: clean-Core-2f-Src-2f-drivers

