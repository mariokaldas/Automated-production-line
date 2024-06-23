/*  limit switch driver
 * limitSwitch.h
 *
 *  Created on: Nov 29, 2023
 *      Author: mario
 */

#ifndef LS_H_
#define LS_H_

#include"MCAL_Atmega32/std_types.h"

/***************************************************************************
 *                                Definitions
 *************************************************************************** */

#define LS_OBJECT_DETECTED    0
#define LS_NO_OBJECT_DETECTED 1

#define LS_PORT PORTC_ID


/***************************************************************************
 *                              LS's definition
 *************************************************************************** */

#define LS_XHOME 0
#define LS_YHOME 1

#define LS_ZHOME 2

/***************************************************************************
 *                           Functions declaration
 *************************************************************************** */

void LS_init(uint8 LS_num);
uint8 LS_getReading(uint8 LS_num);

#endif /* LS_H_ */
