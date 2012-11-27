*
* Concatinate several strings: register concat(dest, source, ...)
*
concat	PUSH	H		Save argument count
	INX	H		Skip return address
	DAD	H		Adjust for word values
	DAD	SP		Offset to stack
	INX	H		Advance to HIGH
	MOV	D,M		Get HIGH dest
	DCX	H		Backup
	MOV	E,M		Get LOW dest
* Get the next argument from the arg list
?1	POP	B		Get argument count
	DCR	C		Reduce argument count
	RZ			End of arguments
	PUSH	B		Save count
	DCX	H		Backup
	MOV	B,M		Get HIGH source
	DCX	H		Backup
	MOV	C,M		Get LOW source
* Append the string to the destination
?2	LDAX	B		Get char from source
	STAX	D		Write to dest
	ANA	A		End of string?
	JZ	?1		Yes, get next
	INX	B		Advance source
	INX	D		Advance dest
	JMP	?2		And proceed
