################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/rtos/freertos_init.c \
../Core/Src/rtos/health_manager.c \
../Core/Src/rtos/idle_hook.c \
../Core/Src/rtos/task_adc.c \
../Core/Src/rtos/task_cli.c \
../Core/Src/rtos/task_oled.c \
../Core/Src/rtos/task_sensors.c \
../Core/Src/rtos/task_start.c \
../Core/Src/rtos/tasks_common.c 

OBJS += \
./Core/Src/rtos/freertos_init.o \
./Core/Src/rtos/health_manager.o \
./Core/Src/rtos/idle_hook.o \
./Core/Src/rtos/task_adc.o \
./Core/Src/rtos/task_cli.o \
./Core/Src/rtos/task_oled.o \
./Core/Src/rtos/task_sensors.o \
./Core/Src/rtos/task_start.o \
./Core/Src/rtos/tasks_common.o 

C_DEPS += \
./Core/Src/rtos/freertos_init.d \
./Core/Src/rtos/health_manager.d \
./Core/Src/rtos/idle_hook.d \
./Core/Src/rtos/task_adc.d \
./Core/Src/rtos/task_cli.d \
./Core/Src/rtos/task_oled.d \
./Core/Src/rtos/task_sensors.d \
./Core/Src/rtos/task_start.d \
./Core/Src/rtos/tasks_common.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/rtos/%.o Core/Src/rtos/%.su Core/Src/rtos/%.cyclo: ../Core/Src/rtos/%.c Core/Src/rtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/rtos -I../Core/Inc/drivers -I../Core/Inc/fault -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-rtos

clean-Core-2f-Src-2f-rtos:
	-$(RM) ./Core/Src/rtos/freertos_init.cyclo ./Core/Src/rtos/freertos_init.d ./Core/Src/rtos/freertos_init.o ./Core/Src/rtos/freertos_init.su ./Core/Src/rtos/health_manager.cyclo ./Core/Src/rtos/health_manager.d ./Core/Src/rtos/health_manager.o ./Core/Src/rtos/health_manager.su ./Core/Src/rtos/idle_hook.cyclo ./Core/Src/rtos/idle_hook.d ./Core/Src/rtos/idle_hook.o ./Core/Src/rtos/idle_hook.su ./Core/Src/rtos/task_adc.cyclo ./Core/Src/rtos/task_adc.d ./Core/Src/rtos/task_adc.o ./Core/Src/rtos/task_adc.su ./Core/Src/rtos/task_cli.cyclo ./Core/Src/rtos/task_cli.d ./Core/Src/rtos/task_cli.o ./Core/Src/rtos/task_cli.su ./Core/Src/rtos/task_oled.cyclo ./Core/Src/rtos/task_oled.d ./Core/Src/rtos/task_oled.o ./Core/Src/rtos/task_oled.su ./Core/Src/rtos/task_sensors.cyclo ./Core/Src/rtos/task_sensors.d ./Core/Src/rtos/task_sensors.o ./Core/Src/rtos/task_sensors.su ./Core/Src/rtos/task_start.cyclo ./Core/Src/rtos/task_start.d ./Core/Src/rtos/task_start.o ./Core/Src/rtos/task_start.su ./Core/Src/rtos/tasks_common.cyclo ./Core/Src/rtos/tasks_common.d ./Core/Src/rtos/tasks_common.o ./Core/Src/rtos/tasks_common.su

.PHONY: clean-Core-2f-Src-2f-rtos

