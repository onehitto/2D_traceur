################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/cnc_2d/Src/cnc_2d.c 

OBJS += \
./Modules/cnc_2d/Src/cnc_2d.o 

C_DEPS += \
./Modules/cnc_2d/Src/cnc_2d.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/cnc_2d/Src/%.o Modules/cnc_2d/Src/%.su: ../Modules/cnc_2d/Src/%.c Modules/cnc_2d/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/cnc_2d/Inc" -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/drivers/Inc" -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/utility/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modules-2f-cnc_2d-2f-Src

clean-Modules-2f-cnc_2d-2f-Src:
	-$(RM) ./Modules/cnc_2d/Src/cnc_2d.d ./Modules/cnc_2d/Src/cnc_2d.o ./Modules/cnc_2d/Src/cnc_2d.su

.PHONY: clean-Modules-2f-cnc_2d-2f-Src

