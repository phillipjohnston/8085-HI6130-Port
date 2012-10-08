/*
 * This macro allows you to use a 'C' function to handle an 8085 interrupt.
 *
 * The only parameter is the DECIMAL address of the interrupt vector.
 *
 * The macro MUST be placed IMMEDIATELY preceeding the definition of the
 * function which is to be the interrupt handler. Example:
 *
 * 		INTERRUPT(_SER_) serial_int_handler()
 * 		{
 *			/* ... Function code to handle serial interrupt ... */
 *		}
 *
 *
 * BEFORE USING THIS MACRO... you MUST modify the startup code to leave
 * room for the interrupt vectors ($0004 - $0020), Use something like
 * this at the beginning of the "prefix" file:
 *
 *					JMP		?start		Jump around vectors
 *					ORG		$0020		Start code after vectors
 *			?start	EQU		*			Startup code begins here
 *
 * This file **REQUIRES** the extended pre-processor (MCP).
 * (CC85 ... -P, or "Preprocess" step enabled in DDSIDE)
 *
 * Copyright 1991-2005 Dave Dunfield
 * All rights reserved.
 */
#define	_SP_				/* Null separator */
#define	INTERRUPT(vec) asm {\
$SE:0						/* Insure output in code segment */\
_/**/vec	/* This label will be the address of the "prefix" stub */\
		ORG		vec			/* Position to interrupt vector */\
		DI					/* Inhibit further interrupts */\
		JMP		_/**/vec	/* Jump to handler */\
		ORG		_/**/vec	/* Position back to original PC */\
		PUSH	PSW			/* Save ACC and condition codes */\
		PUSH	B			/* Save B+C */\
		PUSH	D			/* Save D+E */\
		PUSH	H			/* Save H+L */\
		LHLD	_SP_?temp	/* Get temporary location */\
		PUSH	H			/* Save it as well */\
		CALL	_/**/vec+21	/* Call 'C' interrupt handler */\
		POP		H			/* Restore temporary location */\
		SHLD	_SP_?temp	/* Save for later */\
		POP		H			/* Restore H+L */\
		POP		D			/* Restore D+E */\
		POP		B			/* Restore B+C */\
		POP		PSW			/* Restore ACC+Condition codes */\
		EI					/* Re-allow interrupts */\
		RET					/* We now return to original programming */\
}
#undef _SP_
