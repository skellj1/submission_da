/*
 * DA3A_C.c
 *
 * Created: 3/25/2019 11:51:23 AM
 * Author : James Skelly
 * CPE 301 Design Assignment 3A: UART
 */ 

// Define board frequency 
#define F_CPU 16000000UL

// Include necessary headers for operation
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

// Define Baud Rate and Baud Rate Prescaler
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL)))-1)

// Define the functions used in the program
volatile int Counter;
void USART_init( unsigned int ubrr );
void USART_TX_string(char *data);

// Define variables used in the program
float phi;
char outs[30];
int random_number;

// Define string to be output and white space variable

char str[] = "= The Golden Ratio";		// Initialize string "= The Golden Ratio"
char space[] = " ";						// Initiliaze string for white space

int main(void)
{
	Counter = 0;						// Initialize counter variable to 0
	TIMSK0 |= (1<<TOIE0);				// Set timer overflow interrupt enable bit
	sei ();								// Enable interrupts
	TCCR0A = 0x00;						// Set timer 0 to normal mode operation
	TCCR0B |= (1<<CS02)|(1<<CS00);		// Set prescaler to 1024
	USART_init(BAUD_PRESCALLER);		// Initialize Baud prescaller in program
	USART_TX_string("Connected\r\n");	// Prints connected once the USART is connected 
    
	while (1);							// wait here for interrupt to occur										
}
	// INT USART (RS-232)
void USART_init( unsigned int ubrr )	// USART initial function header
{
	UBRR0H = (unsigned char)(ubrr>>8);	// Set baud rate reg. high bits
	UBRR0L = (unsigned char)ubrr;		// set baud rate reg. low bits
	UCSR0B = (1 << TXEN0);				// Enable the Tx interrupt
	UCSR0C = (3 << UCSZ00);				// Asynchronous 8 N 1
}
	// Send a string to the RS-232
void USART_TX_string(char *data)		// USART string initial function header
{
	while (*data != '\0')				// while conj data is not equal to zero
	{
		while (!(UCSR0A & (1<<UDRE0)));	// while not conditions met 
		UDR0 = *data;					// set UDR0 to data
		data++;							// increment data variable
	}
}
ISR (TIMER0_OVF_vect){					// Header for timer overflow interrupt subroutine
	while (Counter < 61){				// Loop while count is less than 61
		if ((TIFR0 & 0x01) == 1){		// If timer interrupt flag bit 0 is high...
			TIFR0 = 0X01;				// Clear flag
			Counter++;					// Increment count variable
		}
	}
	if (Counter > 60){					// If counter variable is greater than 60...
		USART_TX_string(str);			// Print string character
		USART_TX_string(space);			// Print out blank space
				
		random_number = rand();			// generate a random number in random number variable
		phi = 1.618034;					// stores the golden ratio, 1.618034, in phi variable
				
		snprintf(outs, sizeof(outs), "%3d\r\n", random_number);		// print out random number
		USART_TX_string(outs);
		USART_TX_string(space);
		
		sprintf(outs, "%f", phi);		// Print out floating point value
		USART_TX_string(space);
		Counter = 0;					// Reset counter to 0
	}
}