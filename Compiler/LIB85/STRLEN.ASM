*
* Get the length of a string: int strlen(string)
*
strlen	LXI	H,4		String address
	CALL	?gstkw		Get argument
	XCHG			DE = string ptr
	LXI	H,0		Begin with count of  zero
?1	LDAX	D		Get char from string
	ANA	A		End of string?
	RZ			Yes, return with count
	INX	D		Advance string
	INX	H		Advance count
	JMP	?1		And proceed
