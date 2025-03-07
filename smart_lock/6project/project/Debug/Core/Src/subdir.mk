################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/as608.c \
../Core/Src/fsmc.c \
../Core/Src/gpio.c \
../Core/Src/lcdtp.c \
../Core/Src/main.c \
../Core/Src/mylibrary.c \
../Core/Src/pn532.c \
../Core/Src/pn532_stm32f1.c \
../Core/Src/rtc.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syn6288.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/xpt2046.c 

OBJS += \
./Core/Src/as608.o \
./Core/Src/fsmc.o \
./Core/Src/gpio.o \
./Core/Src/lcdtp.o \
./Core/Src/main.o \
./Core/Src/mylibrary.o \
./Core/Src/pn532.o \
./Core/Src/pn532_stm32f1.o \
./Core/Src/rtc.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syn6288.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/xpt2046.o 

C_DEPS += \
./Core/Src/as608.d \
./Core/Src/fsmc.d \
./Core/Src/gpio.d \
./Core/Src/lcdtp.d \
./Core/Src/main.d \
./Core/Src/mylibrary.d \
./Core/Src/pn532.d \
./Core/Src/pn532_stm32f1.d \
./Core/Src/rtc.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syn6288.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/xpt2046.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/as608.d ./Core/Src/as608.o ./Core/Src/as608.su ./Core/Src/fsmc.d ./Core/Src/fsmc.o ./Core/Src/fsmc.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/lcdtp.d ./Core/Src/lcdtp.o ./Core/Src/lcdtp.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mylibrary.d ./Core/Src/mylibrary.o ./Core/Src/mylibrary.su ./Core/Src/pn532.d ./Core/Src/pn532.o ./Core/Src/pn532.su ./Core/Src/pn532_stm32f1.d ./Core/Src/pn532_stm32f1.o ./Core/Src/pn532_stm32f1.su ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syn6288.d ./Core/Src/syn6288.o ./Core/Src/syn6288.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/xpt2046.d ./Core/Src/xpt2046.o ./Core/Src/xpt2046.su

.PHONY: clean-Core-2f-Src

