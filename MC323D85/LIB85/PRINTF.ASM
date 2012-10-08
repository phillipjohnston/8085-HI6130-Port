*
* Write a string to the console using formatting information
*
printf	DAD	H		x2 for words
	DAD	SP		HL=ptr to args
	INX	H		Offset to HIGH byte
	XCHG			DE=ptr to args
	LXI	H,-100		Size of output buffer
	DAD	SP		Include stack position
	SPHL			Adjust stack
	MOV	B,H		Set HIGH
	MOV	C,L		Set LOW
	PUSH	B		Save for later
	CALL	_format_	Do the deed
	CALL	putstr		Output the string
	LXI	H,102		Remove local vars
	DAD	SP		Include stack position
	SPHL			Adjust stack
	RET
$EX:_format_
$EX:putstr
