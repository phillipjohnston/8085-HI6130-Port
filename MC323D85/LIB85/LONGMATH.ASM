*
* Functions to perform basic arithmetic operations on arbitrarily
* large unsigned numbers. Supports: + - * / % <<1 >>1 == != < >
*
* To work on other than 32 bits, adjust the following constants:
?LSIZE	EQU	4		4 bytes (32 bits)
$DD:Longreg	4
*
* Get parameters into: DE = source, HL = dest
*
?parm2	LXI	H,7		Offset to arguments
	DAD	SP		Adjust to SP
	MOV	D,M		Get HIGH of num1
	DCX	H		Backup
	MOV	E,M		Get LOW of num1
	DCX	H		Backup
?1	MOV	A,M		Get HIGH of num2
	DCX	H		Backup
	MOV	L,M		Get LOW of num2
	MOV	H,A		Set high
	RET
*
* Get single parameter into HL
*
?parm1	LXI	H,5		Offset to argument
	DAD	SP		Adjust SP
	JMP	?1		And get it
*
* Add two LONG numbers: longadd(num1, num2)
*
longadd	CALL	?parm2		Get num1 & num2
?bigadd	MVI	C,?LSIZE	Get length
	ANA	A		No carry in
?2	LDAX	D		Get source
	ADC	M		Add in dest
	STAX	D		Resave
	INX	D		Advance num1
	INX	H		Advance num2
	DCR	C		Decrement count
	JNZ	?2		Do them all
?3	MOV	A,C		Get zero
	RAL			Get CY
	MOV	L,A		Set LOW
	MOV	H,C		Set HIGH
	RET
*
* Subtract two LONG numbers: longsub(num1, num2)
*
longsub	CALL	?parm2		Get num1 and num2
?bigsub	MVI	C,?LSIZE	Get length
	ANA	A		No carry in
?4	LDAX	D		Get source
	SBB	M		Subtract dest
	STAX	D		Resave
	INX	D		Advance num1
	INX	H		Advance num2
	DCR	C		Reduce count
	JNZ	?4		Do them all
	JMP	?3		And get result
*
* Shift LONG number left: longshl(num1)
*
longshl	CALL	?parm1		Get address
?bigshl	ANA	A		No carry in
?bigslc	MVI	C,?LSIZE	Get size
?5	MOV	A,M		Get value
	RAL			Shift left
	MOV	M,A		Resave
	INX	H		Advance
	DCR	C		Reduce count
	JNZ	?5		Do them all
	JMP	?3		And fetch result
*
* Shift a LONG number right: longshr(num1)
*
longshr	CALL	?parm1		Get address
?bigshr	LXI	B,?LSIZE	Get size (zero B)
	DAD	B		Adjust to end (no carry)
?6	DCX	H		Backup pointer
	MOV	A,M		Get byte
	RAR			Rotate
	MOV	M,A		Resave
	DCR	C		Reduce count
	JNZ	?6		Do them all
	JMP	?3		And return result
*
* Copy a LONG number to another: longcpy(num1, num2)
*
longcpy	CALL	?parm2		Get num1 and num2
?bigcpy	MVI	C,?LSIZE	Get size
?7	MOV	A,M		Get byte from num2
	STAX	D		Save in num1
	INX	D		Advance num2
	INX	H		Advance num2
	DCR	C		Reduce count
	JNZ	?7		Do them all
	RET
*
* Zero the temporary register
*
?8	LXI	H,Longreg	Point to register
?9	MVI	C,?LSIZE	Get size
?10	MVI	M,0		Write zero to memory
	INX	H		Advance pointer
	DCR	C		Reduce count
	JNZ	?10		Do them all
	RET
*
* Set a LONG number: longset(ptr, value)
*
longset	CALL	?parm2		Get num1 and value
	XCHG			HL = num1, DE = num2
	PUSH	H		Save pointer
	CALL	?9		Zero the long
	POP	H		Restore pointer
	MOV	M,E		Write LOW value
	INX	H		Advance to next
	MOV	M,D		Write high value
	RET
