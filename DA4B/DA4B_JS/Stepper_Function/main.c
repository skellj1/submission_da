/*
 * Stepper_Function.c
 *
 * Created: 4/20/2019 6:05:27 PM
 * Author : james
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void initial(void);   // function setting Timer/Counter control Register 0
void adc_init(void);  // function declared for ADC set up
void motor(int);	  // function to change motor speed


int main(void)
{

	while (1)
	{
		initial();      // calling function to initialize the setting Timer/Counter control Register 0
		adc_init();     // calling function for ADC set up
		DDRB=0x0F;
		OCR0A = ADC;
		// setting the period to a value of 2, fast frequency
		if ((OCR0A>=0)&&(OCR0A<=20))
		{
			motor(2);
		}
		// setting the period to a value of 10
		else if((OCR0A>20)&&(OCR0A<=40))
		{
			motor(10);
		}
		// setting the period to a value of 20
		else if((OCR0A>40)&&(OCR0A<=80))
		{
			motor(30);
		}
		// setting the period to a value of 30
		else if((OCR0A>80)&&(OCR0A<=120))
		{
			motor(50);
		}
		
		else if((OCR0A>120)&&(OCR0A<=150))
		{
			motor(100);
		}
		// setiting the period to a value of 50, fast frequency
		else if((OCR0A>150)&&(OCR0A<=200))
		{
			motor(200);
		}
		
		else if(OCR0A>200)
		{
			PORTB = 0;
		}
	}
}

void initial(void)
{
	//clear on compare for OCOA
	//CTC mode
	//pre-scaler = 256
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(1<<WGM01)|(0<<WGM00);
	TCCR0B |= (1<<CS02)|(0<<CS01)|(0<<CS00);
}

// function to initialize ADC set up
void adc_init(void)
{
	ADCSRA |= ((1<<ADEN)|(1<<ADSC)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0);
}

void motor(int speed)
{
		PORTB = 0x09;
		_delay_ms(speed);
		PORTB = 0x03;
		_delay_ms(speed);
		PORTB = 0x06;
		_delay_ms(speed);
		PORTB = 0x0c;
		_delay_ms(speed);
}
