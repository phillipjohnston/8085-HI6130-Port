*
* Generate a pseudo-random number: int rand(limit)
*
rand	LXI	H,4		Offset to parm
	CALL	?gstkw		Get the value
	PUSH	H		Save for later
	LHLD	RANDSEED	Get the current seed
	LXI	B,13709		First calculation
	CALL	?mul		Do it
	LXI	B,13849		Second calculation
	DAD	B		Do it
	SHLD	RANDSEED	Resave seed
	MOV	B,H		Copy HIGH
	MOV	B,L		Copy LOW
	POP	H		Get limit
	JMP	?mod		Calculate the limit
$DD:RANDSEED 2
