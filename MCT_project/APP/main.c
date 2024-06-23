/*
 * main.c
 *
 *  Created on: Apr 18, 2024
 *      Author: mario
 */


#include"APP.h"



int main(void){

	APP_init();

	while (TRUE){

		/*
		 * Robotic arm will always be in one of these states
		 * 1- waiting for the object in feeding unit to arrive at the edge of conveyer to be assembled
		 * 2- assembly phase
		 * 3- storing phase
		 *  */

		switch(currentState){

		case WAIT:

			APP_wait();
			break;

		case ASSEMBLY:

			APP_assembly();
			break;

		case STORE:

			APP_store();
			break;
		}
	}
}
