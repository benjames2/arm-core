README 

This file contains assembly code to drive a motor. It does not use any interrupt.  

This is the C code associated with this file 


/**********************************
STEPPER MOTOR - FULL STEPPING WITHOUT INTERRUPT CLOCKWISE DIRECTION
**********************************/

#include "lpc17xx.h"

unsigned int i = 0;
unsigned int MAX_TIME = 100;

int main () {

	SystemInit();

 	LPC_GPIO1->FIODIR |= (0xF0000000);  //Usinp port 1 register 
 	// clear the bits 31 to 28	
 	LPC_GPIO1->FIOPIN &= ~(0xF0000000);
	
	///*
	while(1) 
		{
		// set to 1
		for(i=0; i<MAX_TIME; i++) {}
		LPC_GPIO1->FIOPIN = (0X10000000);
		
		// set to 2
		for(i=0; i<MAX_TIME; i++) {}
		LPC_GPIO1->FIOPIN = (0X20000000);
		
		// set to 4
		for(i=0; i<MAX_TIME; i++) {}
		LPC_GPIO1->FIOPIN = (0X40000000);
		
		// set to 8
		for(i=0; i<MAX_TIME; i++) {}
		LPC_GPIO1->FIOPIN = (0X80000000);
	}
	//*/
//	return 0;
}
