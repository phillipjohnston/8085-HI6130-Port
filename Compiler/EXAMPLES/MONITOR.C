/*
 * A simple monitor program
 *
 * This program demonstrates how to perform simple interactive I/O,
 * and also shows error recovery using "setjmp" and "longjmp".
 *
 * Commands:
 *   M<start> [end]    - Dump memory
 *   S<addr> <data>... - Store into memory
 *   Q                 - Quit
 *   ?                 - Display commands
 */

#include <8085io.h>

unsigned char *optr;
int saveenv[2];

main()
{
	unsigned args[5], i, v;
	char c, d, buffer[25];
	putstr("Monitor");

	switch(setjmp(saveenv)) {
		case 1 : putstr("Invalid command");				break;
		case 2 : putstr("Invalid parameter");			break;
		case 3 : putstr("Invalid number of arguments");	break;
		case 4 : putstr("Aborted!");					}

	for(;;) {
		putstr("\n>");
		if(!getstr(optr = buffer, sizeof(buffer)-1))
			continue;
		if(!(c = toupper(skip())))
			continue;
		++optr;
		i = 0;
		while(skip()) {
			v = 0;
			while((d = toupper(*optr)) && !isspace(d)) {
				++optr;
				v <<= 4;
				if(isdigit(d))
					v += d - '0';
				else if((d >= 'A') && (d <= 'F'))
					v += d - ('A' - 10);
				else
					longjmp(saveenv, 2); }
			args[i++] = v; }

		switch(c) {
			case 'M' :					/* Dump memory */
				optr = *args;
				if(i == 1)
					args[1] = optr;
				else if(i != 2)
					longjmp(saveenv, 3);
				while(optr <= args[1]) {
					printf("\n%04x ", optr);
					for(i=0; i < 16; ++i)
						printf(" %02x", *optr++);
					if(chkch() == 0x1B)
						longjmp(saveenv, 4); }
				continue;
			case 'S' :					/* Store into memory */
				optr = *args;
				if(i < 2)
					longjmp(saveenv, 3);
				for(v = 1; v < i; ++v)
					*optr++ = args[v];
				continue;
			case '?' :					/* Help */
				putstr("M <start> <end>  - Dump memory\n");
				putstr("S <addr> <val>...- Store into memory\n");
				putstr("Q                - Quit");
				continue;
			case 'Q' :					/* Quit program */
				return; }
		longjmp(saveenv, 1); }
}

/*
 * Skip to next non-blank
 */
skip()
{
	while(isspace(*optr))
		++optr;
	return *optr;
}
