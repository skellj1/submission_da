/* DA2A_Task_2_C.c
 * Created: 3/1/2019 3:15:26 PM
 * Author : James Skelly */
 
 // 2. Connect a switch to PORTC.2 to poll for an event to turn on the LED 
 // at PORTB.2 for 1.250 seconds after the event.
 
 
#define F_CPU 16000000UL	// Set frequency of CPU to 16 MHz for delay function
#include <avr/io.h>			// Including AVR and Delay header libraries
#include <util/delay.h>

int main (void)
	{
		
		DDRB |= (1<<2);    // Sets DDRB pin 2 to output mode by performing bitwise
								// OR with a 1 shifted two places to the left, and
								// and the value previously stored in DDRB. The
								// result will be stored in DDRB.
								
		PORTB |= (1<<2);   // Sets PORTB pin 2 to high by performing bitwise
								// OR with a 1 shifted two places to the left, and
								// and the value previously stored in PORTB. The
								// result will be stored in PORTB. This will initially
								// turn the LED off.
		
		DDRC &= (0 << 2);  // Sets DDRC pin 2 to input mode by performing bitwise AND
								// with a 0 shifted two places to the left, and the 
								// value previously stored in DDRC. The result will
								// be stored in DDRC. 
								
		PORTC |= (1 << 2); // Sets PORTC pin 2 to high  by performing bitwise
								// OR with a 1 shifted two places to the left, and
								// and the value previously stored in PORTC. The
								// result will be stored in PORTC. This enables the
								// pull-up resistor.
		
		while (1) 
		{
			if (!(PINC & (1<<PINC2)))   // Check PINC for a value of 1 for a value of 1,
												// complemented to check for a zero instead.
			{
				PORTB &= ~(1<<2);		// Turn on LED by setting PORTB.2 to 0.
				_delay_ms(1250);		// Call for a delay of 1.25 seconds
			}
			else
			PORTB |= (1<<2);			// Turn off the LED by setting PORTB.2 to 1.
		}
		return 0;
	}

