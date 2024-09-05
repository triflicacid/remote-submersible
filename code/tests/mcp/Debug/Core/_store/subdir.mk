################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/_store/7-segment.c \
../Core/_store/mcp23s17.c 

OBJS += \
./Core/_store/7-segment.o \
./Core/_store/mcp23s17.o 

C_DEPS += \
./Core/_store/7-segment.d \
./Core/_store/mcp23s17.d 


# Each subdirectory must supply rules for building sources it contributes
Core/_store/%.o Core/_store/%.su Core/_store/%.cyclo: ../Core/_store/%.c Core/_store/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-_store

clean-Core-2f-_store:
	-$(RM) ./Core/_store/7-segment.cyclo ./Core/_store/7-segment.d ./Core/_store/7-segment.o ./Core/_store/7-segment.su ./Core/_store/mcp23s17.cyclo ./Core/_store/mcp23s17.d ./Core/_store/mcp23s17.o ./Core/_store/mcp23s17.su

.PHONY: clean-Core-2f-_store

