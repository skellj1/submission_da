; Part1_Assembly.asm
;
; Created: 3/8/2019 4:34:23 PM
; Author : James Skelly
;

.ORG 0							; program memory origin is 0
JMP MAIN
.ORG 0x02						; interrupt subroutine origin is 2
JMP EX0_ISR

MAIN:
	LDI R20, HIGH(RAMEND)		; initialize stack 
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20

	LDI R16, 0xFF				; 11111111 into R16
	OUT PORTB, R16				; turn LED off to start

	LDI R20, 0x00				; load 0x00 into reg. 20
	STS EICRA, R20				; set interrupt to activate on low level shift
	SBI DDRB, 2					; set PORTB.2 as output
	SBI PORTD, 2				; set PORTD.2 high
	LDI R20, 1<<INT0			; load interrupt 0 shifted left once into r20
	OUT EIMSK, R20				; set external interrupt mask bit zero high
	SEI							; global interrupt set

HERE: JMP HERE					; wait for interrupt to occur
		
EX0_ISR:				; external interrupt 0 subroutine
	LDI R21, 0xFB		; 11111011 INTO R21
	LDI R16, 0xFF		; 11111111 into R16
	OUT PORTB, R21		; OUTPUT PORTB.2 LED 4 turns on
	rcall delay_1s		; call for delay of 1.25 s from delay subroutines
	rcall delay_100ms	;
	rcall delay_100ms	;
	rcall delay_50ms	; end of 1.25 s delay
	OUT PORTB, R16		; turn LED off 
	RETI				; jump back to address following interrupt


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
		
		ldi  r23, 255
		ldi  r24, 6
delay_1b:
delay_1a:
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		dec r23
		brne delay_1a

		dec r24
		brne delay_1b
		 
		ret
