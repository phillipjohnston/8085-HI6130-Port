*
* Set memory contents: memset(dest, value, size)
*
memset	LXI	H,4		Offset to last arg
	CALL	?gstkw		Get size
	MOV	B,H		BC = size
	MOV	C,L
	LXI	H,6		Offset to second arg
	CALL	?gstkw		Get value
	XCHG			DE = value
	LXI	H,8		Offset to first arg
	CALL	?gstkw		Get dest
?1	MOV	A,B		Get HIGH size
	ORA	C		All done?
	RZ			Yes, quit
	MOV	M,E		Write one location
	INX	H		Advance dest
	DCX	B		Reduce count
	JMP	?1		And proceed
*
* Copy memory contents: memcpy(dest, source, size)
*
memcpy	LXI	H,4		Offset to last arg
	CALL	?gstkw		Get size
	MOV	B,H		BC = size
	MOV	C,L
	LXI	H,6		Offset to second arg
	CALL	?gstkw		Get source
	XCHG			DE = source
	LXI	H,8		Offset to first arg
	CALL	?gstkw		Get dest
?2	MOV	A,B		Get HIGH size
	ORA	C		All done?
	RZ			Yes, quit
	LDAX	D		Get byte from source
	MOV	M,A		Write to dest
	INX	D		Advance source
	INX	H		Advance dest
	DCX	B		Reduce count
	JMP	?2		And proceed
