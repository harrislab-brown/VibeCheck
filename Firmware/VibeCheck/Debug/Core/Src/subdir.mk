################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LSM6DS3.c \
../Core/Src/fast_spi.c \
../Core/Src/main.c \
../Core/Src/sequencer.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c \
../Core/Src/util.c \
../Core/Src/vibecheck.c \
../Core/Src/vibecheck_button_handler.c \
../Core/Src/vibecheck_connect_handler.c \
../Core/Src/vibecheck_rgb.c \
../Core/Src/vibecheck_rgb_handler.c \
../Core/Src/vibecheck_sensor.c \
../Core/Src/vibecheck_sensor_handler.c \
../Core/Src/vibecheck_shell.c \
../Core/Src/vibecheck_strobe.c \
../Core/Src/vibecheck_strobe_handler.c \
../Core/Src/vibecheck_wavegen.c \
../Core/Src/vibecheck_wavegen_handler.c 

OBJS += \
./Core/Src/LSM6DS3.o \
./Core/Src/fast_spi.o \
./Core/Src/main.o \
./Core/Src/sequencer.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o \
./Core/Src/util.o \
./Core/Src/vibecheck.o \
./Core/Src/vibecheck_button_handler.o \
./Core/Src/vibecheck_connect_handler.o \
./Core/Src/vibecheck_rgb.o \
./Core/Src/vibecheck_rgb_handler.o \
./Core/Src/vibecheck_sensor.o \
./Core/Src/vibecheck_sensor_handler.o \
./Core/Src/vibecheck_shell.o \
./Core/Src/vibecheck_strobe.o \
./Core/Src/vibecheck_strobe_handler.o \
./Core/Src/vibecheck_wavegen.o \
./Core/Src/vibecheck_wavegen_handler.o 

C_DEPS += \
./Core/Src/LSM6DS3.d \
./Core/Src/fast_spi.d \
./Core/Src/main.d \
./Core/Src/sequencer.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d \
./Core/Src/util.d \
./Core/Src/vibecheck.d \
./Core/Src/vibecheck_button_handler.d \
./Core/Src/vibecheck_connect_handler.d \
./Core/Src/vibecheck_rgb.d \
./Core/Src/vibecheck_rgb_handler.d \
./Core/Src/vibecheck_sensor.d \
./Core/Src/vibecheck_sensor_handler.d \
./Core/Src/vibecheck_shell.d \
./Core/Src/vibecheck_strobe.d \
./Core/Src/vibecheck_strobe_handler.d \
./Core/Src/vibecheck_wavegen.d \
./Core/Src/vibecheck_wavegen_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/LSM6DS3.cyclo ./Core/Src/LSM6DS3.d ./Core/Src/LSM6DS3.o ./Core/Src/LSM6DS3.su ./Core/Src/fast_spi.cyclo ./Core/Src/fast_spi.d ./Core/Src/fast_spi.o ./Core/Src/fast_spi.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sequencer.cyclo ./Core/Src/sequencer.d ./Core/Src/sequencer.o ./Core/Src/sequencer.su ./Core/Src/stm32h7xx_hal_msp.cyclo ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.cyclo ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32h7xx.cyclo ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o ./Core/Src/system_stm32h7xx.su ./Core/Src/util.cyclo ./Core/Src/util.d ./Core/Src/util.o ./Core/Src/util.su ./Core/Src/vibecheck.cyclo ./Core/Src/vibecheck.d ./Core/Src/vibecheck.o ./Core/Src/vibecheck.su ./Core/Src/vibecheck_button_handler.cyclo ./Core/Src/vibecheck_button_handler.d ./Core/Src/vibecheck_button_handler.o ./Core/Src/vibecheck_button_handler.su ./Core/Src/vibecheck_connect_handler.cyclo ./Core/Src/vibecheck_connect_handler.d ./Core/Src/vibecheck_connect_handler.o ./Core/Src/vibecheck_connect_handler.su ./Core/Src/vibecheck_rgb.cyclo ./Core/Src/vibecheck_rgb.d ./Core/Src/vibecheck_rgb.o ./Core/Src/vibecheck_rgb.su ./Core/Src/vibecheck_rgb_handler.cyclo ./Core/Src/vibecheck_rgb_handler.d ./Core/Src/vibecheck_rgb_handler.o ./Core/Src/vibecheck_rgb_handler.su ./Core/Src/vibecheck_sensor.cyclo ./Core/Src/vibecheck_sensor.d ./Core/Src/vibecheck_sensor.o ./Core/Src/vibecheck_sensor.su ./Core/Src/vibecheck_sensor_handler.cyclo ./Core/Src/vibecheck_sensor_handler.d ./Core/Src/vibecheck_sensor_handler.o ./Core/Src/vibecheck_sensor_handler.su ./Core/Src/vibecheck_shell.cyclo ./Core/Src/vibecheck_shell.d ./Core/Src/vibecheck_shell.o ./Core/Src/vibecheck_shell.su ./Core/Src/vibecheck_strobe.cyclo ./Core/Src/vibecheck_strobe.d ./Core/Src/vibecheck_strobe.o ./Core/Src/vibecheck_strobe.su ./Core/Src/vibecheck_strobe_handler.cyclo ./Core/Src/vibecheck_strobe_handler.d ./Core/Src/vibecheck_strobe_handler.o ./Core/Src/vibecheck_strobe_handler.su ./Core/Src/vibecheck_wavegen.cyclo ./Core/Src/vibecheck_wavegen.d ./Core/Src/vibecheck_wavegen.o ./Core/Src/vibecheck_wavegen.su ./Core/Src/vibecheck_wavegen_handler.cyclo ./Core/Src/vibecheck_wavegen_handler.d ./Core/Src/vibecheck_wavegen_handler.o ./Core/Src/vibecheck_wavegen_handler.su

.PHONY: clean-Core-2f-Src

