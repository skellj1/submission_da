/*
 * 2C_1B.c
 *
 * Created: 3/21/2019 11:37:37 PM
 * Author : James Skelly
 */ 

// 2. Connect a switch to PORTC.2 to poll for an event to turn on the LED
// at PORTB.2 for 1.250 seconds after the event.


#define F_CPU 16000000UL	// Set frequency of CPU to 16 MHz for delay function
#include <avr/io.h>			// Including AVR and Delay header libraries
#include <util/delay.h>

int main (void)
{
	uint8_t Overflow = 0;	// Declare overflow variable and initialize to 0
	DDRB = 0x04;		// Sets DDRB pin 2 to output mode
	PORTB = 0x04;		// Sets PORTB pin 2 to high 
	DDRC = 0x00;		// Sets DDRC pin 2 to input mode 
	PORTC =0x04;		// Sets PORTC pin 2 to high 
	
	TCCR0A = 0x00;			// Set timer0 to operate in normal mode
	TCCR0B = 0x05;			// Set prescaler to 1024
	
	while (1)
	{		
			
		if (!(PINC & (1<<PINC2)))   // Check PINC for a value of 1 for a value of 1,
										// complemented to check for a zero instead.
		{
			Overflow = 0;		// reset overflow variable to 0 when button pressed
			TCNT0 = 0x00;		// reset timer to 0 when button pressed
			PORTB = 0x00;		// Turn on LED by setting PORTB.2 to 0.
		}
		
		
		
		while ((TIFR0 & 0x01) == 0);	// check for overflow in overflow flag bit
		
				TIFR0 = 0x01;			// reset overflow flag
				Overflow++;				// increment overflow variable
			
				if (Overflow == 76)		// check for 76th overflow to achieve desired
											// delay of 1.250 seconds
				{
					PORTB = 0x04;		// turn the LED off after 1.250 seconds
				}
				
			TCNT0 = 0;					// reset the timer
	}
	return 0;
}

