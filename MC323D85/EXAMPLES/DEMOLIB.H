/*
 * Due to significant memory architecture differences between the various
 * target processors, some versions of the Micro-C compiler use different
 * library function names when a object to be manipulated is located in
 * different types of memory. This is most often an issue with "harvard"
 * type processors which maintain separate CODE and DATA memory address
 * spaces. Some compilers and memory models store constant data in the
 * CODE address space, which require it to be accessed differently than
 * normal data.
 *
 * This file provides aliases to common library functions which allow a
 * single name to be used no matter where the constant data is located.
 *
 * Copyright 2001-2004 Dave Dunfield
 * All rights reserved.
 */

/* 8051 Medium/Large models store strings in code space */
#if (_CPU_ == 8051) && (_MDL_ >= 3)
	#define	Putstr	__putstr
	#define	Abort	__abort
	#define	Strcpy	strcpy__
	#define	Strcat	strcat__
	#define	Strcmp	strcmp__
	#define	Move	move__
	#define	_Move	_move__
#endif

/* Provide default definitions for any function not translated */
#ifndef Putstr
	#define	Putstr	putstr
#endif
#ifndef Abort
	#define	Abort	abort
#endif
#ifndef	Move
	#define	Move	move
#endif
#ifndef _Move
	#define	_Move	_move
#endif
#ifndef Strcpy
	#define	Strcpy	strcpy
#endif
#ifndef Strcat
	#define	Strcat	strcat
#endif
#ifndef	Strcmp
	#define	Strcmp	strcmp
#endif
