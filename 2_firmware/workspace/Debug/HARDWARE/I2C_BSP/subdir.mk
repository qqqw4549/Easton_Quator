################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HARDWARE/I2C_BSP/myiic.c 

OBJS += \
./HARDWARE/I2C_BSP/myiic.o 

C_DEPS += \
./HARDWARE/I2C_BSP/myiic.d 


# Each subdirectory must supply rules for building sources it contributes
HARDWARE/I2C_BSP/%.o HARDWARE/I2C_BSP/%.su HARDWARE/I2C_BSP/%.cyclo: ../HARDWARE/I2C_BSP/%.c HARDWARE/I2C_BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/HARDWARE/NRF2401" -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/HARDWARE/I2C_BSP" -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/HARDWARE/QMC5883" -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/HARDWARE/QMI8658" -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/HARDWARE/SPL06" -I"C:/Users/23886/Desktop/Easton_Quator/2_firmware/workspace/Aircraft/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HARDWARE-2f-I2C_BSP

clean-HARDWARE-2f-I2C_BSP:
	-$(RM) ./HARDWARE/I2C_BSP/myiic.cyclo ./HARDWARE/I2C_BSP/myiic.d ./HARDWARE/I2C_BSP/myiic.o ./HARDWARE/I2C_BSP/myiic.su

.PHONY: clean-HARDWARE-2f-I2C_BSP

