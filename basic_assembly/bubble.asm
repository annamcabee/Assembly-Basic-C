;;===============================
;;Name: Anna McAbee
;;===============================

	.orig x3000
	AND R0, R0, 0 	; LENGTH 
	AND R1, R1, 0 	; index
	AND R2, R2, 0 	; ARRAY[i]
	AND R3, R3, 0 	; ARRAY [i+1]
	AND R4, R4, 0	; i - i + 1
	AND R5, R5, 0 	; max address index
	AND R6, R6, 0	; i - R5
	AND R7, R7, 0	;
INIT	LD R0, LENGTH	; R0 = LENGTH
	BRz STOP	; if length = 0, stp
	ADD R0, R0, #-1	; Decrement length because index must be less
	LD R1, ARRAY	; R1 = address of array
	ADD R5, R1, R0	; R5= address index with last array item
	NOT R5, R5	;
	ADD R5, R5, 1	; negate + add 1 for 2's complement
START	AND R0, R0, 0	; clear RO
	LD R1, ARRAY 	; R1 = start of ARRAY
	LDR R2, R1, 0 	; R2 = ARRAY[i]
	LDR R3, R1, 1	; R3 = ARRAY[i+1]
	NOT R3, R3 	; 
	ADD R3, R3, 1	; negate array [i + 1] to compare them
	ADD R4, R3, R2	; i - (i+1)
	BRp LOOP	; if i > i + 1, loop again
	BRnz LOOP2	; else, LOOP2
LOOP	ADD R0, R0, 1	; increment R0
	STR R2, R1, 1	; array[i] address altered
	NOT R3, R3	;
	ADD R3, R3, 1	; negate array[i+1]
	STR R3, R1, 0	; array[i+1] address altered
	ADD R1, R1, 1	; increment index
	ADD R6, R1, R5	; i ?> max adress index
	BRz CHECKSORT	; if i = max address index, checksort
	LDR R2, R1, 0	; i + 1
	LDR R3, R1, 1	; i + 1 + 1
	NOT R3, R3	;
	ADD R3, R3, 1	; negates next in array
	ADD R4, R3, R2	; (i + 1) - (i + 1 + 1)
	BRp LOOP
	BRnz LOOP2	;
LOOP2	ADD R1, R1, 1	; increment array address
	ADD R6, R1, R5	; i ?> max adress index
	BRz CHECKSORT	; if i = max address index, checksort
	LDR R2, R1, 0	; i + 1
	LDR R3, R1, 1	; i + 1 + 1
	NOT R3, R3	;
	ADD R3, R3, 1	; negate of R3
	ADD R4, R3, R2	; (i + 1) - (i + 1 + 1) = if positive lOOP
	BRp LOOP	
	BRnz LOOP2
	CHECKSORT
	ADD R0, R0, 0	; set NZP
	BRp START	; if counter > 0, start loop agian
	BRz STOP	; if counter = 0, stop

	STOP
	HALT

ARRAY   .fill x6000
LENGTH  .fill 12
.end

.orig x6000
.fill 28
.fill -50
.fill 7
.fill 0
.fill 216
.fill 4
.fill 15
.fill -82
.fill 34
.fill 101
.fill -5
.fill 61
.end

