/*
 James Skelly, Midterm 1
 Display the temperature (vs. time) from LM34 as a graph on ThingSpeak.com
 */ 

#define F_CPU 16000000UL	// define the frequency of the cpu to 16 MEG
#define BAUD_RATE 9600		// sets Baud rate, the rate of bit transfer
#define BAUD_PRESCALLER F_CPU/16/BAUD-1 // Baudrate prescaller

// Include the necessary headers
#include <avr/io.h>	
#include <util/delay.h>
#include <avr/interrupt.h>

int overflow; // initialize overflow variable for delay

// function prototypes
void usart_init ();	
void usart_send (unsigned char ch);
void usart_print (char* str);
void read_adc(void);  //Read LM34 using ADC
volatile unsigned int adc_temp; // Volatile raw temperature variable
volatile unsigned int tempF; // Volatile Fahrenheit temperature variable
char outs[256]; // String array used for sending USART commands
volatile char received_data; // String array used for receiving USART communication

int main (void)
{
	 // Setup and enable ADC //
	 ADMUX = (0<<REFS1)|    // Reference Selection Bits
	 (1<<REFS0)|    // AVcc - external cap at AREF
	 (0<<ADLAR)|    // ADC Left Adjust Result
	 (1<<MUX2)|     // Analog Channel Selection Bits
	 (0<<MUX1)|     // ADC4 (PC4)
	 (0<<MUX0);
	 
	 ADCSRA = (1<<ADEN)|    // ADC ENable
	 (0<<ADSC)|     // ADC Start Conversion
	 (0<<ADATE)|    // ADC Auto Trigger Enable
	 (0<<ADIF)|     // ADC Interrupt Flag
	 (0<<ADIE)|     // ADC Interrupt Enable
	 (1<<ADPS2)|    // ADC Prescaler Select Bits
	 (0<<ADPS1)|	  // ADC4 (PC4)
	 (0<<ADPS0);
	 
	 // Setup and enable timer0 //
	 TCCR0B |= (1<<CS02) | (1<<CS00);	// set prescaler to 1024
	 TIMSK0 |= (1<<TOIE0);				// interrupt enabled on compare match A
	 sei();								// enable interrupts
	 TCNT0 = 0;							// resets timer0
	
	usart_init (BAUD_PRESCALLER);
	_delay_ms(500);						// Delay to allow hardware to initialize
	
	while (1)
	{	
		// Repeatedly read the temperature value from the ADC and print to Thingspeak
		
		// AT COMMANDS
		// Check connection with handshake
		char AT[] = "AT\r\n";
		// Set mode 1 => Station mode
		char AT_CWMODE[] = "AT+CWMODE=1\r\n";
		// Connect to Wifi, using correct network name and password
		char AT_CWJAP[] = "AT+CWJAP=\"WiFi_Network\",\"PASSWORD\"\r\n";
		// Single IP Address Mode
		char AT_CIPMUX[] = "AT+CIPMUX=0\r\n";
		// Connect to Thingspeak.com (port 80)
		char AT_CIPSTART[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
		// Set string length
		char AT_CIPSEND[] = "AT+CIPSEND=100\r\n";
		
		// send the previous commands
		_delay_ms(200);
		usart_send(AT);
		_delay_ms(5000);
		usart_send(AT_CWMODE);
		_delay_ms(5000);
		usart_send(AT_CWJAP);
		_delay_ms(3000);
		usart_send(AT_CIPMUX);
		_delay_ms(3000);
		usart_send(AT_CIPSTART);
		_delay_ms(3000);
		usart_send(AT_CIPSEND);
		_delay_ms(5000);
		
		
		
	if (overflow == 61)		// generates 1 second delay using interrupt
		{					// code to convert output to Fahrenheit
			
			PORTC^= (1<<5);
			
			read_adc(); // Read next ADC value from LM34
			adc_temp = (adc_temp/1024.0)*500.0; // Convert to Fahrenheit
			tempF = adc_temp;
		// Print Data to Thingspeak using provided link, website channel key, and field location
			snprintf(outs,sizeof(outs),"INSERT WRITE KEY from THINGSPEAK HERE", tempF);
			USART_tx_string(outs);//send data
			_delay_ms(5000);
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
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable transmit, receive, interrupt
	UCSR0B = (1<<TXEN0) | (1 << RXEN0)| ( 1 << RXCIE0); 
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
