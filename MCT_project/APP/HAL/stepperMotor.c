/*
 * stepperMotor.c
 *
 *  Created on: Apr 15, 2024
 *      Author: mario
 */

/******************************************************************************************
 *                                          INCLUDES
 ******************************************************************************************/

#include"stepperMotor.h"
#include<util/delay.h>

/******************************************************************************************
 *                                    FUNCTIONS DEFINITION
 ******************************************************************************************/

void STEPPERMOTOR_init(uint8 motor_number){

	GPIO_setupPinDirection(STEPPERMOTOR_PORT, motor_number, PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPERMOTOR_PORT, motor_number + 1, PIN_OUTPUT);
}

void STEPPERMOTOR_steps(uint8 motor_number ,uint32 steps,STEPPERMOTOR_directionType direction){

	uint32 i;

	GPIO_writePin(STEPPERMOTOR_PORT,motor_number+1,direction);

	for(i=0; i<steps; i++ ){

		GPIO_writePin(STEPPERMOTOR_PORT,motor_number,LOW);
		GPIO_writePin(STEPPERMOTOR_PORT,motor_number,HIGH);
		_delay_us(1);
	}
}

void STEPPERMOTOR_moveTillLS(uint8 motor_number,STEPPERMOTOR_directionType direction,uint8 LS_number){

	GPIO_writePin(STEPPERMOTOR_PORT,motor_number+1,direction);

	for(;;){

		if(LS_getReading(LS_number) == LS_OBJECT_DETECTED)
			return;

		GPIO_writePin(STEPPERMOTOR_PORT,motor_number,LOW);
		GPIO_writePin(STEPPERMOTOR_PORT,motor_number,HIGH);
		_delay_us(1);
	}
}
