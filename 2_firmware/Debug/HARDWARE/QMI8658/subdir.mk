################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HARDWARE/QMI8658/qmi8658.c 

OBJS += \
./HARDWARE/QMI8658/qmi8658.o 

C_DEPS += \
./HARDWARE/QMI8658/qmi8658.d 


# Each subdirectory must supply rules for building sources it contributes
HARDWARE/QMI8658/%.o HARDWARE/QMI8658/%.su HARDWARE/QMI8658/%.cyclo: ../HARDWARE/QMI8658/%.c HARDWARE/QMI8658/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Desktop/imu_demo/init/workspace/HARDWARE/NRF2401" -I"D:/Desktop/imu_demo/init/workspace/HARDWARE/I2C_BSP" -I"D:/Desktop/imu_demo/init/workspace/HARDWARE/QMC5883" -I"D:/Desktop/imu_demo/init/workspace/HARDWARE/QMI8658" -I"D:/Desktop/imu_demo/init/workspace/HARDWARE/SPL06" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HARDWARE-2f-QMI8658

clean-HARDWARE-2f-QMI8658:
	-$(RM) ./HARDWARE/QMI8658/qmi8658.cyclo ./HARDWARE/QMI8658/qmi8658.d ./HARDWARE/QMI8658/qmi8658.o ./HARDWARE/QMI8658/qmi8658.su

.PHONY: clean-HARDWARE-2f-QMI8658

