################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/HAL/IR.c \
../APP/HAL/LCD.c \
../APP/HAL/limitSwitch.c \
../APP/HAL/servo.c \
../APP/HAL/stepperMotor.c 

OBJS += \
./APP/HAL/IR.o \
./APP/HAL/LCD.o \
./APP/HAL/limitSwitch.o \
./APP/HAL/servo.o \
./APP/HAL/stepperMotor.o 

C_DEPS += \
./APP/HAL/IR.d \
./APP/HAL/LCD.d \
./APP/HAL/limitSwitch.d \
./APP/HAL/servo.d \
./APP/HAL/stepperMotor.d 


# Each subdirectory must supply rules for building sources it contributes
APP/HAL/%.o: ../APP/HAL/%.c APP/HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


