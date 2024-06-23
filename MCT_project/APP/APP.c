/*
 * APP.c
 *
 *  Created on: Apr 18, 2024
 *      Author: mario
 */


/****************************************************************************************
 *                     						Includes
 * ***************************************************************************************/

#include"APP.h"
#include"HAL/limitSwitch.h"
#include"HAL/servo.h"
#include"HAL/stepperMotor.h"
#include"HAL/IR.h"
#include"HAL/LCD.h"

#include<util/delay.h>


/****************************************************************************************
 *                     					Types declaration
 * ***************************************************************************************/



/****************************************************************************************
 *                  				   Global variables
 * ***************************************************************************************/

enum states currentState;
enum products currentProduct;

uint8 parts_counter=0;

uint32 currentXsteps=0;
uint32 currentYsteps=0;

union storePositions_flags store_flag;
union assemblyPositions_flags assembly_flag;

/* This position will only contain green assembled product */
struct position store1_Green={XPOS1,YPOS1};

/* This position will only contain green assembled product */
struct position store2_Green={XPOS2,YPOS1};

/* This position will only contain BLUE assembled product */
struct position store3_BLUE={XPOS1,YPOS2};

/* This position will only contain BLUE assembled product */
struct position store4_BLUE={XPOS2,YPOS2};

/* This position will only contain green base */
struct position assembly1_GreenBase={XPOS1,YPOS3};

/* This position will contain green LID or green BASE*/
struct position assembly2_Green={XPOS2,YPOS3};

/* This position will only contain green LID */
struct position assembly3_GreenLID={XPOS3,YPOS3};

/* This position will only contain BLUE base */
struct position assembly4_BLUEBase={XPOS1,YPOS4};

/* This position will contain BLUE LID or blue base*/
struct position assembly5_BLUE={XPOS2,YPOS4};

/* This position will only contain BLUE LID */
struct position assembly6_BLUELID={XPOS3,YPOS4};


/****************************************************************************************
 *                     				 Functions prototype
 * ***************************************************************************************/

void home();
void moveToPosition(struct position pos);
void pickUp(enum positons_steps posOfZ);
void release(enum positons_steps posOfZ);

/****************************************************************************************
 *                     				 Functions definitions
 * ***************************************************************************************/

void APP_init(void){

	LS_init(LS_XHOME);
	LS_init(LS_YHOME);
	LS_init(LS_ZHOME);

	IR_init(IR_GRIPPER);

	/* The following two lines are NOT sensors go to IR.h for more explanation */
	IR_init(A0);
	IR_init(A1);
	IR_init(A2);

	SERVO_init();

	STEPPERMOTOR_init(STEPPERX);
	STEPPERMOTOR_init(STEPPERY);
	STEPPERMOTOR_init(STEPPERZ);

	LCD_init();

	currentState = WAIT;

}


void APP_wait(){

	LCD_clearScreen();
	LCD_displayStringRowColumn("WAITING",1,1);
	LCD_displayStringRowColumn("state",2,1);

	home();
	STEPPERMOTOR_steps(STEPPERX,XOBJECT,STEPPER_CCW);
	STEPPERMOTOR_steps(STEPPERY,YOBJECT,STEPPER_CCW);
	currentXsteps = XOBJECT;
	currentYsteps = YOBJECT;
	SERVO_angle(SERVO_GRIPPER,SERVO_GRIPPER_OPEN);


	while(!(IR_getReading(A0) || IR_getReading(A1) || IR_getReading(A2)));

	while(1){

		if(IR_getReading(A0) && IR_getReading(A1) && !(IR_getReading(A2))){

			currentProduct = GREEN_BASE;
			break;
		}
		else if(IR_getReading(A0) && !(IR_getReading(A1)) && IR_getReading(A2)){

			currentProduct = GREEN_LID;
			break;
		}
		else if(!(IR_getReading(A0)) && IR_getReading(A1) && IR_getReading(A2)){

			currentProduct = BLUE_BASE;
			break;
		}
		else if(IR_getReading(A0) && IR_getReading(A1) && IR_getReading(A2)){

			currentProduct = BLUE_LID;
			break;
		}
	}
	parts_counter++;

	if(parts_counter == 9){
		parts_counter = 1;
		store_flag.store = 0;
	}

	//	/*
	//	 * This is confirmation pin, it will be used for sending confirmation for MCU which sent
	//	 * previous signals to remove those signals for a new product
	//	 *  */
	//	GPIO_writePin(PORTC_ID,CONFIRMATION,HIGH);
	//	_delay_ms(1000);
	//	GPIO_writePin(PORTC_ID,CONFIRMATION,LOW);

	currentState = ASSEMBLY;

}

