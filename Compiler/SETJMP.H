/*
 * DDS MICRO-C Setjmp/Longjmp definitions
 *
 * The data area 'jmp_buf', used by the 'setjmp' and 'longjmp' functions
 * contains CPU specific data required to transfer control from longjmp
 * back to setjmp. For this reason, the definition of this area differs
 * from one CPU (and memory model) to the next, requiring the use of a
 * large conditional block to correctly define it in all cases.
 *
 * This file **REQUIRES** the extended pre-processor (MCP).
 * (CC... -P, or "Preprocess" step enabled in DDSIDE)
 *
 * Copyright 1988-2005 Dave Dunfield
 * All rights reserved.
 */
#ifndef _CPU_		/* Micro-C/PC */
	#define jmp_buf struct{Jmp_Buf[3];}
#elif (_CPU_==6808)||(_CPU_==6809)||(_CPU_==6811)||(_CPU_==6812)||(_CPU_==8085)||(_CPU_=='CF')
	#define jmp_buf struct{Jmp_Buf[2];}
#elif (_CPU_==6816)||(_CPU_==8086)||(_CPU_==8096)||(_CPU_=='AV')
	#define	jmp_buf struct{Jmp_Buf[3];}
#elif _CPU_==8051
	#if (_MDL_==0) || (_MDL_==1) || (_MDL_==3)	/* Tiny, Small & Medium */
		#define _jmp_buf register struct{char Jmp_Buf[3];}
		#if _MDL_								/* Small & Medium only */
			#define	jmp_buf struct{char Jmp_Buf[3];}
		#endif
	#elif (_MDL_==2) || (_MDL_==4)				/* Compact & Large */
		#define _jmp_buf register struct{char Jmp_Buf[5];}
		#define jmp_buf struct{char Jmp_Buf[5];}
	#else
		#message "_MDL_" = _MDL_
		#error _MDL_ setting (Memory Model) is invalid.
	#endif
#else
	#message "_CPU_" = _CPU_
	#error _CPU_ setting (CPU type) is invalid.
#endif
