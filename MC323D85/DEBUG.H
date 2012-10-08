/*
 * Debugging macros for MICRO-C
 *
 * These macros can be inserted into C source code to provide "printf"
 * formatted debugging output, which can be enabled only when debugging
 * messages are desired. When not enabled, the macro's do not generate
 * any code, allowing you to leave them in production source code.
 *
 * The macro's must be called with DOUBLE brackets around the parameters,
 * and should NOT be followed by a semicolon (;).
 * Three levels of debug messages are provided:
 *	Debug((printf parms))		<- Highest level (1)
 *	Debug1((printf parms))		<- Medium  level (2)
 *	Debug2((printf parms))		<- Lowest  level (3)
 *
 * Before #including this file, you must: #define DEBUG <n>
 * where <n> is the level of debug messages desired:
 *	1 : Only the highest (level 1) messages
 *	2 : Highest and medium (levels 1 & 2) messages
 *	3 : All debugging messages (levels 1, 2 & 3)
 *
 * If the symbol DEBUG is not defined, all of the macros become null
 * definitions which will not generate any code.
 *
 * This file **REQUIRES** the extended pre-processor (MCP).
 * (CC... -P, or "Preprocess" step enabled in DDSIDE)
 *
 * Copyright 1999-2005 Dave Dunfield
 * All rights reserved.
 */

#ifdef DEBUG
	#if (DEBUG < 1) || (DEBUG > 3)
		#error DEBUG must be in range 1-3
	#endif
	#define Debug(a) printf a;
#else
	#define	Debug(a)
#endif

#if DEBUG > 1
	#define Debug1(a) printf a;
#else
	#define Debug1(a)
#endif

#if DEBUG > 2
	#define Debug2(a) printf a;
#else
	#define Debug2(a)
#endif
