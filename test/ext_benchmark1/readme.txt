/**************************************************************************************
EXTERNAL INTERRUPT: TOGGLE THE VALUE OF A VARIABLE WHEN EXTERNAL INTERRUPT IS TRIGGERED
***************************************************************************************/

#include "lpc17xx.h"

#define PINSEL_EINT0    20
#define SBIT_EINT0      0
#define SBIT_EXTMODE0   0
#define SBIT_EXTPOLAR0  0

int var = 0;

void EINT0_IRQHandler(void)
{
    LPC_SC->EXTINT = (1<<SBIT_EINT0);  /* Clear Interrupt Flag */
		var = 1 - var;
}

int main () {

	SystemInit();
	LPC_SC->EXTINT      = (1<<SBIT_EINT0);       /* Clear Pending interrupts */
  LPC_PINCON->PINSEL4 = (1<<PINSEL_EINT0);     /* Configure P2_10 as EINT0 */
  LPC_SC->EXTMODE     = (1<<SBIT_EXTMODE0);   /* Configure EINTx as Edge Triggered*/
  LPC_SC->EXTPOLAR    = (1<<SBIT_EXTPOLAR0); /* Configure EINTx as Falling Edge */

 NVIC_EnableIRQ(EINT0_IRQn);    /* Enable the EINT0 interrupt */

 while(1) {}/*We need an infinte loop so we could always check external interrupt otherwise program terminates*/
}