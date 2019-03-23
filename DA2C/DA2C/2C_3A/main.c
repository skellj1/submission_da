/*
 * 2C_3A.c
 *
 * Created: 3/21/2019 11:39:35 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0x04;	// set PORTB.2 to output
	PORTB = 0x04;	// turn LED initially off
	TCCR0A = 0x02;	// timer0 to operate in CTC mode
	TCCR0B = 0x05;	// set prescaler to 1024
	OCR0A = 0xFF;	// load compare register value
	TIMSK0 = 0x02;	// interrupt enabled on compare match A
	sei();			// enable interrupts
	
    while (1) 
    {
    }
}

uint8_t Overflow = 0;	// initialize overflow variable

ISR (TIMER0_COMPA_vect)
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

