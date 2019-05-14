/*
 * Midterm 2
 *
 * Created: 5/8/2019 1:43:56 AM
 * Author : Skellj1
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "SparkFun_APDS9960.h"
#include "i2c_master.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define FOSC 16000000
#define UBRREQ FOSC/16/BAUD -1
#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

void UART_init (void);
void APDS_init (void);
int uart_putchar( char c, FILE *stream);
FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL , _FDEV_SETUP_WRITE);
void ReadValues(void);

// Initialize variables for field readings of red, green, blue light
uint16_t RED, GREEN, BLUE;
char sred[5], sgreen[5], sblue[5];

int main( void )
{
	UART_init();			// UART Initialization
	APDS_init();			// Sensor Module Initialization
	i2c_init();				// I2C Initialization
	stdout = &str_uart;
	
	RED = 0;				// Initialize RED read variable to 0
	GREEN = 0;				// Initialize GREEN read variable to 0
	BLUE = 0;				// Initialize BLUE read variable to 0
	
	
	_delay_ms(2000);
	printf("AT\r\n");			// AT initial communication confirmation
	_delay_ms(5000);
	printf("AT+CWMODE=3\r\n");	// AP + Station Mode
	_delay_ms(5000);
	printf("AT+CWJAP=\"[Network]\",\"[Password]\"\r\n");	// WIFI information inserted here 

	while(1) 
	{
		_delay_ms(5000);
		printf("AT+CIPMUX=0\r\n");		// Initial connection
		_delay_ms(5000);
		printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");	// connect to cloud
		_delay_ms(5000);
		ReadValues();					// call function to read values from sensor
		printf("AT+CIPSEND=104\r\n");
		
		// channel key for thingspeak (API Key excluded)
		printf("GET https://api.thingspeak.com/update?api_key=[API_KEY_GOES_HERE]&field1=0%05u&field2=%05u&field3=%05u\r\n", RED, GREEN, BLUE);
		_delay_ms(3000);	
	}
}

void UART_init(void)
{
	// Set baud rate
	uint16_t BAUDRATE = UBRREQ;
	UBRR0H = BAUDRATE >> 8;
	UBRR0L = BAUDRATE & 0xFF;
	
	// Enable receive/transmit 
	UCSR0B = ( 1 << RXEN0)|( 1 << TXEN0);
	
	// 8 data bit, one stop bit 
	UCSR0C = (3 << UCSZ00);
}

void APDS_init(void)
{
	uint8_t SETUP;
	
	i2c_readReg(APDS9960_WRITE, APDS9960_ID, &SETUP,1);
	if(SETUP != APDS9960_ID_1) while(1);
	SETUP = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &SETUP, 1);
	SETUP = DEFAULT_ATIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &SETUP, 1);
	SETUP = DEFAULT_WTIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &SETUP, 1);
	SETUP = DEFAULT_PROX_PPULSE;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &SETUP, 1);
	SETUP = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &SETUP, 1);
	SETUP = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &SETUP, 1);
	SETUP = DEFAULT_CONFIG1;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &SETUP, 1);
	SETUP = DEFAULT_PERS;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &SETUP, 1);
	SETUP = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &SETUP, 1);
	SETUP = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &SETUP, 1);
}

int uart_putchar(char c, FILE *stream)
{
	//	wait here 
	while ( !( UCSR0A & ( 1 <<UDRE0)) );
	
	// insert data into buffer
	UDR0 = c;
	return 0;
}

void USART_putstring(char *StringPtr)
{
	while ((*StringPtr != '\0'))
	{	
		while (!(UCSR0A & (1 << UDRE0))); // loop until UDRE0 is high
		UDR0 = *StringPtr;				  // grab value from pointer address
		StringPtr++;					  // Increment string pointer character by character
	}
}


// Function to read values from sensor
void ReadValues()
{
	uint8_t redHigh, redLow;
	uint8_t greenHigh, greenLow;
	uint8_t blueHigh, blueLow;
	
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAH, &redHigh, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAL, &redLow, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAH, &greenHigh, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAL, &greenLow, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAH, &blueHigh, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAL, &blueLow, 1);
	
	// bit manipulation to format values read in from sensor
	
	RED = (redHigh << 8) | redLow;
	GREEN = (greenHigh << 8) | greenLow;
	BLUE = (blueHigh << 8) | blueLow;
	
	// If statements to cap value at 255 to keep an 8-bit value
	if (RED > 255)
	{
		RED = 255;
	}

	if (GREEN > 255)
	{
		GREEN = 255;
	}
	
	if (BLUE > 255)
	{
		BLUE = 255;
	}
}


