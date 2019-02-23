/*
 * Skelly_James_DA1.asm
 *
 *  Created: 2/3/2019 3:11:23 PM
 *   Author: James Skelly
 */ 

 .ORG 0X00						; Sets the program to begin at memory location 0x00

 .EQU MULTIPLICAND = 0X7254		; Initializes the multiplicand (16-bit value)
 .EQU MULTIPLIER = 0X10			; Initializes the multiplier (8-bit value)

 LDI R25, HIGH(MULTIPLICAND)	; Places the higher 8-bits of the multiplicand into register 25
 LDI R24, LOW(MULTIPLICAND)		; Places the lower 8-bits of the multiplicand into register 24
 LDI R22, MULTIPLIER			; Loads the multiplier value into register 22
 LDI R21, MULTIPLIER			; Keeps a copy of the multiplier in register 21 for review when 
									;program terminates
 LDI R16, 0x00					; Places the value zero into register 16

 LOOP:							; Loop label for repeated (iterative) addition
	ADD R20, R24				; Begins repeated addition of the lower 8 bits of the multiplicand, 
									; places the value of the repeated addition in the first solution
									; register, R18
	ADC R19, R25				; Repeated addition of upper 8 bits, including addition of carry bit
									; from SREG, places result in R19, the second solution register
	ADC R18, R16				; Allocates bits 16-23 of the third solution register, initially adding 
									; the value 0 until further iterations, where it will begin to sum
									; up the carry values it receives from the ADC function and the SREG
									; carry bit
	DEC R22						; Decrements R17, the multiplier, by 1 on each iteration
	BRNE LOOP					; Branches back to the top of the LOOP subroutine until R17 is zero

	RJMP END					; Jumps to the END label

END: RJMP END					; Program terminates