 ; Name: Anna McAbee


; Main
; Do not edit this function!

.orig x3000

	LD R6, STACK	; Initialize the stack

	LEA R0, STRING	; R0 = &str[0]
	ADD R1, R0, 0

SL_LOOP	LDR R2, R1, 0	; \ R1 = strlen(str)
	BRz SL_END	; |
	ADD R1, R1, 1	; |
	BR SL_LOOP	; |
SL_END	NOT R2, R0	; |
	ADD R2, R2, 1	; |
	ADD R1, R1, R2	; /

	ADD R6, R6, -2	; \ R0 = eval(str, len)
	STR R0, R6, 0	; |
	STR R1, R6, 1	; |
	LD R2, EVALPTR	; |
	JSRR R2		; |
	LDR R0, R6, 0	; |
	ADD R6, R6, 3	; /

	ST R0, ANS
	HALT

STACK	.fill xf000
ANS	.fill -1
EVALPTR	.fill EVAL
STRING	.stringz "2*4+1*8"
	.blkw 200

EVAL	ADD R6, R6, -1 	
	ADD R6, R6, -1	; leave room for return address & val
	STR R7, R6, 0 	; return address 
	ADD R6, R6, -1 	; room for old frame pointer
	STR R5, R6, 0 	; old FP = R5
	ADD R5, R6, -1 	; FP -> first local var
	ADD R6, R6, -3 	; room for local variables
	AND R4, R4, 0

W1	LDR R4, R5, 0
	LDR R1, R5, 5 	; R1 = len
	NOT R1, R1 	
	ADD R1, R1, 1
	ADD R1, R1, R4 	
	BRzp W 		; if i - len == 0, go to next loop
	LDR R2, R5, 4 	; R0 = string address
	ADD R2, R2, R4 	; string at str + i
	LDR R2, R2, 0 	; char at str + i

	

	LD R0, ADD_ASCII 	; subtract ascii value for +
	ADD R2, R2, R0 		; check if 0
	BRnp INC

	LDR R4, R5, 0
	LDR R0, R5, 4 	; R0 = string address
	ADD R6, R6, -2 	; move up 		
	STR R0, R6, 0 	; string stored on stack for param
	STR R4, R6, 1 	; i for param
	JSR EVAL
	LDR R3, R6, 0 	; R3 = left 
	ADD R6, R6, 3 	
	STR R3, R5, -1 	; store left on stack	
	ADD R6, R6, -2 	; move up 
	AND R0, R0, 0 	
	LDR R0, R5, 4 	; R0 = string address
	LDR R4, R5, 0
	ADD R0, R0, R4
	ADD R0, R0, 1
	STR R0, R6, 0 	; store first param
	LDR R2, R5, 5 	; R2 = len
	NOT R4, R4 
	ADD R2, R2, R4 	; now R2 holds len - i -1
	STR R2, R6, 1 	; store in the second parameter spot

	JSR EVAL
	LDR R2, R6, 0 	; R2 = right
	STR R2, R5, -2 	; store right on stack
	ADD R6, R6, 3 	
	LDR R3, R5, -1 	
	ADD R2, R2, R3 	; R0 = left + right
	STR R2, R5, 3 	; return value
	BR END


INC 	LDR R4, R5, 0 	;load i
	ADD R4, R4, 1 	;
	STR R4, R5, 0
	BR W1 

W	AND R4,R4, 0 	;reset i
	STR R4, R5, 0 	;store incremented i

W2	LDR R4, R5, 0
	LDR R1, R5, 5 	; length —> R1
	NOT R1, R1 	; make length negative
	ADD R1, R1, 1
	ADD R1, R1, R4 	; i - length
	BRzp END1	; done if i - len == 0

	LDR R2, R5, 4 	; string addr —> R0
	ADD R2, R2, R4 	; string at str + i
	LDR R2, R2, 0 	; R2 holds char at str + i

	LD R0, MULT_ASCII 	;put add char in R0
	ADD R2, R2, R0 		;check to see if add operator
	BRnp INC2

	LDR R4, R5, 0
	LDR R0, R5, 4 	; R0 = string address
	ADD R6, R6, -2 	; move up 2 spots
	STR R0, R6, 0 	; storing string on stack
	STR R4, R6, 1 	; store i for param also
	JSR EVAL	; evaluate
	LDR R3, R6, 0 	; R3 = left 
	ADD R6, R6, 3 	
	STR R3, R5, -1 	; store left 
	ADD R6, R6, -2 	; move up 

	AND R0, R0, 0 	; clear
	LDR R0, R5, 4 	; string addr = R0
	LDR R4, R5, 0
	ADD R0, R0, R4;
	ADD R0, R0, 1
	STR R0, R6, 0 	; store first param

	LDR R2, R5, 5 	; R2 = len
	NOT R4, R4 
	ADD R2, R2, R4 	; len - i -1
	STR R2, R6, 1 	; store second param
	JSR EVAL
	LDR R2, R6, 0 	; R2 = right
	STR R2, R5, -2 	; store right on stack
	ADD R6, R6, 3 	; pop
	LDR R3, R5, -1 	; R3 = LEFT
	AND R0, R0, 0 

AGAIN	ADD R0, R0, R3 	; left + right
	ADD R2, R2, -1
	BRp AGAIN
	STR R0, R5, 3 	; return value
	BR END


INC2	LDR R4, R5, 0 	
	ADD R4, R4, 1 
	STR R4, R5, 0
	BR W2

END1	LD R0, ZERO_ASCII
	LDR R3, R5, 4 		; string in R5
	LDR R3, R3, 0
	ADD R3, R3, R0
	STR R3, R5, 3 		; store value in return value
END	ADD R6, R5, 3 		
	LDR R7, R5, 2 		; put return address in R7
	LDR R5, R5, 1 		; back to old frame pointer
	RET

		


MULT_ASCII 	.fill	-42
ADD_ASCII 	.fill	-43
ZERO_ASCII	 .fill	-48



.end
