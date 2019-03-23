/*
 * 2C_2A.c
 *
 * Created: 3/21/2019 11:38:47 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0x04;	// set PORTB.2 to output
	PORTB = 0x04;	// initially turn LED at portb.2 off
	TIMSK0 = 0x01;	// enable timer0 overflow interrupt
	TCNT0 = 0x00;	// initialize timer0 to 0
	sei();			// enable interrupts
	
	TCCR0A = 0x00;	// set timer0 to normal mode operation
	TCCR0B = 0x05;	// set prescaler of 1024
    
    while (1) 
    {
		// main loop
    }
}

uint8_t Overflow = 0;	// initialize overflow variable

ISR (TIMER0_OVF_vect) // interrupt subroutine
{
	TCNT0 = 0;			// reset timer0 to 0 when interrupt occurs
	Overflow++;			// increment overflow variable
	
	if (Overflow == 18)	// if statement to check for 18th overflow of timer0
	{
		PORTB = 0x00;	// turns LED at PORTB.2 on
	}
	
	if (Overflow == 44)	// if statement to check for 44th overflow of timer0
	{
		PORTB = 0x04;	// turns LED off
		Overflow = 0;	// resets overflow variable to 0
	}
	
}
