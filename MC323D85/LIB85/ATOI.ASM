*
* Get a decimal number from a string
*
atoi	LXI	H,4		Address of argument
	CALL	?gstkw		Get stack address
	XCHG			DE = string address
* First, skip any whitespace preceeding the number
?1	LDAX	D		Get char
	INX	D		Advance
	CPI	' '		Space?
	JZ	?1		Yes, skip it
	CPI	9		Tab?
	JZ	?1		Yes, skip it
* Keep track of '-' sign for negative numbers
	LXI	H,0		Begin with zero
	CPI	'-'		Negative sign?
	PUSH	PSW		Save for later
	JZ	?2		Don't restore
	DCX	D		Backup to last char
* Add up the digits into a binary value
?2	LDAX	D		Get char
	SUI	'0'		Convert
	CPI	'0'+10		In range?
	JNC	?3		No, end
	MOV	B,H		Copy X1 value
	MOV	C,L		for later
	DAD	H		HL *= 2
	DAD	H		HL *= 4
	DAD	B		HL *= 5
	DAD	H		HL *= 10
	MOV	C,A		Get char value
	MVI	B,0		Clear high
	DAD	B		Include in result
	INX	D		Advance to next
	JMP	?2		And proceed
* If '-' was used. convert to negative
?3	POP	PSW		Restore codes
	JZ	?neg		Negate the value
	RET
