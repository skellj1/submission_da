;
; Skellj1_DA1B_part2.asm
;
; Created: 2/22/2019 4:22:18 PM
; Author : James Skelly

// PART 1 CODE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LDI xl, 0x00		// Load the lower bits of the start address into
						// the lower 8 bits of the X index register.
LDI xh, 0x02		// Load the upper bits of the start address into
						// the upper 8 bits of the X index register.
LDI R20, 20		// Load immediate value 20 into register 20.

LOOP1:				// Loop label for loop to populate memory 
	ST x+, R20		// Store the value of R20 into the memory location
						// of the X index register, and then increment
						// the value (memory address) stored in X by 1.
	INC R20			// Increment the value in R20 by 1.
	CPI xl, 0x63	// Compare the lower bits of the X register (memory
						// address) with the value 0x63. , or decimal 99.
	BRNE LOOP1		// If 0x63 is not the value in the X reg, stay in loop.

// PART 2 CODE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

LDI xl, 0x00		// Load memory address 0x0200 into index register X.
LDI xh, 0x02

LDI yl, 0x00		// Load memory address 0x0400 into index register Y.
LDI yh, 0x04		

LDI zl, 0x00		// Load memory address 0x0600 into index register Z.
LDI zh, 0x06

LOOP2:				// Loop label for loop that will separate numbers that
						// are divisible by 3 and numbers that are not divisible
						// by 3.
	LD R22, x+		// Load the value stored in the memory location currently
						// stored in the X index register, then increment X.
	MOV	R25, R22	// Store a copy of the value pulled from memory in R25
	LDI R23, 3		// Place the number to be divided (repeatedly subtracted)
						// into a separate register, R23.
	RCALL mod_divide		// Call the modulus division function. 
	CPI R22, 0			// Compare the value in R22, the result of the mod division, 
						// with the value zero to test for divisibility by 3.
	BREQ Y_REG		// Branch to Y_REG subroutine if the result of mod division is 0.
	ST z+, R25		// Store the value from R25, the copy of the number pulled from
						// memory, into the memory location currently stored in the 
						// Z index register, and then increment the value stored in Z.
	CPI xl, 0x63	// Compare the value in the lower X index register with the maximum
						// memory location, 0x63.
	BREQ PART3		// If the X register has reached its maximum memory location, branch
						// to the END label.
	RJMP LOOP2		// Stay in the loop until all of the values have been pulled from the 
						// memory locations held by the X index register. 

Y_REG:				// Subroutine to store numbers divisible by 3 in memory starting at 0x0400.
	ST y+, R25		// Store the value from R25 in the memory location currently stored in the 
						// Y index register, and then increment the value stored in Y.
	CPI xl, 0x63	// Check to see if X has reached the maximum memory location.
	BREQ PART3		// Branch to the END label if X has reached its maximum memory location.
	RJMP LOOP2		// Jump back to the loop if all the values have not been tested for divisibility.

mod_divide:						// Modulus division function to check for divisibility by 3.
	again:						// Again subroutine within mod_divide to repeatedly subtract.
		MOV R24, R22			// Copies the remaining value into R24 after subraction.
		SUB R22, R23			// Subtracts 3 from the value pulled from memory
		BRLT LessThanZero		// If the difference is less than zero, branch to less than zero sub.
		CPI	R22, 0				// Compare the remaining number with zero.
		BREQ EqualToZero		// Branch to equal to zero sub if the number remaining is zero.
		RJMP again				// Jump back to the top of the again sub if the number remaining is
									// greater than zero.

	LessThanZero:			// Label for less than zero subroutine.
		MOV R22, R24		// Move the result of the mod division into R22.
		RJMP DONE			// Jump to DONE

	EqualToZero:			// Equal to zero sub, R22 will be holding the value 0
		RJMP DONE		    // Jump to DONE
	
	DONE:
		RET					// Return to the next line to be executed after RCALL


// PART 3 CODE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

PART3:
	
	LDI yl, 0x00	// Load memory address 0x0400 back into Y.
	LDI yh, 0x04
	LDI zl, 0x00	// Load memory address 0x0600 back into Z.
	LDI zh, 0x06
	LDI R16, 0x0	// Initialize the divisible by 3 sum reg to 0.
	LDI R18, 0x0	// Initiliaze the not div. by 3 sum reg to 0.

LOOP3:
		
	LD R22, y+		// Load the value from the current mem. address of Y.
	ADD R16, R22	// Add the value to the running sum for div by 3 values.
	ADC R17, R0		// Push carry into R17
	LD R25, z+		// Load the value from the current mem. address of Z.
	ADD R18, R25	// Add the value to the running sum for not div by 3 values.
	ADC R19, R0		// Push carry into R19
	CPI zl, 0x42	// Compare low value of Z to upper bound of Z memory
	BREQ END		// Final branch to end
	RJMP LOOP3		// Stay in loop if the sum is not completed
	
END: RJMP END		
		

