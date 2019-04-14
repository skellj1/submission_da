/*
 * DA4A.c
 *
 * Created: 4/10/2019 11:34:18 PM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int button = 0;

void LED_Sequence(void);

int main(void)
{	
	
	DDRD |= (1<<6);		// sets PD6 as output pin
	DDRC = 0x00;		// sets port C as input
	DDRB = 0xFF;		// sets PORTB as output pin
	PORTB = 0xFF;
	PORTC = 0x04;		// initially set portC.2 high
	TIMSK0 = 0x01;		// enable timer0 overflow interrupt
	sei();				// globally enables interrupts
	TCCR0A = 0x00;		// set timer0 to operate in normal mode
	TCCR0B = 0x05;		// set pre-scaler to 1024
	
	while (1)
	{
		if (!(PINC & (1<<PINC2)))	// if button is pressed...
		{
			_delay_ms(200);			// delay for de-bouncing switch
			button ^= 1;			// toggle button variable
		}
	}
}

ISR (TIMER0_OVF_vect)		  // timer0 overflow interrupt vector
{
  
	if (button == 1)
		{
			while (button == 1)
			{
				PORTD = 0b01000000;   // turns on motor
				PORTB = 0b11000011;
				
				if (!(PINC & (1<<PINC2)))	// if button is pressed...
					{
						_delay_ms(200);			// delay for de-bouncing switch
						button ^= 1;			// toggle button variable
					}
			}
		}
	
	else if (button == 0)
		{
			PORTD = 0;			  // turns off motor
			PORTB = 0xFF;
		}
}

void LED_Sequence(void)
{
	
}
