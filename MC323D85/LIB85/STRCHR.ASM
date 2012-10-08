*
* Search for a character in a string: strchr(string, char)
*
strchr	LXI	H,6		Search string
	CALL	?gstkw		Get argument
	XCHG			DE = string
	LXI	H,4		Character
	CALL	?gstkw		HL = search character
?1	LDAX	D		Get character from string
	CMP	L		Does it match
	JZ	?2		Yes, we  have it
	INX	D		Skip to next
	ANA	A		End of string?
	JNZ	?1		No, keep looking
	LXI	D,0		Indicate not found
?2	XCHG			HL = address
	RET
