;;===============================
;;Name: Anna McAbee
;;===============================

.orig x3000
AND 	R4, R4, 0 	;clear R4
VALUE1 	ADD R4, R4, 1 	;default of r4 is 0
	AND R0, R0, 0	;clear R0
	LD R0, A	;A = R0
	AND R1, R1, 0	;clear R1	
	LD R1, B 	;B = R1
	NOT R3, R1 	;flipped B
	ADD R3, R3, 1	;increment B to get 2's comp
LOOP1 	ADD R0, R0, R3 	;a = a - b 
	BRp LOOP1 	;while a>b go back to first loop
	BRn END 	;if negative stop
ST R4, ANSWER 		;if 0 return 1
END 	HALT
	
A       .fill 25 ;25
B       .fill 5
ANSWER  .fill 0
.end
