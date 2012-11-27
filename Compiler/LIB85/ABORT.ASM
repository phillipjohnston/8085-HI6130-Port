*
* Issue message & exit the program
*
abort	POP	H		Remove RET address
	CALL	putstr		Output the string
	JMP	exit		And terminate
$EX:putstr
