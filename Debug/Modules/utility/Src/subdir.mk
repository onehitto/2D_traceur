################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/utility/Src/Com_api.c \
../Modules/utility/Src/G_code.c \
../Modules/utility/Src/Queue_job_manager.c 

OBJS += \
./Modules/utility/Src/Com_api.o \
./Modules/utility/Src/G_code.o \
./Modules/utility/Src/Queue_job_manager.o 

C_DEPS += \
./Modules/utility/Src/Com_api.d \
./Modules/utility/Src/G_code.d \
./Modules/utility/Src/Queue_job_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/utility/Src/%.o Modules/utility/Src/%.su: ../Modules/utility/Src/%.c Modules/utility/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/cnc_2d/Inc" -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/drivers/Inc" -I"C:/Users/BOUHROUZ FADOUA/Documents/stm_projects/2D_traceur/Modules/utility/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modules-2f-utility-2f-Src

clean-Modules-2f-utility-2f-Src:
	-$(RM) ./Modules/utility/Src/Com_api.d ./Modules/utility/Src/Com_api.o ./Modules/utility/Src/Com_api.su ./Modules/utility/Src/G_code.d ./Modules/utility/Src/G_code.o ./Modules/utility/Src/G_code.su ./Modules/utility/Src/Queue_job_manager.d ./Modules/utility/Src/Queue_job_manager.o ./Modules/utility/Src/Queue_job_manager.su

.PHONY: clean-Modules-2f-utility-2f-Src

