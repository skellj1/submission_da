/*
 * Part1_C.c
 *
 * Created: 3/8/2019 4:33:30 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL		// Set frequency of CPU for delay function
#include <avr/io.h>				// AVR input/output header
#include <avr/interrupt.h>		// AVR interrupt header
#include <util/delay.h>			// delay header


int main(void)
{
	DDRB |= (1<<2);		// PB2 is an output
	PORTB |= (1<<2);	// PB2 is high
	DDRD &= (0<<2);		// PD2 is an input
	PORTD |= (1<<2);	// PD2 is high
	
	EICRA = 0x0;		// make INT0 respond to low level shift
	
	EIMSK = (1<<INT0);	// enable external interrupt 0
	sei ();				// globally enable interrupt
	
	while (1);			// stay here until interrupt is activated
}

	ISR (INT0_vect)		// Interrupt subroutine for ext. int. 0
	{		
		PORTB &= ~(1<<2);		// Turn on LED by setting PORTB.2 to 0
		_delay_ms(1250);		// Call for a delay of 1.25 seconds
		PORTB |= (1<<2);		// Turn off LED by setting PORTB.2 to 1
	}


