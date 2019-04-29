/*
 * DA5.c
 * Created: 4/27/2019 4:23:13 PM
 * Author : James Skelly
 */ 

// Set clock frequency for delay function
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600								// set baud rate to 9600
#endif
#include "inc\STDIO_UART.c"

// Include NRF library
#include "inc\nrf24l01.c"
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// function prototyping
void print_config(void);
void ADC_init (void);	
						
volatile unsigned char ADCtemp[5];				// initialize variable for ADC temp
volatile uint8_t ADCvalue;						// initialize variable for the ADC value
volatile bool received;							// initialize variable for message reception
volatile bool status = false;					// initialize variable for status of transmission

// Main code body 

int main(void)
{
	sei();										// globally enable interrupts
	ADC_init();									// initialize the ADC
	uart_init();								// initialize the UART
	
	char tx_message[32];						// character array to output string
	strcpy(tx_message,"Lets get it started");	// copy string "Lets get it started" into array
	
	nrf24_init();								// initialize NRF
	print_config();								// run print config function
	nrf24_start_listening();					// start listening for transmission from other user
	
	strcpy(tx_message,"Lets get it started");	// Copy string into array
	nrf24_send_message(tx_message);				// send the string message to the other user
	
	while (1)
	{// if a message is received, send a message back with a success message for assurance
		if (received == true)
		{
			received = false;					// reset received variable to false
			printf("Received message: %s\n",nrf24_read_message());
			_delay_ms(500);
			
			status = nrf24_send_message(ADCtemp);
			if (status == true) printf("Message sent successfully\n");
		}
	}
}

// Interrupts

//	Interrupt subroutine (IRQ)
ISR(INT0_vect)
{
	received = true;
}

//  Interrupt subroutine for ADC
ISR(ADC_vect)
{
	volatile unsigned int j=0;   
	char temp[5];
	
	ADCvalue = (ADCH << 1);		 // Shifts the left adjusted ADCH value left by 1
	itoa(ADCvalue, temp, 10);	 // Converts integer to string

	while (j<5)					 // Transfers the temp string from itoa() to ADCtemp
	{
		ADCtemp[j] = temp[j];
		j++;
	}
}

// Functions

void ADC_init(void)
{
	ADMUX |= (1 << REFS0)|(1 << ADLAR);	// set AVcc (reference voltage for ADC) and
										// left justify value in ADC (10-bit register)
	
	ADCSRA = (1 << ADEN)|						// enable the ADC
	(1 << ADSC) |								// start converting 
	(1 << ADATE) |								// enable ADC auto-trigger
	(1 << ADIE) |								// enable ADC interrupt
	(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);		// set ADC prescaler of 128
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}