void APP_assembly(){

	uint8 i;

	LCD_clearScreen();
	LCD_displayStringRowColumn("ASSEMBLY",1,1);
	LCD_displayStringRowColumn("state",2,1);


	switch(currentProduct){

	case GREEN_BASE:

		LCD_clearScreen();
		LCD_displayStringRowColumn("green base",1,1);

		if(assembly_flag.assembly_flags.assembly1_GreenBase == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly1_GreenBase);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly1_GreenBase = 1;

			if(assembly_flag.assembly_flags.assembly3_GreenLID==1){

				moveToPosition(assembly3_GreenLID);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly1_GreenBase);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly3_GreenLID = 0;
				store_flag.store_flags.greenProduct_assembled1 = 1;

				currentState = STORE;
			}
			else if(assembly_flag.assembly_flags.assembly2_Green==1){

				moveToPosition(assembly2_Green);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly1_GreenBase);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly2_Green = 0;
				store_flag.store_flags.greenProduct_assembled1 = 1;

				currentState = STORE;
			}
			else{

				currentState = WAIT;
			}
		}
		else if(assembly_flag.assembly_flags.assembly2_Green == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly2_Green);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly2_Green = 2;

			if(assembly_flag.assembly_flags.assembly3_GreenLID==1){

				moveToPosition(assembly3_GreenLID);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly2_Green);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly3_GreenLID = 0;
				store_flag.store_flags.greenProduct_assembled2 = 1;

				currentState = STORE;
			}
			else{

				currentState = WAIT;
			}
		}
		break;

	case GREEN_LID:

		LCD_clearScreen();
		LCD_displayStringRowColumn("green lid",1,1);

		if(assembly_flag.assembly_flags.assembly1_GreenBase == 1){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly1_GreenBase);
			release(ZPOS_BASEHEIGHT);

			store_flag.store_flags.greenProduct_assembled1 = 1;
			currentState = STORE;
		}
		else if(assembly_flag.assembly_flags.assembly2_Green == 2){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly2_Green);
			release(ZPOS_BASEHEIGHT);

			store_flag.store_flags.greenProduct_assembled2 = 1;
			currentState = STORE;
		}
		else if(assembly_flag.assembly_flags.assembly3_GreenLID == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly3_GreenLID);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly3_GreenLID = 1;

			currentState = WAIT;
		}
		else if(assembly_flag.assembly_flags.assembly2_Green == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly2_Green);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly2_Green = 1;

			currentState = WAIT;
		}
		break;

	case BLUE_BASE:

		LCD_clearScreen();
		LCD_displayStringRowColumn("blue base",1,1);

		if(assembly_flag.assembly_flags.assembly4_blueBase == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly4_BLUEBase);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly4_blueBase = 1;

			if(assembly_flag.assembly_flags.assembly6_blueLID==1){

				moveToPosition(assembly6_BLUELID);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly4_BLUEBase);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly6_blueLID = 0;
				store_flag.store_flags.blueProduct_assembled1 = 1;

				currentState = STORE;
			}
			else if(assembly_flag.assembly_flags.assembly5_blue==1){

				moveToPosition(assembly5_BLUE);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly4_BLUEBase);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly5_blue = 0;
				store_flag.store_flags.blueProduct_assembled1 = 1;

				currentState = STORE;
			}
			else{

				currentState = WAIT;
			}
		}
		else if(assembly_flag.assembly_flags.assembly5_blue == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly5_BLUE);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly5_blue = 2;

			if(assembly_flag.assembly_flags.assembly6_blueLID==1){

				moveToPosition(assembly6_BLUELID);
				pickUp(ZPOS_LOW);
				moveToPosition(assembly5_BLUE);
				release(ZPOS_BASEHEIGHT);

				assembly_flag.assembly_flags.assembly6_blueLID = 0;
				store_flag.store_flags.blueProduct_assembled2 = 1;

				currentState = STORE;
			}
			else{

				currentState = WAIT;
			}
		}
		break;


	case BLUE_LID:

		LCD_clearScreen();
		LCD_displayStringRowColumn("blue lid",1,1);

		if(assembly_flag.assembly_flags.assembly4_blueBase == 1){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly4_BLUEBase);
			release(ZPOS_BASEHEIGHT);

			store_flag.store_flags.blueProduct_assembled1 = 1;
			currentState = STORE;
		}
		else if(assembly_flag.assembly_flags.assembly5_blue == 2){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly5_BLUE);
			release(ZPOS_BASEHEIGHT);

			store_flag.store_flags.blueProduct_assembled2 = 1;
			currentState = STORE;
		}
		else if(assembly_flag.assembly_flags.assembly6_blueLID == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly6_BLUELID);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly6_blueLID = 1;

			currentState = WAIT;
		}
		else if(assembly_flag.assembly_flags.assembly5_blue == 0){

			pickUp(ZPOS_CONVEYER);
			moveToPosition(assembly5_BLUE);
			release(ZPOS_LOW);
			assembly_flag.assembly_flags.assembly5_blue = 1;

			currentState = WAIT;
		}
		break;
	}
}

