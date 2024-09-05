################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Custom/actions.c \
../Core/Src/Custom/main.c \
../Core/Src/Custom/pins.c 

OBJS += \
./Core/Src/Custom/actions.o \
./Core/Src/Custom/main.o \
./Core/Src/Custom/pins.o 

C_DEPS += \
./Core/Src/Custom/actions.d \
./Core/Src/Custom/main.d \
./Core/Src/Custom/pins.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Custom/%.o Core/Src/Custom/%.su Core/Src/Custom/%.cyclo: ../Core/Src/Custom/%.c Core/Src/Custom/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Custom

clean-Core-2f-Src-2f-Custom:
	-$(RM) ./Core/Src/Custom/actions.cyclo ./Core/Src/Custom/actions.d ./Core/Src/Custom/actions.o ./Core/Src/Custom/actions.su ./Core/Src/Custom/main.cyclo ./Core/Src/Custom/main.d ./Core/Src/Custom/main.o ./Core/Src/Custom/main.su ./Core/Src/Custom/pins.cyclo ./Core/Src/Custom/pins.d ./Core/Src/Custom/pins.o ./Core/Src/Custom/pins.su

.PHONY: clean-Core-2f-Src-2f-Custom

