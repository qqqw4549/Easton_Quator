################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HARDWARE/NRF2401/NRF2401.c 

OBJS += \
./HARDWARE/NRF2401/NRF2401.o 

C_DEPS += \
./HARDWARE/NRF2401/NRF2401.d 


# Each subdirectory must supply rules for building sources it contributes
HARDWARE/NRF2401/NRF2401.o: ../HARDWARE/NRF2401/NRF2401.c HARDWARE/NRF2401/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../HARDWARE/NRF2401 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HARDWARE-2f-NRF2401

clean-HARDWARE-2f-NRF2401:
	-$(RM) ./HARDWARE/NRF2401/NRF2401.cyclo ./HARDWARE/NRF2401/NRF2401.d ./HARDWARE/NRF2401/NRF2401.o ./HARDWARE/NRF2401/NRF2401.su

.PHONY: clean-HARDWARE-2f-NRF2401