void APP_store(){

	LCD_clearScreen();
	LCD_displayStringRowColumn("STORE",1,1);
	LCD_displayStringRowColumn("state",2,1);

	if(store_flag.store_flags.greenProduct_assembled1 == 1){

		if(store_flag.store_flags.store1_Green == 0){

			moveToPosition(assembly1_GreenBase);
			pickUp(ZPOS_LOW);
			moveToPosition(store1_Green);
			release(ZPOS_LOW);

			store_flag.store_flags.store1_Green = 1;
		}
		else if(store_flag.store_flags.store2_Green == 0){

			moveToPosition(assembly1_GreenBase);
			pickUp(ZPOS_LOW);
			moveToPosition(store2_Green);
			release(ZPOS_LOW);

			store_flag.store_flags.store2_Green = 1;
		}

		store_flag.store_flags.greenProduct_assembled1 = 0;
		assembly_flag.assembly_flags.assembly1_GreenBase =0;
		currentState = WAIT;
	}
	else if(store_flag.store_flags.greenProduct_assembled2 == 1){

		if(store_flag.store_flags.store1_Green == 0){

			moveToPosition(assembly2_Green);
			pickUp(ZPOS_LOW);
			moveToPosition(store1_Green);
			release(ZPOS_LOW);

			store_flag.store_flags.store1_Green = 1;
		}
		else if(store_flag.store_flags.store2_Green == 0){

			moveToPosition(assembly2_Green);
			pickUp(ZPOS_LOW);
			moveToPosition(store2_Green);
			release(ZPOS_LOW);

			store_flag.store_flags.store2_Green = 1;
		}

		assembly_flag.assembly_flags.assembly2_Green =0;
		store_flag.store_flags.greenProduct_assembled2 = 0;
		currentState = WAIT;
	}

	else if(store_flag.store_flags.blueProduct_assembled1 == 1){

		if(store_flag.store_flags.store3_blue == 0){

			moveToPosition(assembly4_BLUEBase);
			pickUp(ZPOS_LOW);
			moveToPosition(store3_BLUE);
			release(ZPOS_LOW);

			store_flag.store_flags.store3_blue = 1;
		}
		else if(store_flag.store_flags.store4_blue == 0){

			moveToPosition(assembly4_BLUEBase);
			pickUp(ZPOS_LOW);
			moveToPosition(store4_BLUE);
			release(ZPOS_LOW);

			store_flag.store_flags.store4_blue = 1;
		}

		store_flag.store_flags.blueProduct_assembled1 = 0;
		assembly_flag.assembly_flags.assembly4_blueBase =0;
		currentState = WAIT;
	}

	else if(store_flag.store_flags.blueProduct_assembled2 == 1){

		if(store_flag.store_flags.store3_blue == 0){

			moveToPosition(assembly5_BLUE);
			pickUp(ZPOS_LOW);
			moveToPosition(store3_BLUE);
			release(ZPOS_LOW);

			store_flag.store_flags.store3_blue = 1;
		}
		else if(store_flag.store_flags.store4_blue == 0){

			moveToPosition(assembly5_BLUE);
			pickUp(ZPOS_LOW);
			moveToPosition(store4_BLUE);
			release(ZPOS_LOW);

			store_flag.store_flags.store4_blue = 1;
		}

		store_flag.store_flags.blueProduct_assembled2 = 0;
		assembly_flag.assembly_flags.assembly5_blue =0;
		currentState = WAIT;
	}
}

