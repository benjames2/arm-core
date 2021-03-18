/**********************************
STEPPER MOTOR - FULL STEPPING WITH RIT INTERRUPT CLOCKWISE DIRECTION
**********************************/

#include "lpc17xx.h"
//#include <stdio.h>
//#include <math.h>

#define RITENCLR  		(1UL<<1)
#define RITEN	   		(1UL<<3)
#define RITENBR		(1UL<<2)
#define ENABLE_PCRIT 	(1UL<<16)

unsigned int i = 0;
unsigned int step [] = {28,29,30,31};


void  RIT_IRQHandler (void) {

	LPC_GPIO1->FIOPIN = (1<<step[i]);
	
	// clear the interrupt flag
	LPC_RIT->RICTRL |= 1;
	
	// step i in the order 0 -> 1 -> 2 -> 3 -> 0 ->...
	if(i==3)i=0;

	else i++;

}


int main () {
	SystemInit();

/***********************************************************************************************************************
Setup the Repititive Interrupt Timer for the stepper motor
************************************************************************************************************************/

 	LPC_GPIO1->FIODIR |= (0xF0000000);
	
 	// clear the bits 31 to 28	

 	LPC_GPIO1->FIOPIN &= ~(0xF0000000);
	
	/* enable RIT in PCON register */
	 LPC_SC->PCONP |= ENABLE_PCRIT; 

	// clear the CTRL register //
	LPC_RIT->RICTRL = 0;
	
	// set the compare value //
	LPC_RIT->RICOMPVAL = 100; // 12 steps per second = 30 degree //originally 1500000

	// clear the counter register //
	LPC_RIT->RICOUNTER = 0;

	
	// set the CTRL register //
	 LPC_RIT->RICTRL = RITENCLR| RITENBR| RITEN;

	// enable the RIT interrupts
	NVIC_EnableIRQ(RIT_IRQn);

	// loop forever
	while(1) {

		}
	}
