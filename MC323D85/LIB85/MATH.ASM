*
* Return ABSOLUTE VALUE of argument: int abs(int arg)
*
abs	LXI	H,4		Address of argument
	CALL	?gstkw		Get argument
	MOV	A,H		Get high value
	ANA	A		Is it negative?
	JM	?neg		Negate the value
	RET
*
* Return MAXIMUM of two arguments: int max(int arg1, int arg2)
*
max	LXI	H,6		Address of first argument
	CALL	?gstkw		Get argument
	MOV	B,H		B = HIGH arg1
	MOV	C,L		C = LOW arg1
	LXI	H,4		Address of second argument
	CALL	?gstkw		HL = arg2
	PUSH	H		Save HL
	PUSH	B		Save BC
	CALL	?scomp		Perform the compare
	JNC	?2		BC >= HL
?1	POP	B		Restore BC
	POP	H		Restore HL
	RET
*
* Return MINIMUM of two arguments: int min(int arg1, int arg2)
*
min	LXI	H,6		Address of first argument
	CALL	?gstkw		Get argument
	MOV	B,H		B = HIGH arg1
	MOV	C,L		C = LOW arg1
	LXI	H,4		Address of second argument
	CALL	?gstkw		HL = arg2
	PUSH	H		Save HL
	PUSH	B		Save BC
	CALL	?scomp		Perform the compare
	JNC	?1		BC >= HL
?2	POP	H		Restore BC into result
	POP	B		Clean up stack
	RET
*
* Return SQUARE ROOT of argument: int sqrt(int arg)
*
sqrt	LXI	H,4		Offset to parm
	CALL	?gstkw		Get it
	MOV	B,H		Copy HIGH
	MOV	C,L		Copy LOW
	LXI	H,255*255	Limit value
	CALL	?ucomp		Compare them
	JC	?3		<, its OK
	LXI	H,256		Assume 256*256
	RET
?3	LXI	H,0		Start with 0
?4	PUSH	H		Save count
	PUSH	B		Save limit
	MOV	B,H		Copy HIGH
	MOV	C,L		Copy LOW
	CALL	?mul		Perform mult
	POP	B		Recover limit
	CALL	?ucomp		BC < HL?
	POP	H		Get count back
	RZ			We have result
	RC			"" "" ""
	INX	H		Advance
	JMP	?4		And proceed
