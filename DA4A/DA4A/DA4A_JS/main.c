/*
 * DA4A_JS.c
 * Created: 4/11/2019 10:54:25 AM
 * Author : James Skelly
 */ 

// define clock for delay function, include headers
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int adcVal;			// holds value of ADC
char toggle = 0;	// initialize toggle variable

int main(void)
{
	DDRB = 0xFF;			// PORTB as output
	DDRC = 0x00;			// PORTC as input
	PORTB |= 0xFF;			// Set all of PORTB high initially
	
	// set timer1 operation mode and prescaler
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);	// use PWM for output compare pins
													// OC1A/B, Fast PWM, Non-inverted mode
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11);		// timer1 pre-scaler set to 8
	ICR1 = 10000;									// set TOP value
	
	PORTC |= (1<<PORTC1);	// set portc initially high to wait for button press
	PCICR = (1<<PCIE1);		// set portc input capture enable bit
	PCMSK1 = (1<<PCINT9);	// enable pin change interrupt
	
	// Initialize ADC 
	DIDR0 = 0x1;		// disable the digital input on ADC0 pin
	ADMUX = (1<<REFS0);	// set ADC reference pin to PC0 (Potentiometer)
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// enable ADC using system clock 
	ADCSRB = 0x0;		// free running mode
	
	sei();				// globally enable interrupts
	
	while(1)		// wait for pin change interrupt request
	{	
	}
}

ISR(PCINT1_vect){
	if(!(PINC & (1 << PINC2)))				// if button is pressed...
	{				
		_delay_ms(200);						// debounce button pressed
		while(!(PINC & (1 << PINC2)));		
		
		if(toggle == 1){
			PORTB |= (1 << PORTB1);			// set PB1 high
			PORTB &= ~(1 << PORTB5);		// turn on LED D1
			ADCSRA |= (1 << ADSC);			// start conversion
			while((ADCSRA&(1<<ADIF))==0);	// wait for conversion to finish
			adcVal = ADC & 0x03FF;			// extract right 10-bits of ADC register
			OCR1A = 10*adcVal;				// OCR1A value for duty cycle
		}
		else if(toggle == 0){
			OCR1A = 0;						// set output compare value to 0
			PORTB &= ~(1 << PORTB1);		// set PB1 low
			PORTB |= (1 << PORTB5);			// turn of LED D1
		}
		
		toggle ^= 1;						// toggle the variable used to toggle
	}
}