void home(){

	if(currentXsteps == XPOS3){

		STEPPERMOTOR_steps(STEPPERX,XPOS3/2,STEPPER_CW);
		STEPPERMOTOR_moveTillLS(STEPPERZ,STEPPER_CCW,LS_ZHOME);
		STEPPERMOTOR_moveTillLS(STEPPERY,STEPPER_CW,LS_YHOME);
		STEPPERMOTOR_moveTillLS(STEPPERX,STEPPER_CW,LS_XHOME);
	}
	else{
		STEPPERMOTOR_moveTillLS(STEPPERZ,STEPPER_CCW,LS_ZHOME);
		STEPPERMOTOR_moveTillLS(STEPPERY,STEPPER_CW,LS_YHOME);
		STEPPERMOTOR_moveTillLS(STEPPERX,STEPPER_CW,LS_XHOME);
	}
}

void moveToPosition(struct position pos){

	if((currentXsteps == XOBJECT)&&pos.y==YPOS4){

		moveToPosition(assembly1_GreenBase);
	}

	if(pos.y > currentYsteps){

		STEPPERMOTOR_steps(STEPPERY,(pos.y - currentYsteps),STEPPER_CCW);
		currentYsteps = pos.y;
	}
	else if(pos.y < currentYsteps){
		STEPPERMOTOR_steps(STEPPERY,(currentYsteps - pos.y),STEPPER_CW);
		currentYsteps = pos.y;
	}

	if(pos.x > currentXsteps){

		STEPPERMOTOR_steps(STEPPERX,(pos.x - currentXsteps),STEPPER_CCW);
		currentXsteps = pos.x;
	}
	else if(pos.x < currentXsteps){
		STEPPERMOTOR_steps(STEPPERX,(currentXsteps - pos.x),STEPPER_CW);
		currentXsteps = pos.x;
	}
}

void pickUp(enum positons_steps posOfZ){

	STEPPERMOTOR_steps(STEPPERZ,posOfZ,STEPPER_CW);
	SERVO_angle(SERVO_GRIPPER,SERVO_GRIPPER_CLOSE);
	_delay_ms(2000);
	STEPPERMOTOR_moveTillLS(STEPPERZ,STEPPER_CCW,LS_ZHOME);
}

void release(enum positons_steps posOfZ){

	STEPPERMOTOR_steps(STEPPERZ,posOfZ,STEPPER_CW);
	_delay_ms(2000);
	SERVO_angle(SERVO_GRIPPER,SERVO_GRIPPER_OPEN);
	_delay_ms(2000);
	STEPPERMOTOR_moveTillLS(STEPPERZ,STEPPER_CCW,LS_ZHOME);
}


