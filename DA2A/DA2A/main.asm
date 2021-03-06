; DA2A.asm
; Created: 3/1/2019 3:05:51 PM
; Author : James Skelly
; Replace with your application code

; Design Assignment 2A:
; The goal of the assignment is use GPIO and delays:

; 1. Design a delay subroutine to generate a waveform on PORTB.2 with 60% DC and 0.725
; sec period.

LDI R19, 0x04				// Load immediate 0x04 into r19
LDI R20, 0x00				// Load immediate 0x00 into R20
OUT DDRB, R19				// Set PORTB.2 to output 

main:
	OUT PORTB, R19			// Set PORTB.2 high 
	rcall delay_100ms		// Call for a delay of 435 ms
	rcall delay_100ms		//
	rcall delay_100ms		//
	rcall delay_100ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_5ms			// End of 435 ms delay

	OUT PORTB, R20			// Set PORTB.2 low
	rcall delay_100ms		// Call for a delay of 290 ms
	rcall delay_100ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		//
	rcall delay_10ms		// End of 290 ms delay
	
rjmp main					// Jump back to the top of the loop

delay_100ms:
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	ret

delay_10ms:
	rcall delay_5ms
	rcall delay_5ms
	ret


delay_5ms:
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	ret


delay_1ms:
		push r16			// push r16 to stack
		push r18			// push r18 to stack
		ldi  r16, 255		// load 255 into r16
		ldi  r18, 6			// load 6 into r18

delay_1b:					// label for outer loop
delay_1a:					// label for inner loop
		nop					// no operation, takes 1 cycle
		nop					//
		nop					//
		nop					//
		nop					//
		nop					//
		nop					//
		dec r16				// decrement the value in r16
		brne delay_1a		// if the value in r16 is not 0,
								// stay in the loop 

		dec r18				// decrement the value in r18		
		brne delay_1b		// if the value in r18 is not 0,
								// stay in the loop
		 
		pop r18				// pop r18 back from the stack
		pop r16				// pop r16 back from the stack
		ret					// return to line below rcall
	
