*
* Allocate a block of memory: char *malloc(size)
*
malloc	LXI	H,4		Address of parm
	CALL	?gstkw		Get size of block
	PUSH	H		Save size
* Search for a free block of memory
	LXI	H,?heap		Point to start
?1	MOV	A,M		Get flag byte
	INX	H		Advance
	MOV	C,M		Get LOW size
	INX	H		Advance
	MOV	B,M		Get HIGH size
	INX	H		Advance
	ANA	A		End of list?
	JZ	?5		Yes, handle it
	DCR	A		De-allocated?
	JNZ	?2		No, do not use
* De-allocated block, see if it is large enough
	XTHL			HL = size
	MOV	D,H		Copy HIGH
	MOV	E,L		Copy LOW
	PUSH	H		Save size
	CALL	?ucomp		Is BC >= HL
	POP	H		Restore size
	XTHL			HL = address
	JNC	?3		Yes, its OK
* This block is NOT suitable, advance to next
?2	DAD	B		Adjust by size
	JMP	?1		And proceed
* This block is OK to re-allocate
?3	PUSH	H		Save address
	MOV	H,D		HL = size
	MOV	L,E
	CALL	?sub		HL = remainder
	LXI	B,3		Threshold for splitting
	PUSH	H		Save remainder
	CALL	?ucomp		BC < HL ?
	POP	B		Restore remainder
	POP	H		Get address
	JNC	?4		Too small to break up
* Split this block into two blocks
	PUSH	H		Resave address
	DAD	D		Offset to new space
	MVI	M,$01		Indicate free
	INX	H		Advance
	DCX	B		Adjust for TYPE
	DCX	B		Adjust for size LOW
	DCX	B		Adjust for size HIGH
	MOV	M,C		Write LOW size
	INX	H		Advance
	MOV	M,B		Write HIGH size
	JMP	?6		And proceed
* Block is too small to split, just re-allocate
?4	POP	B		Remove size
	DCX	H		Skip HIGH size
	DCX	H		Skip LOW size
	JMP	?7		Set type & exit
* Allocate on end of memory
?5	POP	B		Get size
	PUSH	B		Resave
	PUSH	H		Save address
	DAD	B		Offset for new SIZE
	MOV	B,H		BC = new end
	MOV	C,L
	LXI	H,-1024		Margin value
	DAD	SP		Get stack address
	CALL	?ucomp		NEW END < SP
	JNC	?8		No, return zero (failure)
* Ok to proceed & allocate memory
	SUB	A		Terminator
	STAX	B		Zero terminate chain
* Patch this block to show the correct size
?6	POP	H		Get address back
	POP	D		Get new size
	DCX	H		Backup to HIGH address
	MOV	M,D		Write it
	DCX	H		Backup to LOW address
	MOV	M,E		Write it
?7	DCX	H		Backup to allocation flag
	MVI	M,$02		Write it
	INX	H		Move pointer
	INX	H		Back to the
	INX	H		Actual memory block
	RET
* Cannot allocate block, return 0
?8	POP	H		Clean stack
	POP	H		Clean stack
	LXI	H,0		Return 0
	RET
*
* Release a block of memory: free(ptr)
*
free	LXI	H,4		Argument address
	CALL	?gstkw		Get argument
	DCX	H		Backup
	DCX	H		To
	DCX	H		Start of block
	MOV	B,H		BC = block
	MOV	C,L
* Search the allocation list for this block
	LXI	H,?heap		Heap begins here
?9	MOV	A,M		Get byte from list
	ANA	A		End of list?
	JZ	?11		Yes, not found
	PUSH	H		Save value
	CALL	?ucomp		Compare the values
	POP	H		Restore value
	JZ	?10		Match, do this
	INX	H		Advance to size
	MOV	E,M		Get LOW size
	INX	H		Advance to size
	MOV	D,M		Get HIGH size
	INX	H		Advance to data
	DAD	D		Adjust to next
	JMP	?9		And proceed
* Mark this block as un-allocated
?10	MVI	M,1		Clear the flag
*
* Garbage collection, scan the memory allocation blocks, and
* convert any contiguous un-allocated blocks into single areas.
* Also, truncate the list at the last allocated block.
*
?11	LXI	H,?heap		Start here
* Check this block for allocation state
?12	MOV	A,M		Get flag
	ANA	A		End of list?
	RZ			Yes, exit
	INX	H		Advance to size
	MOV	D,H
	MOV	E,L
	MOV	C,M		Get LOW size
	INX	H		Advance to size
	MOV	B,M		Get HIGH size
	INX	H		Advance to data
	DCR	A		De-allocated?
	JNZ	?15		No, advance
* This block is de-allocated, check the next one
?13	DAD	B		Advance to next
	MOV	A,M		Get allocation state
	ANA	A		End of list?
	JZ	?16		Yes, truncate
	DCR	A		Allocated?
	JNZ	?14		Yes, stop looking
* Next block is also free, join them
	INX	H		Advance to next
	MOV	A,M		Get LOW size
	ADD	C		Include existing count
	MOV	C,A		Resave
	INX	H		Advance to next
	MOV	A,M		Get HIGH size
	ADC	B		Include existing count
	MOV	B,A		Resave
	INX	B		Include flag byte
	INX	B		Include LOW size byte
	INX	B		Include HIGH size byte
	JMP	?13		And continue
* Resave this block size
?14	XCHG			Get Address back
	MOV	M,C		Write LOW size
	INX	H		Advance to high
	MOV	M,B		Write HIGH size
	INX	H		Advance to data
* Advance to next block
?15	DAD	B		Skip data block
	JMP	?12		And go again
* Mark this block as end of list
?16	DCX	D		Backup to flag
	SUB	A		Get zero
	STAX	D		Zero terminate
	RET
