*
* Test for a character from the console: getchr()
*
chkchr	IN	$81		Read status
	ANI	00000010	Received character?
	JNZ	?1		Yes, handle it
	DCR	A		Convert to -1
	MOV	L,A		Zero LOW result
	MOV	H,A		Zero HIGH result
	RET
*
* Read a character from the 8251 serial port: getch()
*
getchr	IN	$81		Read status
	ANI	%00000010	Received character?
	JZ	getchr		No, wait for it
?1	IN	$80		Read the data
	MOV	L,A		Return in HL
	MVI	H,0		Zero high
	RET
*
* Output a character to the serial port: putch(char c)
*
putchr	LXI	H,4		Offset to first parm
	CALL	?gstkb		Get the value
?2	IN	$81		Read 8251 status
	RRC			CY = TX ready?
	JNC	?2		No, wait for it
	MOV	A,L		Get char
	OUT	$80		Write to port
	RET
