*
* Test for an ASCII character
*
isascii	LXI	D,$0080		Range to test
	JMP	?3		Perform test
*
* Test for alphanumeric character (0-9, A-Z, a-z)
*
isalnum	POP	B		BC = ret address
	CALL	isdigit		Is it a number?
	PUSH	B		Replace ret address
	RZ			Found it
*
* Test for alphabetic character (A-Z, a-z)
*
isalpha	POP	B		BX = ret address
	CALL	isupper		Test uppercase
	PUSH	B		Replace ret address
	RZ			Found it
*
* Test for lowercase alphabetic character (a-z)
*
islower	LXI	D,'az'+1	Range to test
	JMP	?3		Perform test
*
* Test for uppercase alphabetic character (A-Z)
*
isupper	LXI	D,'AZ'+1	Range to test
	JMP	?3		Perform test
*
* Test for graphic (printable) character
*
isgraph	LXI	D,$217F		Range to test
	JMP	?3		And proceed
*
* Test for hexidecimal digit
*
isxdigit POP	B		BC = ret address
	LXI	D,'AF'+1	Range to test
	CALL	?3		Test it
	PUSH	B		Fix stack
	RZ			Found it
	POP	B		BC = ret address
	LXI	D,'af'+1	Range to test
	CALL	?3		Test ot
	PUSH	B		Fix stack
	RZ			Found it
*
* Test for an ASCII digit (0-9)
*
isdigit	LXI	D,'09'+1	Range to test
* Test argument against range in DE
?3	LXI	H,4		Argument address
	CALL	?gstkw		get argument
* Test HL against the range in DE
?4	MOV	A,L		Get character
	CMP	D		too LOW?
	JC	?2		Yes, its invalid
	CMP	E		Too ?HIGH
	JNC	?2		Yes, its invalid
* Complete test by insuring that high bits are zero
?1	MOV	A,H		Get high value
	LXI	H,1		Assume its TRUE
	ANA	A		Test for zero high
	RZ			Yes, its OK
* Return with FALSE value
?2	LXI	H,0		Get a FALSE
	RET
*
* Test for CONTROL character
*
iscntrl	LXI	H,4		Argument address
	CALL	?gstkw		Get argument
	MOV	A,L		Get lower
	CPI	$7F		Delete?
	JZ	?1		Yes, this is it
	LXI	D,$0020		Control code range
	JMP	?4		Test for it
*
* Test for a punctuation character
*
ispunct	LXI	H,4		Argument address
	CALL	?gstkw		Get argument
	PUSH	H		Save value
	CALL	isalnum		Alphanumeric?
	POP	H		Clean up stack
	JZ	?2		Yes, false
isprint	LXI	D,$207F		Printable range
	JMP	?3		And test for it
*
* Test for a "space" character
*
isspace	LXI	H,4		Argument address
	CALL	?gstkw		Get argument
	MOV	A,L		Get char
	CPI	' '		Is it SPACE?
	JZ	?1		Yes, it is
	CPI	9		Is it TAB?
	JZ	?1		Yes it is
	CPI	10		Is it NEWLINE?
	JZ	?1		Yes it is
	JMP	?2		Report error
