#define F_CPU 16000000UL
#define MOTOR 6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int button = 0;

int main(void)
{
	unsigned int a = 0;
	
	DDRB |= (1<<1);		// sets PortB.3 as output
	DDRC &= ~(1<<0);	
	
	TCCR1A = (1<<COM1A1)|(1<<WGM11);			// clear on compare match
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);	// Fast PWM mode 14 timer1
	ICR1 = 9999;								// set top value for timer1
	
	// Setup and enable ADC //
	/*ADMUX = (0<<REFS1)|    // Reference Selection Bits
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
	(0<<ADPS0);*/
	DIDR0 |= (1<<ADC0D);
	ADMUX |= 0;
	ADCSRA = 0x87;
	ADCSRB = 0;
	
	
	while (1)
	{
		ADCSRA|=(1<<ADSC);					// start conversion
		while((ADCSRA&(1<<ADIF))==0);   // wait for conversion to finish
		
		a = (ADC & 0x3ff);
		a = a * 5; 
		OCR1A = 5000;
		OCR1B = 0;
	}
}