/* Infra-red sensor driver
 * IR.h
 *
 *  Created on: Nov 29, 2023
 *      Author: mario
 */

#ifndef IR_H_
#define IR_H_

#include"MCAL_Atmega32/std_types.h"
#include"MCAL_Atmega32/GPIO.h"

/***************************************************************************
 *                                Definitions
 *************************************************************************** */

#define IR_OBJECT_DETECTED    1
#define IR_NO_OBJECT_DETECTED 0

#define IR_PORT PORTC_ID


/***************************************************************************
 *                              IR's definition
 *************************************************************************** */

#define IR_GRIPPER 3

/*
 * The following 3 defines are not IR sensors, they're tHREE GPIO pins connected from other
 * MCU used for indication of the object type AND COLOR
 * (instead of using serial communication for simplicity)
 *
 * IR_BASE && IR_COVER == HIGH >> GREEN BASE
 * IR_BASE && IR_COLOR == HIGH >> GREEN COVER
 * IR_COVER && IR_COLOR == HIGH >> RED BASE
 * IR_BASE && IR_COVER && IR_COLOR == HIGH >> RED COVER
 *
 *  */
#define A0 4
#define A1 5
#define A2 6



/***************************************************************************
 *                           Functions declaration
 *************************************************************************** */

void IR_init(uint8 IR_num);
uint8 IR_getReading(uint8 IR_num);

#endif /* IR_H_ */
