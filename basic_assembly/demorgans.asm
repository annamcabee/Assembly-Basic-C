;;===============================
;;Name: Anna McAbee
;;===============================

	.orig x3000
START	LD R1, A
	LD R2, B

	NOT R1, R1 	; Not of variable A
	NOT R2, R2 	; Not of variable B
	
	ADD R1, R1, #1	;Add 1 to make it a valid 2's complement number 
	ADD R2, R2, #1	;Add 1 to make it a valid 2's complement number 


	AND R3, R1, R2 	; And ~A and ~B
	NOT R4, R3 	; ~(~A & ~B)

	ST R4, ANSWER	; Store Final answer in label ANSWER
	HALT

A       .fill 6
B       .fill 11
ANSWER  .fill 0
.end
