*
* Set up the 8254 Timer and 8251 Uart for
* serial operation at the desired speed: serinit(baud, control)
*
serinit	LXI	H,6		Offset to first parm
	CALL	?gstkw		Get value
* Initialize th 8254 Timer
	MVI	A,%10110110	Timer 2, Mode 3
	OUT	$8B		Write to MODE port
	MOV	A,L		Get LOW value
	OUT	$8A		Write TIMER 2
	MOV	A,H		Get HIGH value
	OUT	$8A		Write TIMER 2
* Initialize the 8251 Uart
	LXI	H,4		Offset to second parm
	CALL	?gstkw		Get value
	XRA	A		Get a ZERO
	OUT	$81		Clear out 8251
	OUT	$81		"" "" ""
	MVI	A,%01000000	Internal RESET
	OUT	$81		Write 8251 Control
	MOV	A,H		Get HIGH value
	OUT	$81		Write 8251 control
	MOV	A,L		Get LOW value
	OUT	$81		Write 8251 control
	RET
