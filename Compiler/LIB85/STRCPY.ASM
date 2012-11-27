*
* Concatinate to a string: strcat(dest, source)
*
strcat	LXI	H,6		Destination string
	CALL	?gstkw		Get argument
?1	MOV	A,M		Get char from source
	ANA	A		End of string?
	JZ	?2		Yes, it is
	INX	H		Advance to next
	JMP	?1		And proceed
*
* Copy a string: strcpy(dest, source)
*
strcpy	LXI	H,6		Destination string
	CALL	?gstkw		Get argument
?2	XCHG			DE = Dest
	LXI	H,4		Dest string
	CALL	?gstkw		Get argument
?3	MOV	A,M		Get char from source
	STAX	D		Write to dest
	INX	H		Advance source
	INX	D		Advance dest
	ANA	A		End of string?
	JNZ	?3		No, do them all
	DCX	D		Point to terminating zero
	XCHG			Return pointer to dest
	RET
