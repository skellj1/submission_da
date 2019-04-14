/*
 * Button_PWM.c
 *
 * Created: 4/11/2019 2:30:08 AM
 * Author : James Skelly
 */ 

#define F_CPU 16000000UL
#define MOTOR 6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int button = 0;

int main(void)
{
	DDRD |= (1<<6);		// sets PD6 as output pin
	DDRB |= (1<<2);		// sets PortB.3 as output 
	DDRC = 0x00;		// sets port C as input
	PORTC = 0xFF;		// initially set portC high
	
	TIMSK0 = 0x01;		// enable timer0 overflow interrupt
	sei();				// globally enables interrupts
	
	TCCR0A = 0x00;		// set timer0 to operate in normal mode
	TCCR0B = 0x05;		// set prescaler to 1024
	
	
	// Setup and enable ADC //
   ADMUX = (0<<REFS1)|    // Reference Selection Bits
   (1<<REFS0)|    // AVcc - external cap at AREF
   (0<<ADLAR)|    // ADC Left Adjust Result
   (0<<MUX2)|     // Analog Channel Selection Bits
   (0<<MUX1)|     // ADC0 (PC0) 
   (0<<MUX0);
   
   ADCSRA = (1<<ADEN)|    // ADC Enable
   (0<<ADSC)|     // ADC Start Conversion
   (0<<ADATE)|    // ADC Auto Trigger Enable
   (0<<ADIF)|     // ADC Interrupt Flag
   (0<<ADIE)|     // ADC Interrupt Enable
   (0<<ADPS2)|    // ADC Pre-scaler Select Bits
   (1<<ADPS1)|	  // scales output of ADC by 4
   (0<<ADPS0);
	
	TCNT0 = 0;
	
	while (1)
	{	
		ADCSRA|=(1<<ADSC);					// start conversion
		while((ADCSRA&(1<<ADIF))==0);   // wait for conversion to finish

		ADCSRA |= (1<<ADIF);	// reset interrupt flag
		
		if (!(PINC & (1<<PINC2)))	// if button 2 is pressed...
		{
			_delay_ms(200);			// delay for de-bouncing button
			button ^= 1;			// toggle button variable
		}
	}
}

ISR (TIMER0_OVF_vect)		  // timer0 overflow interrupt vector
{
	if (button == 1)
	{	
		TCCR2A = (1<<COM2A0)|(1<<WGM21)|(1<<WGM20);	// Fast PWM, non-inverted
 		TCCR2B = (1<<WGM22)|(1<<CS22)|(1<<CS21)|(1<<CS20);	// pre-scaler of 1024, toggle OC2A on compare match
	
		while (button == 1)
		{
			PORTD |= (1<<MOTOR);   // turns on motor
			PORTB |= (1<<PB3);   // turns on motor
			//int a = ADCH;
			//OCR2A = a;
			
			if (!(PINC & (1<<PINC2)))	// if button 2 is pressed...
			{
				_delay_ms(200);			// delay for de-bouncing button
				button ^= 1;				// toggle button variable
			}
		}
	}
	
	else if (button == 0)
	{
		PORTD &= ~(1<<MOTOR);	  // turns off motor
		PORTB = 0;
	}
	
	TCNT0 = 0;
}
