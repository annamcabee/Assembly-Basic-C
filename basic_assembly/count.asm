;;===============================
;;Name: Anna McAbee
;;===============================

	.orig x3000
INIT	LD R1, LENGTH	;R1 stores a counter set to the value of LENGTH
	LD R2, ARRAY 	;R2 stores a pointer to the next address in ARRAY
	LD R3, #0
	AND R3, R3, #0	;R3 stores the current count of the number
	LD R0, NUMBER
	
LOOP	ADD R1, R1, 0	; Add 0 to the counter to set the NZP registers in accordance to 				; the counter's value
	BRnz END	;If the counter is not positive, end the loop
	
	ADD R1, R1, #-1		;Decrement the counter of where we are in the array
	LDR R4, R2, #0		;R4 stores the next value in ARRAY
	ADD R2, R2, #1		;Increment the address pointer

	NOT R5, R4	
	ADD R5, R5, #1		;R5 stores a negative copy of the current value
	ADD R5, R5, R0	;Subtract the current value in ARRAY from the NUMBER

	BRnp LOOP	;If the current value is not equal to 0, start loop over
	ADD R3, R3, #1	;Else increment the counter
	BR LOOP		;Start the loop over
	
END 	ST R3, ANSWER	;Store the count of the number in ANSWER
	HALT
	
NUMBER  .fill 9
ARRAY   .fill x6000
LENGTH  .fill 10
ANSWER	.fill 0
.end

.orig x6000
.fill 8
.fill 9
.fill 7
.fill 0
.fill -3
.fill 11
.fill 9
.fill -9
.fill 2
.fill 9
.end
