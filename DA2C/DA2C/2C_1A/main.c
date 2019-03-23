/*
 * 2C_1A.c
 *
 * Created: 3/21/2019 11:37:08 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
	uint8_t Overflow = 0;	// declare overflow variable and initialize to 0
	DDRB = 0x04;	// set data direction reg. B to output
	TCCR0A = 0;		// normal mode operation
	TCCR0B = 0x05;	// prescaler of 1024
    TCNT0 = 0x0;	// start the timer
	
    while (1) 
    {
		// wait for the overflow event
		while ((TIFR0 & 0x01) == 0);
		
		TCNT0 = 0x00;	// reset timer0 to 0
		TIFR0 = 0x01;	// reset overflow flag 
		Overflow++;		// increment overflow variable
		
		if (Overflow == 18)		// if statement for 18th overflow of timer0
		{							// generate delay of 290 ms
			PORTB = 0x00;		// sets PORTB.2 high (reverse logic, LED on)
		}
		
		if (Overflow == 44)		// if statement for 44th overflow of timer0
		{							// generate delay of 435 ms
			PORTB = 0x04;		// sets PORTB.2 low (LED off)
			Overflow = 0;		// resets overflow counter to 0
		}
    }
}

