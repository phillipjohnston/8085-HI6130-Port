*
* Shared format routine used by "printf", "sprintf" etc.
* INPUT: DE=ptr to args, BC=Output buffer
*
_format_ LXI	H,-8		Buffer size
	DAD	SP		Include stack
	SPHL			Adjust stack
	XCHG			HL=ptr to args
	MOV	D,M		Get HIGH byte of source
	DCX	H		Backup to LOW
	MOV	E,M		Get LOW byte of source
	DCX	H		Backup to previous
?1	LDAX	D		Get char from string
	INX	D		Advance to next
	CPI	'%'		Format specifier?
	JZ	?2		Yes, handle it
	STAX	B		Write to output
	INX	B		Advance to next
	ANA	A		End of string?
	JNZ	?1		No, keep going
	LXI	H,8		Buffer size
	DAD	SP		Include stack
	SPHL			Adjust stack
	RET
* Format specifier has been found
?2	PUSH	B		Save output pointer
	LXI	B,0		B=flags, C=width
* Test for '-' (Left justify)
	LDAX	D		Get next char
	CPI	'-'		Left justify?
	JNZ	?3		No, its OK
	INX	D		Skip '-'
	MVI	B,%10000000	Set left justify bit
* Test for leading '0' (Zero fill)
?3	LDAX	D		Get next char
	CPI	'0'		Zero fill?
	JNZ	?4		No, its OK
	MOV	A,B		Get flags
	ORI	%01000000	Set '0' flag
	MOV	B,A		And re-save
* Evaluate the field width
?4	LDAX	D		Get char
	SUI	'0'		Convert from ASCII
	CPI	10		In range?
	JNC	?5		No, its not
	INX	D		Advance
	PUSH	PSW		Save value
	MOV	A,C		Get current value
	RLC			x2
	RLC			x4
	ADD	C		x5
	RLC			x10
	MOV	C,A		Resave
	POP	PSW		Restore value
	ADD	C		Include old
	MOV	C,A		Resave
	JMP	?4		And proceed
* Interpret the format specifier character
?5	LDAX	D		Get specifier
	INX	D		Skip it
* Stack format & arg pointer, but leave output buffer on top
	XCHG			HL = format, DE = arg pointer
	XTHL			Stack it
	PUSH	H		ReSave output
	XCHG			Swap back
	MOV	D,M		Get HIGH value
	DCX	H		Backup to previous
	MOV	E,M		Get LOW value
	DCX	H		Backup to previous
	XTHL			Save ARG ptr
	PUSH	H		ReSave output
* 'u' Unsigned decimal number
	CPI	'u'		Unsigned decimal
	JZ	?6		Yes, handle it
* 'd' Signed decimal number
	CPI	'd'		Decimal
	JNZ	?7		No, try next
	MOV	A,D		Get value
	ANA	A		Test for negative
	JP	?6		No, its ok
	XCHG			HL = value
	CALL	?neg		Negate the value
	XCHG			DE = value
	POP	H		Get output
	MVI	M,'-'		Get minus sign
	INX	H		Advance pointer
	PUSH	H		Resave
	MOV	A,C		Get size
	ANA	A		Fixed format?
	JZ	?6		No, its OK
	DCR	C		Yes, adjust
?6	MVI	A,$0A		Set base 10
	JMP	?9		And process number
* 'x' Hexidecimal number
?7	CPI	'x'		Hexidecimal
	JNZ	?8		No, try next
	MVI	A,$10		Set base 16
	JMP	?9		And process number
* 'o' Octal number
?8	CPI	'o'		Octal
	JNZ	?12		Octal number
	MVI	A,$08		Set base 8
* Build string for value in DE using nemeric base in A
?9	LXI	H,13		Offset to end of buffer
	DAD	SP		Point to it
	MVI	M,0		Zero terminate
	PUSH	B		Save flags & size
	MOV	B,D		Copy high value
	MOV	C,E		Copy low value
	XCHG			DE = output pointer
	MOV	L,A		Save base
	MVI	H,0		Set high to zero
?10	PUSH	H		Save BASE
	CALL	?mod		HL = BC % HL, BC = BC / HL
	MOV	A,L		Get result
	POP	H		Restore BASE
	ADI	'0'		Convert to ASCII
	CPI	'9'+1		HEX digit?
	JC	?11		Its OK
	ADI	7		Convert HEX
?11	DCX	D		Backup
	STAX	D		Write it
	MOV	A,B		Get HIGH
	ORA	C		Test against LOW
	JNZ	?10		And proceed
	POP	B		Restore output pointer
	JMP	?14		And proceed
* 's' string
?12	CPI	's'		String?
	JZ	?14		Yes, handle it
* 'c' character
	LXI	H,11		Offset to buffer
	DAD	SP		Point to it
	MVI	M,0		Zero tail
	DCX	H		Backup
	MOV	M,E		Set char
	CPI	'c'		Character?
	JZ	?13		Yes, its OK
	MVI	M,'?'		Indicate unknown
?13	XCHG			DE = string
* Copy string to output with correct width and justification
?14	POP	H		HL = Output
	MOV	A,B		Get flags
	ANA	A		Right Justify?
	CP	?100		Yes, pad it
	PUSH	D		Save value
	PUSH	B		Save flags
	MVI	B,0		Initial count
?15	MOV	A,C		Get size of data
	ANA	A		No size specified?
	JZ	?16		Yes, do not enforce
	INR	B		Advance length
	CMP	B		In range?
	JC	?17		Over, exit
?16	LDAX	D		Get char
	ANA	A		End of string?
	JZ	?17		Yes, exit
	MOV	M,A		Save in dest
	INX	D		Advance source
	INX	H		Advance dest
	JMP	?15		And proceed
?17	POP	B		Restore flags
	POP	D		Restore value
	MOV	A,B		Get flags
	ANA	A		Left justify?
	CM	?100		Yes, pad it
	MOV	B,H		Get new output
	MOV	C,L		Get new output
	POP	H		Restore ARG pointer
	POP	D		Restore	FORMAT string
	JMP	?1		And proceed
*
* Pad the field using flags in B, and width in C
*
?100	MOV	A,C		Get specified width?
	ANA	A		0 = free format
	RZ			No need to justify
	MOV	A,B		Get flags
	ANI	%01000000	Zero fill?
	RRC			Shift into correct position
	RRC			Shift into correct position
	ORI	' '		Get ' ' or '0'
	PUSH	D		Save pointer
	PUSH	B		Save BC
	PUSH	PSW		Save char
	MVI	B,0		Zero count
?101	LDAX	D		Get char
	ANA	A		End of string
	JZ	?102		Yes, exit
	INX	D		Advance string
	INR	B		Advance count
	JMP	?101		And proceed
?102	POP	D		Get character
?103	MOV	A,B		Get actual length
	CMP	C		In range?
	JNC	?104		Its over
	MOV	M,D		Output a space
	INX	H		Advance output
	INR	B		Advance count
	JMP	?103		And proceed
?104	POP	B		Restore flags
	POP	D		Restire pointer
	RET
