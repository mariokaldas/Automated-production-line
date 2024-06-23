################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/HAL/MCAL_ATMEGA32/gpio.c \
../APP/HAL/MCAL_ATMEGA32/timer1.c 

OBJS += \
./APP/HAL/MCAL_ATMEGA32/gpio.o \
./APP/HAL/MCAL_ATMEGA32/timer1.o 

C_DEPS += \
./APP/HAL/MCAL_ATMEGA32/gpio.d \
./APP/HAL/MCAL_ATMEGA32/timer1.d 


# Each subdirectory must supply rules for building sources it contributes
APP/HAL/MCAL_ATMEGA32/%.o: ../APP/HAL/MCAL_ATMEGA32/%.c APP/HAL/MCAL_ATMEGA32/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


