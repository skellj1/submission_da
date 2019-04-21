/*
 * Servo.c
 *
 * Created: 4/18/2019 9:13:12 PM
 * Author : James Skelly
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void timer_init(void);   // timer1 setup function
void adc_init(void);	 // ADC setup function

int main(void)
{
	while (1)
	{
		timer_init();		// initialize timer1
		adc_init();			// initialize ADC
		ICR1=5000;			// ICR is TOP for FAST PWM mode 14; set top to 5000
		DDRB |= (1<<PB1);	// PB1 output mode, because OC1A is internally tied to PB1 
		OCR1A=ADC;			// set ADC output value as compare register value
		_delay_ms(500);		// call for delay as motor moves from adjusting pot value
	}
}

// Clear OC1A & OC1B on compare match, FAST PWM mode, prescaler of 64
void timer_init(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
}

void adc_init(void)
{
	ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0);
}