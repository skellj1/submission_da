/*
 * Stepper.c
 *
 * Created: 4/20/2019 6:22:27 PM
 * Author : James Skelly
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void timer_init(void);  
void adc_init(void);  

int main(void)
{
	int period;    // integer used to set the period
	
	while (1)
	{
		timer_init();      // initialize and set up timer to be used
		adc_init();        // initialize and set up ADC
		DDRB=0x0F;		   // Port B low bits to output
		OCR0A = ADC;	   // set value of ADC data registers as compare value for timer
		
		// The following if statements each contain the same code with a different period,
		// depending on the voltage read out of the potentiometer at PC0. The values that 
		// PORTB is set to (i.e. 0x09, 0x03, 0x06, and 0x0C) allow the motor to make steps
		// and rotate at a speed specified by the potentiometer. Since all of this is inside
		// the while(1) loop, the motor will keep spinning until OCR0A satisfies a different
		// condition, in which case the motor will spin at a different speed. 
		
		if ((OCR0A>=0)&&(OCR0A<=50))
		{
			period =2 ;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
			OCR0A = ADC;
		}
		else if ((OCR0A>50)&&(OCR0A<=120))
		{
			period=30;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
			OCR0A = ADC;
		}
		else if ((OCR0A>120)&&(OCR0A<=150))
		{
			period=100;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
			OCR0A = ADC;
		}
		else if ((OCR0A>150)&&(OCR0A<=200))
		{
			period=200;
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0c;
			_delay_ms(period);
			OCR0A = ADC;
		}
		else if (OCR0A>200)
		{
			PORTB = 0;
			OCR0A = ADC;
		}
	}
}

void timer_init(void)
{
	// Clear OC0A on compare match, CTC mode, prescaler 256
	TCCR0A |= (1<<COM0A1)|(1<<WGM01);
	TCCR0B |= (1<<CS02);
}

void adc_init(void)
{
	ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0);
}

