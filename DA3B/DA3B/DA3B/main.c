/*
 James Skelly, DA3B C Code
 */ 

#define F_CPU 16000000UL	// define the frequency of the cpu to 16 MEG
#define BAUD_RATE 9600		// sets Baud rate, the rate of bit transfer

// Include the necessary headers
#include <avr/io.h>	
#include <util/delay.h>
#include <avr/interrupt.h>

int overflow; // initialize overflow variable for delay

// void function prototypes
void usart_init ();	
void usart_send (unsigned char ch);

int main (void)
{
	usart_init ();
	
   // Setup and enable ADC //
   ADMUX = (0<<REFS1)|    // Reference Selection Bits
   (1<<REFS0)|    // AVcc - external cap at AREF
   (0<<ADLAR)|    // ADC Left Adjust Result
   (1<<MUX2)|     // Analog Channel Selection Bits
   (0<<MUX1)|     // ADC5 (PC5) 
   (1<<MUX0);
   
   ADCSRA = (1<<ADEN)|    // ADC ENable
   (0<<ADSC)|     // ADC Start Conversion
   (0<<ADATE)|    // ADC Auto Trigger Enable
   (0<<ADIF)|     // ADC Interrupt Flag
   (0<<ADIE)|     // ADC Interrupt Enable
   (1<<ADPS2)|    // ADC Prescaler Select Bits
   (0<<ADPS1)|	  // ADC5 (PC5)
   (1<<ADPS0);
	
	// Setup and enable timer0 // 
	TCCR0B |= (1<<CS02) | (1<<CS00);	// set prescaler to 1024
	TIMSK0 |= (1<<TOIE0);		// interrupt enabled on compare match A
	sei();				// enable interrupts
	TCNT0 = 0;			// resets timer0
	
   
	while (1)
	{	
		ADCSRA|=(1<<ADSC);	//start conversion
			while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
			
			ADCSRA |= (1<<ADIF);

	if (overflow == 61)		// generates 1 second delay using interrupt
		{					// code to convert output to Fahrenheit
			
			int a = ADCL;
			a = a | (ADCH<<8);
			a = (a/1024.0) * 5000/10;
			usart_send((a/100)+'0');
			a = a % 100;
			usart_send((a/10)+'0');
			a = a % 10;
			usart_send((a)+'0');
			usart_send('\r');
			overflow=0;		// resets the overflow increment value to 0
		}
	}
	return 0;
}

ISR (TIMER0_OVF_vect)		// Interrupt subroutine
{
	while(!(TIFR0 & 0x01) == 0);	// while timer interrupt flag is high...
	{	
		TCNT0 = 0;			// ... reset timer0 
		TIFR0 = 1;			// ... reset the interrupt flag
		overflow++;			// ... increment overflow variable
	}
}

void usart_init (void)		//	function to initialize the USART 
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (unsigned char ch)	// function to transit characters to PC
{
	while (! (UCSR0A & (1<<UDRE0))); 	//wait until UDR0 is empty
	UDR0 = ch;							//transmit ch
}

void usart_print(char* str)			// function to print out characters on PC
{
	int i = 0;
	while(str[i] != 0)
		usart_send(str[i]);
}
