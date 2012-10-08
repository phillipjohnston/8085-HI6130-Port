*
* Set a long jump in INTERNAL memory: setjmp(int jaddr[2])
*
setjmp	POP	B		Remove return address
	LXI	H,0		Begin with zero
	DAD	SP		HL = Stack pointer
	PUSH	B		Resave return address
	XCHG			DE = stack pointer
	LXI	H,4		Offset to parameter
	CALL	?gstkw		Get dest address
	MOV	M,C		Save LOW ret address
	INX	H		Advance
	MOV	M,B		Save HIGH ret address
	INX	H		Advance
	MOV	M,E		Save SP low
	INX	H		Advance
	MOV	M,D		Save SP high
	LXI	H,0		Return ZERO
	RET
*
* Perform a long jump via INTERNAL memory: longjmp(int jaddr[2], int rc)
*
longjmp	LXI	H,4		Offset to VALUE
	CALL	?gstkw		Get value
	XCHG			DE = value
	LXI	H,6		Offset to block
	CALL	?gstkw		Get Block
	MOV	C,M		Get LOW ret address
	INX	H		Advance
	MOV	B,M		Get HIGH ret address
	INX	H		Advance
	MOV	A,M		Get LOW SP
	INX	H		Advance
	MOV	H,M		Get HIGH SP
	MOV	L,A		Set up low
	SPHL			Set new stack
	PUSH	B		Save return address
	XCHG			HL = value
	RET
