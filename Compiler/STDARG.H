/*
 * DDS MICRO-C variable function argument definitions
 *
 * Note the following for MICRO-C functions with a variable # of args:
 * - The listed arguments are the LAST ones (not the first)
 * - A function declared "register" will have an argument count
 *   passed in the processor accumulator, which can be used to
 *   calculate the address of the first parameter.
 *
 * Example of macro use:
 *	va_func function(char *args)	/* Declare function */
 *	{	va_list list;				/* Define arg list pointer */
 *		va_start(list, args);		/* Setup arg list pointer */
 *		... va_arg(list, int);		/* Get next arg from list */
 *		va_end(list); }				/* Clean up */
 *
 * Notes:
 * -va_start() must be called FIRST (before any non-declarative code).
 * -Since the listed function arguments are the LAST ones, the first use
 *  of "va_arg" will retrieve the FIRST argument to the function.
 *
 * This file **REQUIRES** the extended pre-processor (MCP).
 * (CC... -P, or "Preprocess" step enabled in DDSIDE)
 *
 * Copyright 1999-2005 Dave Dunfield
 * All rights reserved.
 */
#define	va_func	register
#define	va_end(l)
#if _CPU_ == 8051
	#define	va_list			register unsigned*
	#define	va_start(l, a)	l=-(nargs()*2)+&a
	#define	va_arg(l, t)	((t)*++l)
#else
	#define	va_list			unsigned*
	#define va_start(l, a)	l=(nargs()*2)+&a
	#define va_arg(l, t)	((t)*--l)
#endif
