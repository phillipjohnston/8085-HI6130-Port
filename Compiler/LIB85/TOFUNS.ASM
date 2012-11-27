*
* Convert character to UPPER case
*
toupper	LXI	H,4		Address of argument
	CALL	?gstkw		Get argument
	PUSH	H		Pass argument
	CALL	islower		Is it lower case?
	MOV	A,H		Get HIGH
	ORA	L		Test for TRUE/FALSE
	POP	H		Restore value
	JZ	?1		No, its OK
	LXI	D,-$20		Adjustment
	DAD	D		Fix character
?1	RET
*
* Convert a character to LOWER case
*
tolower	LXI	H,4		Address of argument
	CALL	?gstkw		Get argument
	PUSH	H		Pass argument
	CALL	isupper		Is it upper case?
	MOV	A,H		Get HIGH
	ORA	L		Test for TRUE/FALSE
	POP	H		Restore value
	JZ	?1		No, its OK
	LXI	D,$20		Adjustment
	DAD	D		Fix character
	RET
$EX:islower
$EX:isupper
