*
* DDS MICRO-C Serial I/O functions.
* These routines are set up to talk to an 8251 device at I/O address $80.
*
* Test for a character from the console
*
chkch	IN	$81		Read status
	ANI	%00000010	Received character?
	JNZ	?1		Yes, handle it
	MOV	L,A		Zero LOW result
	MOV	H,A		Zero HIGH result
	RET
*
* Read a character from the 8251 serial port: getch()
*
getch	IN	$81		Read status
	ANI	%00000010	Received character?
	JZ	getch		No, wait for it
?1	IN	$80		Read the data
	MOV	L,A		Return in HL
	MVI	H,0		Zero high
	CPI	$0D		Carriage return?
	RNZ			No, its OK
	MVI	L,$0A		Convert to NEWLINE
	RET
*
* Output a character to the serial port: putch(char c)
*
putch	LXI	H,4		Offset to first parm
	CALL	?gstkb		Get the value
?2	IN	$81		Read 8251 status
	RRC			CY = TX ready?
	JNC	?2		No, wait for it
	MOV	A,L		Get char
	OUT	$80		Write to port
	CPI	$0A		Newline?
	RNZ			No, its ok
	MVI	L,$0D		Append a CR
	JMP	?2		And proceed
*
* Write a string to the serial port: putstr(char * s)
*
putstr	LXI	H,4		Offset to first parm
	CALL	?gstkw		Get address of string
	XCHG			DE = String
?3	LDAX	D		Get char
	ANA	A		End of string?
	RZ			Yes, exit
	MOV	L,A		Save for later
	CALL	?2		Write the character
	INX	D		Advance to next
	JMP	?3		And proceed
*
* Read a string from the serial port: getstr(char *s, int l)
*
getstr	LXI	H,6		Offset to first parm
	CALL	?gstkw		Get string address
	XCHG			DE = string buffer
	LXI	H,4		Offset to secomd parm
	CALL	?gstkw		HL = size
	LXI	B,0		Starting length
	PUSH	H		Save size
?4	CALL	getch		Get a char
	MOV	A,L		Read translated char
	CPI	$08		Backspace?
	JZ	?6		Yes, handle it
	CPI	$7F		Delete?
	JZ	?6		Yes, handle it
	POP	H		Restore size
	CPI	$0A		Newline
	JZ	?5		Handle it
	STAX	D		Save in buffer
	PUSH	H		Save size
	CALL	?ucomp		Test for size
	JNC	?4		Ignore
	LDAX	D		Get char back
	MOV	L,A		Copy for loutput
	CALL	?2		Echo the character
	INX	D		Point to next
	INX	B		Advance size
	JMP	?4		And proceed
* NEWLINE received, end of string
?5	XRA	A		String terminator
	STAX	D		Save it
	MVI	L,$0A		Newline
	CALL	?2		Echo & exit
	MOV	H,B		Get size HIGH
	MOV	L,C		Get size LOW
	RET
* BACKSPACE or DELETE
?6	MOV	A,B		Get high if size
	ORA	C		Any data in buffer
	JZ	?4		Ignore delete code
	PUSH	D		Save pointer
	LXI	D,?7		Point to message
	CALL	?3		Echo it
	POP	D		Restore pointer
	DCX	D		Backup pointer
	DCX	B		Reduce size
	JMP	?4		And proceed
?7	DB	$08,' ',$08,0	Wipe away last char