*
* Test a LONG value for zero: longtst(num1)
*
longtst	CALL	?parm1		Get pointer
?bigtst	MVI	C,?LSIZE	Get size
	XRA	A		Get a zero
?11	ORA	M		Is it zero?
	JNZ	?12		No, report its not
	INX	H		Advance pointer
	DCR	C		Reduce count
	JNZ	?11		Do them all
?12	MOV	L,A		NZ chr in low
	MOV	H,C		Count in high
	RET
*
* Compare two LONG numbers: longcmp(num1, num2)
*
longcmp	CALL	?parm2		Get num1 and num2
?bigcmp	LXI	B,?LSIZE	Get byte count (and zero B)
	DAD	B		num2 += ?LSIZE
	XCHG			For 16 bit + in H
	DAD	B		num1 += ?LSIZE
?13	DCX	H		Backup num1 value
	DCX	D		Backup num2 value
	LDAX	D		Get num2 value
	CMP	M		Compare with num1
	JC	?14		num1 > num2
	JNZ	?15		num1 < num2
	DCR	C		Reduce count
	JNZ	?13		Do them all
	MOV	H,C		HIGH = 0
	MOV	L,C		LOW = 0
	RET
?14	LXI	H,1		Return 1 (num1 > num2)
	RET
?15	LXI	H,-1		Return -1 (num1 < num2)
	RET
*
* Multiply two LONG numbers: longmul(num1, num2)
*
longmul	LXI	H,-?LSIZE	Get local variable size
	DAD	SP		Get new value
	SPHL			Reset stack pointer
	XCHG			DE = Dest (num3)
	LXI	H,?LSIZE+4	Point to num2
	CALL	?gstkw		HL = [SP+HL]
	CALL	?bigcpy		num3 = num2
	CALL	?8		Zero temp register
?16	LXI	H,?LSIZE+6	Point to num1
	CALL	?gstkw		HL = [SP+HL]
	PUSH	H		Save num1 ptr
	CALL	?bigshr		Shift it
	ANA	A		Test for carry
	JZ	?17		Do not add
	LXI	D,Longreg	Point to temp reg
	LXI	H,2		Skip saved value
	DAD	SP		HL = num3
	CALL	?bigadd		Longreg += num3
?17	POP	H		Restore num1 ptr
	CALL	?bigtst		Is it zero?
	JZ	?18		Yes, exit
	LXI	H,0		Start with zero
	DAD	SP		HL = num3
	PUSH	H		Save num3 pointer
	CALL	?bigshl		Shift it left
	POP	H		Restore num3 ptr
	CALL	?bigtst		Is it zero
	JNZ	?16		No, keep going
?18	LXI	H,?LSIZE	Get size
	DAD	SP		Adjust SP
	SPHL			Resave it
	LXI	H,6		Point to num1
	CALL	?gstkw		HL = [SP+HL]
	XCHG			Dest in DE
	LXI	H,Longreg	Point to long reg
	JMP	?bigcpy		num1 = Longreg
*
* Divide LONG numbers: longdiv(num1, num2)
*
longdiv	CALL	?8		Zero temp register
	MVI	B,(?LSIZE*8)+1	Get bit count
?19	ANA	A		No carry in
?20	PUSH	PSW		Save for later
	LXI	H,8		Offset to num1
	CALL	?gstkw		Get parameter
	POP	PSW		restore carry
	CALL	?bigslc		Shift it
	DCR	B		Reduce count
	RZ			Hit zero, exit
	LXI	H,Longreg	Point to temp
	RAR			Set CY based on last shift
	CALL	?bigslc		Shift it
	LXI	H,4		Point to num2
	CALL	?gstkw		Get parameter
	PUSH	H		Save for later
	LXI	D,Longreg	Point at temp reg
	PUSH	B		Preserve our counter
	CALL	?bigcmp		Do the compare
	POP	B		Restore our counter
	MOV	A,H		Get high result
	ANA	A		Test for <
	POP	H		Restore pointer
	JNZ	?19		And continue
	LXI	D,Longreg	Point at temp
	CALL	?bigsub		Longreg -= num2
	STC			Set carry in
	JMP	?20		And proceed
