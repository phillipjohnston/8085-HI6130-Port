/*
 * Character classification macros for DDS MICRO-C
 *
 * These macros classify the passed character based on a table
 * lookup. The accepted range of character values which may be
 * tested is (0 to 127, and EOF).
 *
 * NOTE: These macro's use a large (129 byte) character classification
 * table. If you are doing only a few character classification operations,
 * you may wish to NOT include this file, and use the smaller functions
 * from the library.
 *
 * This file **REQUIRES** the extended pre-processor (MCP).
 * (CC... -P, or "Preprocess" step enabled in DDSIDE)
 *
 * Copyright 1990-2005 Dave Dunfield
 * All rights reserved.
 */

/* Type bits in _chartype_ */
#define IS_CTL	0x01		/* Control code */
#define IS_SPC	0x02		/* Space character */
#define IS_DIG	0x04		/* Numeric digit */
#define IS_UPP	0x08		/* Upper-case alphabetic */
#define IS_LOW	0x10		/* Lower-case alphabetic */
#define IS_HEX	0x20		/* Valid hex-alphabetic character */
#define IS_PUN	0x40		/* Non-graphic printable character */

extern const char _chartype_[];	/* Classification table */

/* Character classification macros */
#define	iscntrl(c)	(_chartype_[(c)+1] & IS_CTL)
#define	isspace(c)	(_chartype_[(c)+1] & IS_SPC)
#define	isdigit(c)	(_chartype_[(c)+1] & IS_DIG)
#define	isupper(c)	(_chartype_[(c)+1] & IS_UPP)
#define	islower(c)	(_chartype_[(c)+1] & IS_LOW)
#define	ispunct(c)	(_chartype_[(c)+1] & IS_PUN)
#define	isalpha(c)	(_chartype_[(c)+1] & (IS_UPP | IS_LOW))
#define	isxdigit(c)	(_chartype_[(c)+1] & (IS_DIG | IS_HEX))
#define	isalnum(c)	(_chartype_[(c)+1] & (IS_DIG | IS_UPP | IS_LOW))
#define	isgraph(c)	(_chartype_[(c)+1] & (IS_PUN | IS_DIG | IS_UPP | IS_LOW))
