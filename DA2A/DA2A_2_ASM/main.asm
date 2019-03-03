; DA2A_2_ASM.asm
; Created: 3/1/2019 3:14:30 PM
; Author : James Skelly

; 2. Connect a switch to PORTC.2 to poll for an event to turn on the LED 
; at PORTB.2 for 1.250 seconds after the event.
	
.ORG 0 

LDI R16,  0x04		; loads 0x04 into register 16
OUT DDRB, R16		; sets data direction register PORTB.2 to output
OUT PORTB,R16		; sets PORTB.2 high 
LDI R17,  0x00		; loads 0x04 into register 17
OUT DDRC, R17		; sets data direction register PORTC to all inputs
OUT PORTC, R16		; sets port C, pin 2 high
NOP

MAIN:
	IN R20, PINC	; reads input on pin C into register 20
	COM R20			; invert the bits inputted to pin c
	ANDI R20, 0x04	; bitwise AND the value in register 20 with 0x04					
	CPI R20, 0x04	; compare the value in r20 with 0x04
	BRNE MAIN			

LIGHT_ON:
	LDI R21, 0xFB		; 11111011 INTO R21
	OUT PORTB, R21		; OUTPUT PORTB.2 LED 4 turns on
	rcall delay_1s		; call for delay of 1.25 s
	rcall delay_100ms	;
	rcall delay_100ms	;
	rcall delay_50ms	; end of 1.25 s delay
	OUT PORTB, R16		; turn LED off 
	RJMP MAIN			; jump back to top of code

	

delay_1s:
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	ret


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

delay_50ms:
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
		push r16
		push r18
		ldi  r16, 255
		ldi  r18, 6
delay_1b:
delay_1a:
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		dec r16
		brne delay_1a

		dec r18
		brne delay_1b
		 
		pop r18
		pop r16
		ret
	

	
