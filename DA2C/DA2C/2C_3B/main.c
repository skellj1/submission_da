/*
 * 2C_3B.c
 *
 * Created: 3/21/2019 11:44:36 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int button = 0;

int main(void)
{
    DDRB = 0x04;	// set PORTB.2 to output
    PORTB = 0x04;	// turn LED initially off
	DDRC = 0x00;	// set PORTC.2 to input
	PORTC = 0x04;	// set PORTC.2 initially high
	
    TCCR0A = 0x02;	// timer0 to operate in CTC mode
    TCCR0B = 0x05;	// set prescaler to 1024
    
	OCR0A = 0xFF;	// load compare register value
    
	TIMSK0 = 0x02;	// interrupt enabled on compare match A
    sei();			// enable interrupts
	
    while (1) 
    {
		if (!(PINC & (1<<PINC2)))   // Check PINC for a value of 1 for a value of 1,
		// complemented to check for a zero instead.
		{
			button = 1;	// activates if statement inside interrupt subroutine
		}
		
		else
		{
			PORTB = 0x04; // keeps LED off until button is pressed
			button = 0;	  // does not enter subroutine if statement
		}
    }
	return 0;
}

ISR (TIMER0_COMPA_vect)
{
	int overflow = 0;			// initialize overflow variable
	int overflow_amt = 76;		// set variable for amount required to achieve
									// desired delay of 1.25 seconds.
	int overflow_check = 0;		// initialize overflow check variable
	
	if (button == 1)
	{
		PORTB = 0x00;			// turn on LED if button is pressed
		TCNT0 = 0;				// start timer0 at 0
		
		while(overflow < overflow_amt)		// keep looping until desired delay is reached
		{
			overflow_check = TIFR0 & 0x01;	// check for overflow
			if (overflow_check == 1)		// if overflow,
			{
				overflow++;						// increment overflow variable
				TIFR0 = 0x01;					// reset overflow flag
			}
		}
		overflow = 0;							// reset overflow variable to 0
		PORTB = 0x04;							// turn LED off after 1.250 s delay
		button = 0;								// reset button variable to 0
	}
}
