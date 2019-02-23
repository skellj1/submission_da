;
; Skellj1_DA1B.asm
;
; Created: 2/22/2019 4:22:18 PM
; Author : James Skelly

LDI xl, 0x00		// Load the lower bits of the start address into
						// the lower 8 bits of the X index register.
LDI xh, 0x02		// Load the upper bits of the start address into
						// the upper 8 bits of the X index register.
LDI R20, 100		// Load immediate value 100 into register 20.

LOOP:				// Loop label for loop to populate memory 
	st x+, R20		// Store the value of R20 into the memory location
						// of the X index register, and then increment
						// the value (memory address) stored in X by 1.
	INC R20			// Increment the value in R20 by 1.
	CPI xl, 0x63	// Compare the lower bits of the X register (memory
						// address) with the value 0x63. , or decimal 99.
	BRNE LOOP		// If 0x63 is not the value in the X reg, stay in loop.

END: RJMP END		// Terminates program
