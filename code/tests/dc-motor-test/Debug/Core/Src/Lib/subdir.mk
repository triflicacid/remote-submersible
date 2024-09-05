################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Lib/action-mgr.c \
../Core/Src/Lib/communication.c \
../Core/Src/Lib/counter.c \
../Core/Src/Lib/dc-motor.c \
../Core/Src/Lib/lora.c \
../Core/Src/Lib/mcp23s17.c \
../Core/Src/Lib/stepper-motor-event.c \
../Core/Src/Lib/stepper-motor.c \
../Core/Src/Lib/stored-code.c \
../Core/Src/Lib/timed-lock.c \
../Core/Src/Lib/tm1637.c \
../Core/Src/Lib/util.c 

OBJS += \
./Core/Src/Lib/action-mgr.o \
./Core/Src/Lib/communication.o \
./Core/Src/Lib/counter.o \
./Core/Src/Lib/dc-motor.o \
./Core/Src/Lib/lora.o \
./Core/Src/Lib/mcp23s17.o \
./Core/Src/Lib/stepper-motor-event.o \
./Core/Src/Lib/stepper-motor.o \
./Core/Src/Lib/stored-code.o \
./Core/Src/Lib/timed-lock.o \
./Core/Src/Lib/tm1637.o \
./Core/Src/Lib/util.o 

C_DEPS += \
./Core/Src/Lib/action-mgr.d \
./Core/Src/Lib/communication.d \
./Core/Src/Lib/counter.d \
./Core/Src/Lib/dc-motor.d \
./Core/Src/Lib/lora.d \
./Core/Src/Lib/mcp23s17.d \
./Core/Src/Lib/stepper-motor-event.d \
./Core/Src/Lib/stepper-motor.d \
./Core/Src/Lib/stored-code.d \
./Core/Src/Lib/timed-lock.d \
./Core/Src/Lib/tm1637.d \
./Core/Src/Lib/util.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Lib/%.o Core/Src/Lib/%.su Core/Src/Lib/%.cyclo: ../Core/Src/Lib/%.c Core/Src/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Lib

clean-Core-2f-Src-2f-Lib:
	-$(RM) ./Core/Src/Lib/action-mgr.cyclo ./Core/Src/Lib/action-mgr.d ./Core/Src/Lib/action-mgr.o ./Core/Src/Lib/action-mgr.su ./Core/Src/Lib/communication.cyclo ./Core/Src/Lib/communication.d ./Core/Src/Lib/communication.o ./Core/Src/Lib/communication.su ./Core/Src/Lib/counter.cyclo ./Core/Src/Lib/counter.d ./Core/Src/Lib/counter.o ./Core/Src/Lib/counter.su ./Core/Src/Lib/dc-motor.cyclo ./Core/Src/Lib/dc-motor.d ./Core/Src/Lib/dc-motor.o ./Core/Src/Lib/dc-motor.su ./Core/Src/Lib/lora.cyclo ./Core/Src/Lib/lora.d ./Core/Src/Lib/lora.o ./Core/Src/Lib/lora.su ./Core/Src/Lib/mcp23s17.cyclo ./Core/Src/Lib/mcp23s17.d ./Core/Src/Lib/mcp23s17.o ./Core/Src/Lib/mcp23s17.su ./Core/Src/Lib/stepper-motor-event.cyclo ./Core/Src/Lib/stepper-motor-event.d ./Core/Src/Lib/stepper-motor-event.o ./Core/Src/Lib/stepper-motor-event.su ./Core/Src/Lib/stepper-motor.cyclo ./Core/Src/Lib/stepper-motor.d ./Core/Src/Lib/stepper-motor.o ./Core/Src/Lib/stepper-motor.su ./Core/Src/Lib/stored-code.cyclo ./Core/Src/Lib/stored-code.d ./Core/Src/Lib/stored-code.o ./Core/Src/Lib/stored-code.su ./Core/Src/Lib/timed-lock.cyclo ./Core/Src/Lib/timed-lock.d ./Core/Src/Lib/timed-lock.o ./Core/Src/Lib/timed-lock.su ./Core/Src/Lib/tm1637.cyclo ./Core/Src/Lib/tm1637.d ./Core/Src/Lib/tm1637.o ./Core/Src/Lib/tm1637.su ./Core/Src/Lib/util.cyclo ./Core/Src/Lib/util.d ./Core/Src/Lib/util.o ./Core/Src/Lib/util.su

.PHONY: clean-Core-2f-Src-2f-Lib

