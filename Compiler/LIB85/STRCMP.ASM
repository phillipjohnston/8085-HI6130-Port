*
* Compare two strings
*
strcmp	LXI	H,6		Offset to first arg
	CALL	?gstkw		Get the value
	XCHG			DE = string1
	LXI	H,4		Offset to second arg
	CALL	?gstkw		HL = string2
?1	LDAX	D		Get value
	CMP	M		Same as dest?
	JNZ	?2		No, its not
	INX	D		Advance source
	INX	H		Advance dest
	ANA	A		End of strings?
	JNZ	?1		No, keep testing
	MOV	H,A		Zero HIGH
	MOV	L,A		Zero LOW
	RET
?2	LXI	H,-1		[DE] < [HL]
	RC
	LXI	H,1		[DE] > [HL]
	RET
