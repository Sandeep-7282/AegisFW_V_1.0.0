################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/fault/fault_record.c \
../Core/Src/fault/hardfault.c 

OBJS += \
./Core/Src/fault/fault_record.o \
./Core/Src/fault/hardfault.o 

C_DEPS += \
./Core/Src/fault/fault_record.d \
./Core/Src/fault/hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/fault/%.o Core/Src/fault/%.su Core/Src/fault/%.cyclo: ../Core/Src/fault/%.c Core/Src/fault/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/rtos -I../Core/Inc/drivers -I../Core/Inc/fault -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-fault

clean-Core-2f-Src-2f-fault:
	-$(RM) ./Core/Src/fault/fault_record.cyclo ./Core/Src/fault/fault_record.d ./Core/Src/fault/fault_record.o ./Core/Src/fault/fault_record.su ./Core/Src/fault/hardfault.cyclo ./Core/Src/fault/hardfault.d ./Core/Src/fault/hardfault.o ./Core/Src/fault/hardfault.su

.PHONY: clean-Core-2f-Src-2f-fault

