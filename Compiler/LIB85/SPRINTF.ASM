*
* Perform formatted output to a string
*
sprintf	DAD	H		x2 for words
	DAD	SP		HL=ptr to args
	MOV	C,M		Get LOW string
	INX	H		Advance to HIGH
	MOV	B,M		Get HIGH string
	DCX	H		Backup to LOW
	DCX	H		Backup to previous
	XCHG			DE=ptr to args
	JMP	_format_	Do the deed
$EX:_format_
