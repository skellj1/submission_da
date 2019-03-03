/*
 * DA2A_1_C.c
 *
 * Created: 3/1/2019 3:15:00 PM
 * Author : James Skelly
 * Design a delay subroutine to generate a waveform on PortB.2 with
 * 60% DC and 0.725 second period. 
 */ 

#define F_CPU 16000000UL			// Set frequency of CPU to 16 MHz for delay function

#include <avr/io.h>					// Including AVR and Delay header libraries
#include <util/delay.h>

int main(void)
{
	DDRB = 0x04;					// Set data direction register bit 2 as an output
	
	while(1)
	{
		PORTB = (1 << PORTB2);      // Shift a 1 into PORTB.2
		_delay_ms(435);				// Call for delay of 435 ms (0.725s * 0.6)
		PORTB = ~(1 << PORTB2);		// Shift a 0 into PORTB.2
		_delay_ms(290);				// Call for delay of 290 ms (0.725s * 0.4)
		
	}
   return 0; 
}
