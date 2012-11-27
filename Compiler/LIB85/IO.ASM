*
* Read a byte from an I/O port: in(address)
*
in	LXI	H,4		Address or parm
	CALL	?gstkw		Get address
	MVI	A,$DB		'IN' instruction
	MVI	H,$C9		'RET' instruction
	STA	?IOtmp		Write first byte
	SHLD	?IOtmp+1	Write last two
	CALL	?IOtmp		Perform the IN
	MOV	L,A		Get the value
	MVI	H,0		clear high
	RET
*
* Write a byte to an I/O port: out(address, data)
*
out	LXI	H,6		Address of first parm
	CALL	?gstkw		HL = address
	MVI	A,$D3		'OUT' instruction
	MVI	H,$C9		'RET' instruction
	STA	?IOtmp		Write first byte
	SHLD	?IOtmp+1	Write last two
	LXI	H,2		Address of last parm
	DAD	SP		Offset to it
	MOV	A,M		Get the value
	JMP	?IOtmp		Output & return
$DD:?IOtmp 3
