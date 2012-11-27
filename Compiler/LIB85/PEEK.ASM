* Read a BYTE of memory: int peek(address)
peek	LXI	H,4		Address of argument
	CALL	?gstkw		Get argument
	MOV	L,M		Read value
	MVI	H,0		Zero high
	RET
* Read a WORD of memory: int peekw(address)
peekw	LXI	H,4		Address of argument
	CALL	?gstkw		Get argument
	MOV	A,M		Read LOW byte of value
	INX	H		Advance to next
	MOV	H,M		Read HIGH byte of value
	MOV	L,A		Include LOW
	RET
* Write a BYTE to memory: poke(address, data)
poke	LXI	H,4		Address of last argument
	CALL	?gstkw		Get value to write
	XCHG			DE = value
	LXI	H,6		Address of first argument
	CALL	?gstkw		HL = address to write
	MOV	M,E		Write to memory
	RET
* Write a WORD to memory: pokew(address, data)
pokew	LXI	H,4		Address of last argument
	CALL	?gstkw		Get value to write
	XCHG			DE = value
	LXI	H,6		Address of first argument
	CALL	?gstkw		HL = address to write
	MOV	M,E		Write to memory
	INX	H		Advance to next
	MOV	M,D		Write to high
	RET
