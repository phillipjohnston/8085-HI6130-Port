/*
* Remote Terminal Library
* rt.c
*
* Phillip Johnston
* 8 October 2012
*/


// Holt project headers
#include "defs.h"
#include "rt.h"
#include "regdefs.h"

extern struct rt_reg_map_t * pH6130; //This pointer originally went to the master reg struct.  We just need inidividuals.

#if(RT1_ena)
extern struct rt1_d_table * pRT1d;
extern struct rt1_illcmd_table * pRT1i;
#endif // (RT1_ena)

#if(RT2_ena)
extern struct rt2_d_table * pRT2d;
extern struct rt2_illcmd_table * pRT2i;
#endif // (RT2_ena)


#if (RT1_ena)    //------------ RT1 ENABLED ------------


// 	This function initializes the Holt HI-613x RT1 by writing 
//	configuration registers in the device. Only RT1 mode option 
//	bits are affected. The program has already called function 
// 	initialize_613x_shared() to initialize the common parameters
//	shared by BC, RT1, RT2 and/or Bus Monitor
//
void initialize_6130_RT1(void) {
	

	unsigned int k;
	unsigned int i,j;
	
	static unsigned int descr_table_RT1[512] = {
		/* this array is used to initialize the Descriptor Table. For subaddress-
	receive and subaddress-transmit commands, the array sets the desired data 
	buffer style and initializes data pointer values. 
	
	For mode code commands, use of the "Simplified Mode Command Processing" 
	option is assumed, so the only potential initialization in the mode 
	command half of the table is loading mode data word values for transmit 
	mode commands. 
	
	Only 3 defined mode commands actually transmit a mode data word; MC16, 
	M18 and MC19 decimal. For mode commands MC18 and MC19, the device 
	automatically transmits the correct data word value, NOT fetched from 
	this table. The transmitted value is copied into the table after transmit.
	For MC16, the transmitted value comes from this table.
	
	Only 3 defined mode commands actually receive a mode data word: MC17, 
	MC20 and MC21 decimal. If the terminal is not using "illegal command 
	detection" it will respond "in form" to all valid undefined, reserved or 
	unimplemented mode commands. By providing storage for all possible mode
	commands, the Descriptor Table provides predictable command response to
	all valid mode code commands, as well as a repository for received data. */

		/* ===================================================================== */
		/*  R T 1   R E C E I V E    S U B A D D R E S S   C O M M A N D S       */
		/* ===================================================================== */
		/*  Note:Subaddresses    ControlWord  DescrWord2  DescrWord3  DescrWord4 */
		/*  0,31 are not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD, // Ctrl Word offset = 0x0000
		/*  Rx Subaddress 01  */   0x0004,     0x0800,     0x0822,     0x0844, // ping-pong
		/*  Rx Subaddress 02  */   0x8000,     0x0A00,     0x0020,     0x0D10, // idx-32 ixeqz  @@ 08D0
		/*  Rx Subaddress 03  */   0x8001,     0x1176,     0x1176,     0x15B6, // cir1 ixeqz (32 MSG BUFFER)
		/*  Rx Subaddress 04  */   0x8042,     0x1E00,     0x1E00,     0x1C00, // cir2-8 ixeqz (256 MSG MAX!)
		/*  Rx Subaddress 05  */   0x0000,     0x1A36,     0x0000,     0x1A36, // ----   
		/*  Rx Subaddress 06  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 07  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 08  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 09  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 10  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 11  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 12  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 13  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 14  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 15  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 16  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 17  */   0x0000,     0x1A36,     0x0000,     0x1A36, // shared index sgl-msg
		/*  Rx Subaddress 18  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 19  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 20  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 21  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 22  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 23  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 24  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 25  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 26  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 27  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 28  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 29  */   0x0000,     0x1A36,     0x0000,     0x1A36, // ----
		/*  Rx Subaddress 30  */   0x4000,     0x08AE,     0x0000,     0x08AE, // idx sgl-msg IWA irq
		/*  This row not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD, 
		/*                                                                       */
		/* ===================================================================== */
		/*  R T 1  T R A N S M I T   S U B A D D R E S S   C O M M A N D S       */
		/* ===================================================================== */
		/*  NOTE: IF BROADCAST COMMANDS ARE SUPPORTED, ACCIDENTAL BROADCAST-     */
		/*  TRANSMIT COMMANDS WILL UPDATE MIW + TT WORDS AT THE BROADCAST DATA   */
		/*  POINTER LOCATION (PING-PONG & INDEXED MODES) SO INITIALIZE B'CAST    */
		/*  POINTERS TO PREDICTABLE ADDRESSES, BUT NOT 0x0000 (CONFIG.REG 1!)    */
		/*                                                                       */
		/*  Note:Subaddresses    ControlWord  DescrWord2  DescrWord3  DescrWord4 */
		/*  0,31 are not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD, // Ctrl Word offset = 0x0080
		/*  Tx Subaddress 01  */   0x0004,     0x0866,     0x0888,     0x08AA, // ping-pong
		/*  Tx Subaddress 02  */   0x8000,     0x0D32,     0x0020,     0x1172, // idx-32 ixeqz
		/*  Tx Subaddress 03  */   0x8001,     0x15D6,     0x15D6,     0x1A16, // cir1 ixeqz
		/*  Tx Subaddress 04  */   0x8052,     0x1E00,     0x1E00,     0x1C00, // cir2-16 ixeqz
		/*  Tx Subaddress 05  */   0x0000,     0x1A58,     0x0000,     0x1A58, // ----
		/*  Tx Subaddress 06  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 07  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 08  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 09  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 10  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 11  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 12  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 13  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 14  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 15  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 16  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 17  */   0x0000,     0x1A58,     0x0000,     0x1A58, // shared index sgl-msg
		/*  Tx Subaddress 18  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 19  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 20  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 21  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 22  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 23  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 24  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 25  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 26  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 27  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 28  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 29  */   0x0000,     0x1A58,     0x0000,     0x1A58, // ----
		/*  Tx Subaddress 30  */   0x0000,     0x08AE,     0x0000,     0x08AE, // idx sgl-msg
		/*  This row not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD,
		/*                                                                       */
		/* ===================================================================== */
		/*  R T 1   R E C E I V E   M O D E   C O D E   C O M M A N D S          */
		/* ===================================================================== */
		/*                       ControlWord MsgInfoWord  TimeTagWord DataWord   */
		/* undefined Rx MC 00 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, // ----  // Ctrl Word offset = 0x0100
		/*     "     Rx MC 01 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 02 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 03 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |     
		/*     "     Rx MC 04 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 05 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 06 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 07 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 08 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, // shared index sgl-msg
		/*     "     Rx MC 09 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 10 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 11 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 12 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 13 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 14 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 15 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 16 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*  DEFINED  Rx MC 17 */   0x0004,     0x1AE4,     0x1AE8,     0x1AEC, /* synchronize with data */
		/* undefined Rx MC 18 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*     "     Rx MC 19 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*  DEFINED  Rx MC 20 */   0x0004,     0x1AF0,     0x1AF4,     0x1AF8, /* shutdown selected bus*/
		/*  DEFINED  Rx MC 21 */   0x0004,     0x1AFC,     0x1B00,     0x1B04, /* override sel bus shutdown*/
		/*  reserved Rx MC 22 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*     "     Rx MC 23 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 24 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 25 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 26 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*     "     Rx MC 27 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 28 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 29 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 30 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 31 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*                                                                       */
		/* ===================================================================== */
		/*  R T 1  T R A N S M I T   M O D E   C O D E   C O M M A N D S         */
		/*         using the "Simplified Mode Command Processing" option         */
		/* ===================================================================== */
		/*                       ControlWord MsgInfoWord  TimeTagWord DataWord   */ // Ctrl Word offset = 0x0180
		/*  DEFINED  Tx MC 00 */   0x4004,     0x1ABA,     0x1ABC,     0x1ABE, /* dynamic bus control,, IWA interrupt */
		/*     "     Tx MC 01 */   0x0004,     0x1A90,     0x1A92,     0x1A94, /* synchronize */
		/*     "     Tx MC 02 */   0x0004,     0x1A96,     0x1A98,     0x1A9A, /* transmit status */
		/*     "     Tx MC 03 */   0x0004,     0x1A9C,     0x1A9E,     0x1AA0, /* initiate self test */
		/*     "     Tx MC 04 */   0x0004,     0x1AA2,     0x1AA4,     0x1AA6, /* shutdown bus */
		/*     "     Tx MC 05 */   0x0004,     0x1AA8,     0x1AAA,     0x1AAC, /* overrride shutdown bus */
		/*     "     Tx MC 06 */   0x0004,     0x1AAE,     0x1AB0,     0x1AB2, /* inhibit terminal flag */
		/*     "     Tx MC 07 */   0x0004,     0x1AB4,     0x1AB6,     0x1AB8, /* override inhibit term flag */
		/*     "     Tx MC 08 */   0x6004,     0x1ABA,     0x1ABC,     0x1ABE, /* reset terminal */
		/*  reserved Tx MC 09 */   0x0000,     0x1A82,     0x0000,     0x1A82, // ----
		/*     "     Tx MC 10 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 11 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 12 */   0x0000,     0x1A82,     0x0000,     0x1A82, // shared index sgl-msg
		/*     "     Tx MC 13 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 14 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     | 
		/*     "     Tx MC 15 */   0x0000,     0x1A82,     0x0000,     0x1A82, // ----
		/*  DEFINED  Tx MC 16 */   0x0004,     0x1AC0,     0x1AC4,     0x1AC8, /* transmit vector word */
		/* undefined Tx MC 17 */   0x0000,     0x1A86,     0x0000,     0x1A86, // shared index sgl-msg
		/*  DEFINED  Tx MC 18 */   0x0004,     0x1ACC,     0x1AD0,     0x1AD4, /* transmit last command */
		/*  DEFINED  Tx MC 19 */   0x0004,     0x1AD8,     0x1ADC,     0x1AE0, /* transmit BIT word */
		/* undefined Tx MC 20 */   0x0000,     0x1A86,     0x0000,     0x1A86, // ----
		/*     "     Tx MC 21 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*  reserved Tx MC 22 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 23 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 24 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 25 */   0x0000,     0x1A86,     0x0000,     0x1A86, // shared index sgl-msg
		/*     "     Tx MC 26 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 27 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 28 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 29 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 30 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     | 
		/*     "     Tx MC 31 */   0x0000,     0x1A86,     0x0000,     0x1A86 }; // -- 

	/* end of descr_table_RT1[512] declaration */

#if (ILLEGAL_CMD_DETECT)
	static unsigned int illegal_table[256] = {
		/* This array is loaded by the initialization function only when the terminal
	uses "illegal command detection", that is, when the macro ILLEGAL_CMD_DETECT 
	= YES in the header file 613x_initialization.h. 
	
	When macro ILLEGAL_CMD_DETECT = NO, the default all-zeros table value after 
	/MR master reset is retained, so all valid commands	get an "in form" response.
	Terminals not using "illegal command detection" should preserve the RAM's 
	(all 0x0000) reset state for the table address range to provide consistent 
	"in form" response for all valid commands. This function call provides this.

	This template illegalizes all mode code commands that are either undefined 
	or reserved in MIL-STD-1553B. Also, the 3 transmit mode codes that have a 
	mode data word are made illegal when broadcast. Mode code 0 (dynamic bus 
	control) cannot be implemented by the HI-613X and is also made illegal.
	
	If using this example as a template, please note that the array below 
	renders the following list of commands illegal:

	* All undefined and reserved mode code commands
	* Broadcast versions of these transmit mode cmds: MC0 MC2 MC16 MC18 MC19
	* All broadcast transmit subaddress commands 
	* Tx mode code MC0 "dynamic bus control" because BC switch-over not programmed
	* Tx mode code MC3 "initiate self test," entirely application specific 
	
	******************** PLEASE NOTE NEXT 2 LINES ! *************************
	*    All non-broacast receive commands to subaddress 16, and all        *
	* transmit commands to subaddress 17 are illegal for demonstration only *
	*************************************************************************/

		/* ====================================================================== */
		/*       RT1 BROADCAST RECEIVE MODE CODE AND SUBADDRESS COMMANDS          */
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*       _____________________ MC15 - MC12 Rx (undefined)                 */
		/*      |  ____________________ MC11 - MC8 Rx (undefined)                 */
		/*      | |  ___________________ MC7 - MC4 Rx (undefined)                 */
		/*      | | |  __________________ MC3 - MC0 Rx (undefined)                */
		/*      | | | |  _________________ MC31 - MC28 Rx (reserved)              */
		/*      | | | | |  ________________ MC27 - MC24 Rx (reserved)             */
		/*      | | | | | |                                                       */
		/*      | | | | | |       __________ MC23 Rx (reserved)                   */
		/*      | | | | | |      |  _________ MC22 Rx (reserved)                  */
		/*      | | | | | |      | |  ________ MC21 Rx (override sel tx shutdown) */ 
		/*      | | | | | |      | | |  _______ MC20 Rx (selected tx shutdown)    */
		/*      | | | | | |      | | | |                                          */
		/*      | | | | | |      | | | |    ____ MC19 Rx (undefined)              */
		/*      | | | | | |      | | | |   |  ___ MC18 Rx (undefined)             */
		/*      | | | | | |      | | | |   | |  __ MC17 Rx (sync with data)       */
		/*      | | | | | |      | | | |   | | |  _ MC16 Rx (undefined)           */
		/*      | | | | | |      | | | |   | | | |                                */
		/*      | | | | | |  bin 1 1 0 0   1 1 0 1                                */
		/*      | | | | | |      -------   -------                                */
		/*   0x F F F F F F         C         D                                   */
		/*                                                                        */
		/*   Broadcast                                                            */
		/* Rx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0100 0x0101 0x0102 0x0103 0x0104 0x0105 0x0106 0x0107  */
		0xFFFF,0xFFCD,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0108 0x0109 0x010A 0x010B 0x010C 0x010D 0x010E 0x010F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0110 0x0111 0x0112 0x0113 0x0114 0x0115 0x0116 0x0117  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0118 0x0119 0x011A 0x011B 0x011C 0x011D 0x011E 0x011F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0120 0x0121 0x0122 0x0123 0x0124 0x0125 0x0126 0x0127  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0128 0x0129 0x012A 0x012B 0x012C 0x012D 0x012E 0x012F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0130 0x0131 0x0132 0x0133 0x0134 0x0135 0x0136 0x0137  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                              SA31 = SA0    */
		/* Rx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0138 0x0139 0x013A 0x013B 0x013C 0x013D 0x013E 0x013F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFCD,
		
		/* ====================================================================== */
		/*       RT1 BROADCAST TRANSMIT MODE CODE AND SUBADDRESS COMMANDS         */
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*    _________________ MC15 - MC12 Tx (reserved)                         */
		/*   |                                                                    */
		/*   |   ________________ MC11 Tx (reserved)                              */
		/*   |  |  _______________ MC10 Tx (reserved)                             */
		/*   |  | |  ______________ MC9 Tx (reserved)                             */
		/*   |  | | |  _____________ MC8 Tx (reset remote terminal)               */
		/*   |  | | | |                                                           */
		/*   |  | | | |   ____________ MC7 Tx (override inhibit term flag bit)    */
		/*   |  | | | |  |  ___________ MC6 Tx (inhibit term flag bit)            */
		/*   |  | | | |  | |  __________ MC5 Tx (override transmitter shutdown)   */
		/*   |  | | | |  | | |  _________ MC4 Tx (transmitter shutdown)           */
		/*   |  | | | |  | | | |                                                  */
		/*   |  | | | |  | | | |   _______ MC3 Tx (init self test) NOT IN DEMO PGM*/
		/*   |  | | | |  | | | |  |  ______ MC2 Tx (transmit status) NO BROADCAST */
		/*   |  | | | |  | | | |  | |  _____ MC1 Tx (synchronize)                 */
		/*   |  | | | |  | | | |  | | |  ____ MC0 Tx (dyn bus control)NO BROADCAST*/
		/*   |  | | | |  | | | |  | | | |                      and NOT IMPLEMENTED*/
		/*   |  | | | |  | | | |  | | | |   __ MC31 - MC28 Tx (reserved)          */
		/*   |  | | | |  | | | |  | | | |  |  __ MC27 - MC24 Tx (reserved)        */
		/*   |  | | | |  | | | |  | | | |  | |  __ MC23-22 (res) & MC21-20 (undef)*/ 
		/*   |  | | | |  | | | |  | | | |  | | |                                  */
		/*   |  | | | |  | | | |  | | | |  | | |   ____ MC19 Tx NO BROADCAST      */
		/*   |  | | | |  | | | |  | | | |  | | |  |  ___ MC18 Tx NO BROADCAST     */
		/*   |  | | | |  | | | |  | | | |  | | |  | |  __ MC17 Tx (undefined)     */
		/*   |  | | | |  | | | |  | | | |  | | |  | | |  _ MC16 Tx NO BROADCAST   */
		/*   |  | | | |  | | | |  | | | |  | | |  | | | |                         */
		/*   |  1 1 1 0  0 0 0 0  1 1 0 1  | | |  1 1 1 1                         */
		/*   |  -------  -------  -------  | | |  -------                         */
		/*  0xF    E        0        D     F F F     F                            */
		/*                                                                        */
		/*   Broadcast                                                            */
		/* Tx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0140 0x0141 0x0102 0x0143 0x0144 0x0145 0x0146 0x0147  */
		0xFE0D,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0148 0x0149 0x014A 0x014B 0x014C 0x014D 0x014E 0x014F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0150 0x0151 0x0152 0x0153 0x0154 0x0155 0x0156 0x0157  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0158 0x0159 0x015A 0x015B 0x015C 0x015D 0x015E 0x015F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0160 0x0161 0x0162 0x0163 0x0164 0x0165 0x0166 0x0167  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0168 0x0169 0x016A 0x016B 0x016C 0x016D 0x016E 0x016F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0170 0x0171 0x0172 0x0173 0x0174 0x0175 0x0176 0x0177  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                              SA31 = SA0    */
		/* Tx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0178 0x0179 0x017A 0x017B 0x017C 0x017D 0x017E 0x017F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFE0D,0xFFFF,	

		/* ====================================================================== */
		/* RT1 "OWN ADDRESS" NON-BROADCAST RECEIVE MODE CODE & SUBADDRESS COMMANDS*/
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*       _____________________ MC15 - MC12 Rx (undefined)                 */
		/*      |  ____________________ MC11 - MC8 Rx (undefined)                 */
		/*      | |  ___________________ MC7 - MC4 Rx (undefined)                 */
		/*      | | |  __________________ MC3 - MC0 Rx (undefined)                */
		/*      | | | |  _________________ MC31 - MC28 Rx (reserved)              */
		/*      | | | | |  ________________ MC27 - MC24 Rx (reserved)             */
		/*      | | | | | |                                                       */
		/*      | | | | | |       __________ MC23 Rx (reserved)                   */
		/*      | | | | | |      |  _________ MC22 Rx (reserved)                  */
		/*      | | | | | |      | |  ________ MC21 Rx (override sel tx shutdown) */ 
		/*      | | | | | |      | | |  _______ MC20 Rx (selected tx shutdown)    */
		/*      | | | | | |      | | | |                                          */
		/*      | | | | | |      | | | |    ____ MC19 Rx (undefined)              */
		/*      | | | | | |      | | | |   |  ___ MC18 Rx (undefined)             */
		/*      | | | | | |      | | | |   | |  __ MC17 Rx (sync with data)       */
		/*      | | | | | |      | | | |   | | |  _ MC16 Rx (undefined)           */
		/*      | | | | | |      | | | |   | | | |                                */
		/*      | | | | | |  bin 1 1 0 0   1 1 0 1                                */
		/*      | | | | | |      -------   -------                                */
		/*   0x F F F F F F         C         D                                   */
		/*                                                                        */
		/*  Own Address                                                           */
		/* Rx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0180 0x0181 0x0182 0x0183 0x0184 0x0185 0x0186 0x0187  */
		0xFFFF,0xFFCD,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0188 0x0189 0x018A 0x018B 0x018C 0x018D 0x018E 0x018F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0190 0x0191 0x0192 0x0193 0x0194 0x0195 0x0196 0x0197  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0198 0x0199 0x019A 0x019B 0x019C 0x019D 0x019E 0x019F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01A0 0x01A1 0x01A2 0x01A3 0x01A4 0x01A5 0x01A6 0x01A7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01A8 0x01A9 0x01AA 0x01AB 0x01AC 0x01AD 0x01AE 0x01AF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01B0 0x01B1 0x01B2 0x01B3 0x01B4 0x01B5 0x01B6 0x01B7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                             SA31 = SA0    */
		/* Rx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01B8 0x01B9 0x01BA 0x01BB 0x01BC 0x01BD 0x01BE 0x01BF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFCD,
		
		/* ====================================================================== */
		/* RT1 "OWN ADDRESS" NON-BROADCAST TRANSMIT MODECODE & SUBADDRESS CMMANDS */
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*    _________________ MC15 - MC12 Tx (reserved)                         */
		/*   |                                                                    */
		/*   |   ________________ MC11 Tx (reserved)                              */
		/*   |  |  _______________ MC10 Tx (reserved)                             */
		/*   |  | |  ______________ MC9 Tx (reserved)                             */
		/*   |  | | |  _____________ MC8 Tx (reset remote terminal)               */
		/*   |  | | | |                                                           */
		/*   |  | | | |   ____________ MC7 Tx (override inhibit term flag bit)    */
		/*   |  | | | |  |  ___________ MC6 Tx (inhibit term flag bit)            */
		/*   |  | | | |  | |  __________ MC5 Tx (override transmitter shutdown)   */
		/*   |  | | | |  | | |  _________ MC4 Tx (transmitter shutdown)           */
		/*   |  | | | |  | | | |                                                  */
		/*   |  | | | |  | | | |   _______ MC3 Tx (init self test) NOT IN DEMO PGM*/
		/*   |  | | | |  | | | |  |  ______ MC2 Tx (transmit status)              */
		/*   |  | | | |  | | | |  | |  _____ MC1 Tx (synchronize)                 */
		/*   |  | | | |  | | | |  | | |  ____ MC0 Tx (dyn bus control) CANNOT DO! */
		/*   |  | | | |  | | | |  | | | |                                         */
		/*   |  | | | |  | | | |  | | | |   __ MC31 - MC28 Tx (reserved)          */
		/*   |  | | | |  | | | |  | | | |  |  __ MC27 - MC24 Tx (reserved)        */
		/*   |  | | | |  | | | |  | | | |  | |  __ MC23-22 (res) & MC21-20 (undef)*/ 
		/*   |  | | | |  | | | |  | | | |  | | |                                  */
		/*   |  | | | |  | | | |  | | | |  | | |   ____ MC19 Tx (tx BIT word)     */
		/*   |  | | | |  | | | |  | | | |  | | |  |  ___ MC18 Tx (tx last command)*/
		/*   |  | | | |  | | | |  | | | |  | | |  | |  __ MC17 Tx (undefined)     */
		/*   |  | | | |  | | | |  | | | |  | | |  | | |  _ MC16 (tx vector word)  */
		/*   |  | | | |  | | | |  | | | |  | | |  | | | |                         */
		/*   |  1 1 1 0  0 0 0 0  1 0 0 1  | | |  0 0 1 0                         */
		/*   |  -------  -------  -------  | | |  -------                         */
		/*  0xF    E        0        9     F F F     2                            */
		/*                                                                        */
		/*  Own Address                                                           */
		/* Tx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x01C0 0x01C1 0x0102 0x01C3 0x01C4 0x01C5 0x01C6 0x01C7  */
		0xFE09,0xFFF2,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01C8 0x01C9 0x01CA 0x01CB 0x01CC 0x01CD 0x01CE 0x01CF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01D0 0x01D1 0x01D2 0x01D3 0x01D4 0x01D5 0x01D6 0x01D7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress  -----12-----  -----13-----  -----14-----  -----15-----  */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01D8 0x01D9 0x01DA 0x01DB 0x01DC 0x01DD 0x01DE 0x01DF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01E0 0x01E1 0x01E2 0x01E3 0x01E4 0x01E5 0x01E6 0x01E7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01E8 0x01E9 0x01EA 0x01EB 0x01EC 0x01ED 0x01EE 0x01EF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress  -----24-----  -----25-----  -----26-----  -----27-----  */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01F0 0x01F1 0x01F2 0x01F3 0x01F4 0x01F5 0x01F6 0x01F7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                             SA31 = SA0    */
		/* Tx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01F8 0x01F9 0x01FA 0x01FB 0x01FC 0x01FD 0x01FE 0x01FF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFE09,0xFFF2 };	
#endif
	//  End of illegal_table[256] declaration



	// ======================================================================================

	// These  parameters are defined in 613x_initialization.h

	i = RTTO_15U|NOTICE2|TRXDB|AUTO_SHUTDN|AUTO_SYNC|MC16OPT|AUTO_MC8_RESET;

	#if (!SUPPORT_BROADCAST)
	i |= BCASTINV;
	#endif

	#if (!UNDEF_MCODES_VALID)
	i |= UMCINV;
	#endif

	#if (USE_SMCP)
	i |= SMCP;
	#endif

	// HI-6130 uses host bus interface, HI-6131 uses host SPI interface. From 
	// here, we use bus interface to initialize HI-6130 registers and RAM tables,
	// or we use SPI to initialize HI-6131 registers and RAM tables

	// ======================================================================================

	pH6130->RT1_CONFIG_REG = i;

	// do not overwrite previously initialized common features
	j = pH6130->MASTER_CONFIG_REG & ~(RT1STEX);

	// if "bus shutdown" mode codes 4 & 20 disable Tx only but Rx still operates 
	// normally (NOT RECOMMENDED) then OR in BSDTXO, affecting Remote Terminals:
	// pH6130->MASTER_CONFIG_REG = j|RT1ENA|BSDTXO; 
	// otherwise use this...
	pH6130->MASTER_CONFIG_REG = j|RT1ENA;

	j = pH6130->HDW_INT_ENABLE_REG & ~(RT1TTM);
	// enable RT1 address parity fail interrupt, but not time tag match interrupt
	pH6130->HDW_INT_ENABLE_REG = j|RT1APF;
	
	j = pH6130->HDW_INT_OUTPUT_ENABLE_REG & ~(RT1TTM);
	// enable pin output for selected interrupts 
	pH6130->HDW_INT_OUTPUT_ENABLE_REG = j|RT1APF;
	
	// for demo, mode code 8 (reset RT) is handled differently for RT1 and RT2.
	// When MC8 occurs for RT1, the RT1_MC8 interrupt is decoded by this program's
	// interrupt handler, which asserts RT1RESET in reg 0x0002. When MC8 occurs for 
	// RT2, that terminal is configured for AUTO_MC8_RESET to automatically assert 
	// the RT2RESET in reg 0x0002 without host intervention.  
	j = pH6130->RT_INT_ENABLE_REG & 0xFC00;
	// enable RT1 interrupts, except the Message Error interrupt 
	pH6130->RT_INT_ENABLE_REG = j|RT1_IXEQZ|RT1_IWA|RT1_IBR|RT1_MC8;

	j = pH6130->RT_INT_OUTPUT_ENABLE_REG & 0xFC00;
	// enable pin output for selected RT1 interrupts
	pH6130->RT_INT_OUTPUT_ENABLE_REG = j|RT1_IXEQZ|RT1_IWA|RT1_IBR|RT1_MC8;
	
	pH6130->RT1_DESC_TBL_BASE_ADDR_REG = 0x0400; 
	pH6130->RT1_1553_STATUS_BITS_REG = 0x0000;	 
	pH6130->RT1_TTAG_UTILITY_REG = 0x0000;	 
	pH6130->RT1_BUSA_SELECT_REG = 0xAAAA;		 
	pH6130->RT1_BUSB_SELECT_REG = 0xBBBB;		 
	pH6130->RT1_BIT_WORD_REG = 0x0000;			 
	pH6130->RT1_ALT_BIT_WORD_REG = 0xABCD;			 


	// load the RT1 Descriptor Table
	// Starting at the table base address written above, copy the RT1 Descriptor 
	// Table array (declared above) into HI-6130 RAM.
	
	j = pH6130->RT1_DESC_TBL_BASE_ADDR_REG << 1;	// left-shift offset

	// If using simplified mode command processing (SMCP), the program is only 
	// required to initialize Descriptor Word 1 (Control Words) for each mode command 
	// in the 2nd half of the descriptor table. It is not necessary to initialize 
	// Descriptor Words 2,3,4 in the 2nd (mode code) half of the Descriptor Table.
	// THE FIRST HALF OF THE TABLE FOR SUBADDRESS COMMANDS MUST BE FULLY INITIALIZED. 

	#if (USE_SMCP)

	// in subaddress command half of table, every word is written
	for ( i = 0, k = (STARTING_ADDR + j); i < 256; i++) {
		*((unsigned int *)(k)) = descr_table_RT1[i];
		k += 2;
	}
	// in mode command half of table, just write Control Words, every 4th word
	for ( ; i < 512; i++) {
		//  just write 4 host-maintained Control Word bits
		*((unsigned int *)(k)) = descr_table_RT1[i] & 0xF000; // desc word 1
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 2 = 0
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 3 = 0
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 4 = 0
		k += 2;
		i += 3;
	}		

	#else // not using SMCP

	// every word in table is written 
	for ( i = 0, k = (STARTING_ADDR + j); i < 512; i++) {
		*((unsigned int *)(k)) = descr_table_RT1[i];
		k += 2;
	}

	#endif

	#if (ILLEGAL_CMD_DETECT)
	// copy the Illegalization Table array (declared above) into HI-613x.
	// load Illegalization Table for RT1
	
	// write address k starts at table base address 
	k = STARTING_ADDR + (RT1_ILLEGAL_TABLE_BASE_ADDR << 1);
	
	for ( i = 0; i < 256; i++) {	
		*((unsigned int *)(k)) = illegal_table[i];
		k = k + 2;
	}

	#endif // (ILLEGAL_CMD_DETECT)

	// ======================================================================================


}	// end: initialize_613x_RT1()


//--------------------------------------------------------------------------------------
//	This function loads dummy data into the limited set of RT1 transmit buffers 
//	assigned above during initialization. This is only used for testing.
//--------------------------------------------------------------------------------------
void write_dummy_tx_data_RT1(void) {
	
	unsigned int i, j;
	static unsigned int a_data[32] = {0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,0x0808,
		0x0909,0x1010,0x1111,0x1212,0x1313,0x1414,0x1515,0x1616,
		0x1717,0x1818,0x1919,0x2020,0x2121,0x2222,0x2323,0x2424,
		0x2525,0x2626,0x2727,0x2828,0x2929,0x3030,0x3131,0x3232};

	static unsigned int b_data[32] = {0xF001,0xF002,0xF003,0xF004,0xF005,0xF006,0xF007,0xF008,
		0xF009,0xF00A,0xF00B,0xF00C,0xF00D,0xF00E,0xF00F,0xF010,
		0xF011,0xF012,0xF013,0xF014,0xF015,0xF016,0xF017,0xF018,
		0xF019,0xF01A,0xF01B,0xF01C,0xF01D,0xF01E,0xF01F,0xF020};


	unsigned int k;

	// FOR TESTING PING-PONG, A PAIR OF DPA/DPB Tx BUFFERS: EACH BUFFER RESERVES
	// SPACE FOR MSG INFO WORD AND TIME TAG WORD, PLUS 32 DATA WORDS 
	
	// first a 32-word buffer starting at offset = 0x0866 
	k = (STARTING_ADDR + (0x0866 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = a_data[i];
		k = k + 2;
	}
	
	// next a 32-word buffer starting at offset = 0x0888 
	k = (STARTING_ADDR + (0x0888 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = b_data[i];
		k = k + 2;
	}

	// ================================================================================= 

	// FOR TESTING INDEXED MODE, A BUFFER THAT HOLDS 32 32-WORD MESSAGES,
	// REQUIRING INTERLACED MSG INFO AND TIME TAG WORDS FOR EACH MESSAGE 

	// total 1088-word buffer starting at offset = 0x0D32 
	// this is 32 contiguous segments of 34 words each, 
	k = (STARTING_ADDR + (0x0D32 << 1));
	for ( j = 0; j < 32; j++) {
		// skip 2 addresses at top of 32-word segment for MsgInfo Word and TimeTag word
		k = k + 4; 
		// write the 32 data words...					
		for ( i = 0; i < 32; i++) {	
			*((unsigned int *)(k)) = b_data[i];
			k = k + 2;
		}
	}

	// ================================================================================= 

	// FOR TESTING CIRCULAR MODE 1, A BUFFER THAT HOLDS 32 32-WORD MESSAGES,
	// REQUIRING INTERLACED MSG INFO AND TIME TAG WORDS FOR EACH MESSAGE  

	// total 1088-word buffer starting at offset = 0x15D6 
	// this is 32 contiguous segments of 34 words each, 
	k = (STARTING_ADDR + (0x15D6 << 1));
	for ( j = 0; j < 32; j++) {
		// skip 2 addresses at top of 32-word segment for MsgInfo Word and TimeTag word
		k = k + 4; 
		// write the 32 data words...					
		for ( i = 0; i < 32; i++) {	
			*((unsigned int *)(k)) = a_data[i];
			k = k + 2;
		}
	}
	// follow with a 32-word safety pad in case of circ-1 buffer overrun
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = 0xBADD;
		k = k + 2;
	}
	
	// ================================================================================= 
	
	// FOR TESTING CIRCULAR MODE 2, A CONTIGUOUS 32 X 32-WORD DATA BLOCK 
	
	// total 8192-word buffer with offset range from 0x1E00 to 0x3DFF 
	k = (STARTING_ADDR + (0x1E00 << 1));
	// write the 8192 data words using incrementing data pattern...
	for ( i = 0, j = 0; i < 8192; i++, j++) {	
		*((unsigned int *)(k)) = j;
		k = k + 2;
	}
	
	// ================================================================================= 

	// for unimplemented transmit SA's. a 32-word buffer starting at offset = 0x1A58 
	k = (STARTING_ADDR + (0x1A58 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = 0xDEAD;
		k = k + 2;
	}
	
	return;

}	// end write_dummy_tx_data_RT1()



#endif	  //------------ END RT1 ENABLED ------------



//==========================================================================



#if (RT2_ena)    //------------ RT2 ENABLED ------------


//   	This function initializes the Holt HI-613x RT2 by writing 
//	configuration registers in the device. Only RT2 mode option 
//	bits are affected. The program has already called function 
// 	initialize_613x_shared() to initialize the common parameters
//	shared by BC, RT1, RT2 and/or Bus Monitor
//
void initialize_6130_RT2(void) {
	
	unsigned int k;

	unsigned int i,j;
	
	static unsigned int descr_table_RT2[512] = {
		/* this array is used to initialize the Descriptor Table. For subaddress-
	receive and subaddress-transmit commands, the array sets the desired data 
	buffer style and initializes data pointer values. 
	
	For mode code commands, use of the "Simplified Mode Command Processing" 
	option is assumed, so the only potential initialization in the mode 
	command half of the table is loading mode data word values for transmit 
	mode commands. 
	
	Only 3 defined mode commands actually transmit a mode data word; MC16, 
	M18 and MC19 decimal. For mode commands MC18 and MC19, the device 
	automatically transmits the correct data word value, NOT fetched from 
	this table. The transmitted value is copied into the table after transmit.
	For MC16, the transmitted value comes from this table.
	
	Only 3 defined mode commands actually receive a mode data word: MC17, 
	MC20 and MC21 decimal. If the terminal is not using "illegal command 
	detection" it will respond "in form" to all valid undefined, reserved or 
	unimplemented mode commands. By providing storage for all possible mode
	commands, the Descriptor Table provides predictable command response to
	all valid mode code commands, as well as a repository for received data. */

		/* ===================================================================== */
		/*    R T 2   R E C E I V E    S U B A D D R E S S   C O M M A N D S     */
		/* ===================================================================== */
		/*  Note:Subaddresses    ControlWord  DescrWord2  DescrWord3  DescrWord4 */
		/*  0,31 are not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD,
		/*  Rx Subaddress 01  */   0x0004,     0x0800,     0x0822,     0x0844, // ping-pong
		/*  Rx Subaddress 02  */   0x8000,     0x08D0,     0x0020,     0x0D10, // idx-32 ixeqz
		/*  Rx Subaddress 03  */   0x8001,     0x1176,     0x1176,     0x15B6, // cir1 ixeqz (32 MSG BUFFER)
		/*  Rx Subaddress 04  */   0x0000,     0x1A36,     0x0000,     0x1A36, // ----
		/*  Rx Subaddress 05  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |   
		/*  Rx Subaddress 06  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 07  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 08  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 09  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 10  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 11  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 12  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 13  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 14  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 15  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 16  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 17  */   0x0000,     0x1A36,     0x0000,     0x1A36, // shared index sgl-msg
		/*  Rx Subaddress 18  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 19  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 20  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 21  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 22  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 23  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 24  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 25  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 26  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 27  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 28  */   0x0000,     0x1A36,     0x0000,     0x1A36, //     |
		/*  Rx Subaddress 29  */   0x0000,     0x1A36,     0x0000,     0x1A36, // ----
		/*  Rx Subaddress 30  */   0x4000,     0x08AE,     0x0000,     0x08AE, // idx sgl-msg IWA irq
		/*  This row not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD, 
		/*                                                                       */
		/* ===================================================================== */
		/*     R T 2   T R A N S M I T   S U B A D D R E S S   C O M M A N D S   */
		/* ===================================================================== */
		/*  NOTE: IF BROADCAST COMMANDS ARE SUPPORTED, ACCIDENTAL BROADCAST-     */
		/*  TRANSMIT COMMANDS WILL UPDATE MIW + TT WORDS AT THE BROADCAST DATA   */
		/*  POINTER LOCATION (PING-PONG & INDEXED MODES) SO INITIALIZE B'CAST    */
		/*  POINTERS TO PREDICTABLE ADDRESSES, BUT NOT 0x0000 (CONFIG.REG 1!)    */
		/*                                                                       */
		/*  Note:Subaddresses    ControlWord  DescrWord2  DescrWord3  DescrWord4 */
		/*  0,31 are not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD,
		/*  Tx Subaddress 01  */   0x0004,     0x0866,     0x0888,     0x08AA, // ping-pong
		/*  Tx Subaddress 02  */   0x8000,     0x0D32,     0x0020,     0x1172, // idx-32 ixeqz
		/*  Tx Subaddress 03  */   0x8001,     0x15D6,     0x15D6,     0x1A16, // cir1 ixeqz
		/*  Tx Subaddress 04  */   0x0000,     0x1A58,     0x0000,     0x1A58, // ----
		/*  Tx Subaddress 05  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 06  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 07  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 08  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 09  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 10  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 11  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 12  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 13  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 14  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 15  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 16  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 17  */   0x0000,     0x1A58,     0x0000,     0x1A58, // shared index sgl-msg
		/*  Tx Subaddress 18  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 19  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 20  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 21  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 22  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 23  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 24  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 25  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 26  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 27  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 28  */   0x0000,     0x1A58,     0x0000,     0x1A58, //     |
		/*  Tx Subaddress 29  */   0x0000,     0x1A58,     0x0000,     0x1A58, // ----
		/*  Tx Subaddress 30  */   0x0000,     0x08AE,     0x0000,     0x08AE, // idx sgl-msg
		/*  This row not used */   0xDEAD,     0xDEAD,     0xDEAD,     0xDEAD,
		/*                                                                       */
		/* ===================================================================== */
		/*    R T 2  R E C E I V E   M O D E   C O D E   C O M M A N D S         */
		/* ===================================================================== */
		/*                       ControlWord MsgInfoWord  TimeTagWord DataWord   */
		/* undefined Rx MC 00 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, // ----
		/*     "     Rx MC 01 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 02 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 03 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |     
		/*     "     Rx MC 04 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 05 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 06 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 07 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 08 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, // shared index sgl-msg
		/*     "     Rx MC 09 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 10 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 11 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 12 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 13 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 14 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 15 */   0x0000,     0x1A7A,     0x0000,     0x1A7A, //     |
		/*     "     Rx MC 16 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*  DEFINED  Rx MC 17 */   0x0004,     0x1AE4,     0x1AE8,     0x1AEC, /* synchronize with data */
		/* undefined Rx MC 18 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*     "     Rx MC 19 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*  DEFINED  Rx MC 20 */   0x0004,     0x1AF0,     0x1AF4,     0x1AF8, /* shutdown selected bus*/
		/*  DEFINED  Rx MC 21 */   0x0004,     0x1AFC,     0x1B00,     0x1B04, /* override sel bus shutdown*/
		/*  reserved Rx MC 22 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*     "     Rx MC 23 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 24 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 25 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 26 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // shared index sgl-msg
		/*     "     Rx MC 27 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 28 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 29 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 30 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, //     |
		/*     "     Rx MC 31 */   0x0000,     0x1A7E,     0x0000,     0x1A7E, // ----
		/*                                                                       */
		/* ===================================================================== */
		/*   R T 2  T R A N S M I T   M O D E   C O D E   C O M M A N D S        */
		/*         using the "Simplified Mode Command Processing" option         */
		/* ===================================================================== */
		/*                       ControlWord MsgInfoWord  TimeTagWord DataWord   */
		/*  DEFINED  Tx MC 00 */   0x4004,     0x1ABA,     0x1ABC,     0x1ABE, /* dynamic bus control,, IWA interrupt */
		/*     "     Tx MC 01 */   0x0004,     0x1A90,     0x1A92,     0x1A94, /* synchronize */
		/*     "     Tx MC 02 */   0x0004,     0x1A96,     0x1A98,     0x1A9A, /* transmit status */
		/*     "     Tx MC 03 */   0x0004,     0x1A9C,     0x1A9E,     0x1AA0, /* initiate self test */
		/*     "     Tx MC 04 */   0x0004,     0x1AA2,     0x1AA4,     0x1AA6, /* shutdown bus */
		/*     "     Tx MC 05 */   0x0004,     0x1AA8,     0x1AAA,     0x1AAC, /* overrride shutdown bus */
		/*     "     Tx MC 06 */   0x0004,     0x1AAE,     0x1AB0,     0x1AB2, /* inhibit terminal flag */
		/*     "     Tx MC 07 */   0x0004,     0x1AB4,     0x1AB6,     0x1AB8, /* override inhibit term flag */
		/*     "     Tx MC 08 */   0x0004,     0x1ABA,     0x1ABC,     0x1ABE, /* reset terminal, */
		/*  reserved Tx MC 09 */   0x0000,     0x1A82,     0x0000,     0x1A82, // ----
		/*     "     Tx MC 10 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 11 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 12 */   0x0000,     0x1A82,     0x0000,     0x1A82, // shared index sgl-msg
		/*     "     Tx MC 13 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     |
		/*     "     Tx MC 14 */   0x0000,     0x1A82,     0x0000,     0x1A82, //     | 
		/*     "     Tx MC 15 */   0x0000,     0x1A82,     0x0000,     0x1A82, // ----
		/*  DEFINED  Tx MC 16 */   0x0004,     0x1AC0,     0x1AC4,     0x1AC8, /* transmit vector word */
		/* undefined Tx MC 17 */   0x0000,     0x1A86,     0x0000,     0x1A86, // shared index sgl-msg
		/*  DEFINED  Tx MC 18 */   0x0004,     0x1ACC,     0x1AD0,     0x1AD4, /* transmit last command */
		/*  DEFINED  Tx MC 19 */   0x0004,     0x1AD8,     0x1ADC,     0x1AE0, /* transmit BIT word */
		/* undefined Tx MC 20 */   0x0000,     0x1A86,     0x0000,     0x1A86, // ----
		/*     "     Tx MC 21 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*  reserved Tx MC 22 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 23 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 24 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 25 */   0x0000,     0x1A86,     0x0000,     0x1A86, // shared index sgl-msg
		/*     "     Tx MC 26 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 27 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 28 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 29 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     |
		/*     "     Tx MC 30 */   0x0000,     0x1A86,     0x0000,     0x1A86, //     | 
		/*     "     Tx MC 31 */   0x0000,     0x1A86,     0x0000,     0x1A86 }; // -- 

	// end of descr_table_RT2[512] declaration


	static unsigned int illegal_table[256] = {
		/* This array is loaded by the initialization function only when the terminal
	uses "illegal command detection", that is, when the macro ILLEGAL_CMD_DETECT 
	= YES in the header file 613x_initialization.h. 
	
	When macro ILLEGAL_CMD_DETECT = NO, the default all-zeros table value after 
	/MR master reset is retained, so all valid commands	get an "in form" response.
	Terminals not using "illegal command detection" should preserve the RAM's 
	(all 0x0000) reset state for the table address range to provide consistent 
	"in form" response for all valid commands. This function call provides this.

	This template illegalizes all mode code commands that are either undefined 
	or reserved in MIL-STD-1553B. Also, the 3 transmit mode codes that have a 
	mode data word are made illegal when broadcast. Mode code 0 (dynamic bus 
	control) cannot be implemented by the HI-613X and is also made illegal.
	
	If using this example as a template, please note that the array below 
	renders the following list of commands illegal:

	* All undefined and reserved mode code commands
	* Broadcast versions of these transmit mode cmds: MC0 MC2 MC16 MC18 MC19
	* All broadcast transmit subaddress commands 
	* Tx mode code MC0 "dynamic bus control" because BC switch-over not programmed
	* Tx mode code MC3 "initiate self test," entirely application specific 
	
	******************** PLEASE NOTE NEXT 2 LINES ! *************************
	*    All non-broacast receive commands to subaddress 16, and all        *
	* transmit commands to subaddress 17 are illegal for demonstration only *
	*************************************************************************/

		/* ====================================================================== */
		/*       RT2 BROADCAST RECEIVE MODE CODE AND SUBADDRESS COMMANDS          */
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*       _____________________ MC15 - MC12 Rx (undefined)                 */
		/*      |  ____________________ MC11 - MC8 Rx (undefined)                 */
		/*      | |  ___________________ MC7 - MC4 Rx (undefined)                 */
		/*      | | |  __________________ MC3 - MC0 Rx (undefined)                */
		/*      | | | |  _________________ MC31 - MC28 Rx (reserved)              */
		/*      | | | | |  ________________ MC27 - MC24 Rx (reserved)             */
		/*      | | | | | |                                                       */
		/*      | | | | | |       __________ MC23 Rx (reserved)                   */
		/*      | | | | | |      |  _________ MC22 Rx (reserved)                  */
		/*      | | | | | |      | |  ________ MC21 Rx (override sel tx shutdown) */ 
		/*      | | | | | |      | | |  _______ MC20 Rx (selected tx shutdown)    */
		/*      | | | | | |      | | | |                                          */
		/*      | | | | | |      | | | |    ____ MC19 Rx (undefined)              */
		/*      | | | | | |      | | | |   |  ___ MC18 Rx (undefined)             */
		/*      | | | | | |      | | | |   | |  __ MC17 Rx (sync with data)       */
		/*      | | | | | |      | | | |   | | |  _ MC16 Rx (undefined)           */
		/*      | | | | | |      | | | |   | | | |                                */
		/*      | | | | | |  bin 1 1 0 0   1 1 0 1                                */
		/*      | | | | | |      -------   -------                                */
		/*   0x F F F F F F         C         D                                   */
		/*                                                                        */
		/*   Broadcast                                                            */
		/* Rx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0100 0x0101 0x0102 0x0103 0x0104 0x0105 0x0106 0x0107  */
		0xFFFF,0xFFCD,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0108 0x0109 0x010A 0x010B 0x010C 0x010D 0x010E 0x010F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0110 0x0111 0x0112 0x0113 0x0114 0x0115 0x0116 0x0117  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0118 0x0119 0x011A 0x011B 0x011C 0x011D 0x011E 0x011F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0120 0x0121 0x0122 0x0123 0x0124 0x0125 0x0126 0x0127  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0128 0x0129 0x012A 0x012B 0x012C 0x012D 0x012E 0x012F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                                            */
		/* Rx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0130 0x0131 0x0132 0x0133 0x0134 0x0135 0x0136 0x0137  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*   Broadcast                                              SA31 = SA0    */
		/* Rx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0138 0x0139 0x013A 0x013B 0x013C 0x013D 0x013E 0x013F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFCD,
		
		/* ====================================================================== */
		/*       RT2 BROADCAST TRANSMIT MODE CODE AND SUBADDRESS COMMANDS         */
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*    _________________ MC15 - MC12 Tx (reserved)                         */
		/*   |                                                                    */
		/*   |   ________________ MC11 Tx (reserved)                              */
		/*   |  |  _______________ MC10 Tx (reserved)                             */
		/*   |  | |  ______________ MC9 Tx (reserved)                             */
		/*   |  | | |  _____________ MC8 Tx (reset remote terminal)               */
		/*   |  | | | |                                                           */
		/*   |  | | | |   ____________ MC7 Tx (override inhibit term flag bit)    */
		/*   |  | | | |  |  ___________ MC6 Tx (inhibit term flag bit)            */
		/*   |  | | | |  | |  __________ MC5 Tx (override transmitter shutdown)   */
		/*   |  | | | |  | | |  _________ MC4 Tx (transmitter shutdown)           */
		/*   |  | | | |  | | | |                                                  */
		/*   |  | | | |  | | | |   _______ MC3 Tx (init self test) NOT IN DEMO PGM*/
		/*   |  | | | |  | | | |  |  ______ MC2 Tx (transmit status) NO BROADCAST */
		/*   |  | | | |  | | | |  | |  _____ MC1 Tx (synchronize)                 */
		/*   |  | | | |  | | | |  | | |  ____ MC0 Tx (dyn bus control)NO BROADCAST*/
		/*   |  | | | |  | | | |  | | | |                      and NOT IMPLEMENTED*/
		/*   |  | | | |  | | | |  | | | |   __ MC31 - MC28 Tx (reserved)          */
		/*   |  | | | |  | | | |  | | | |  |  __ MC27 - MC24 Tx (reserved)        */
		/*   |  | | | |  | | | |  | | | |  | |  __ MC23-22 (res) & MC21-20 (undef)*/ 
		/*   |  | | | |  | | | |  | | | |  | | |                                  */
		/*   |  | | | |  | | | |  | | | |  | | |   ____ MC19 Tx NO BROADCAST      */
		/*   |  | | | |  | | | |  | | | |  | | |  |  ___ MC18 Tx NO BROADCAST     */
		/*   |  | | | |  | | | |  | | | |  | | |  | |  __ MC17 Tx (undefined)     */
		/*   |  | | | |  | | | |  | | | |  | | |  | | |  _ MC16 Tx NO BROADCAST   */
		/*   |  | | | |  | | | |  | | | |  | | |  | | | |                         */
		/*   |  1 1 1 0  0 0 0 0  1 1 0 1  | | |  1 1 1 1                         */
		/*   |  -------  -------  -------  | | |  -------                         */
		/*  0xF    E        0        D     F F F     F                            */
		/*                                                                        */
		/*   Broadcast                                                            */
		/* Tx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0140 0x0141 0x0102 0x0143 0x0144 0x0145 0x0146 0x0147  */
		0xFE0D,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0148 0x0149 0x014A 0x014B 0x014C 0x014D 0x014E 0x014F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0150 0x0151 0x0152 0x0153 0x0154 0x0155 0x0156 0x0157  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0158 0x0159 0x015A 0x015B 0x015C 0x015D 0x015E 0x015F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0160 0x0161 0x0162 0x0163 0x0164 0x0165 0x0166 0x0167  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0168 0x0169 0x016A 0x016B 0x016C 0x016D 0x016E 0x016F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                                            */
		/* Tx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0170 0x0171 0x0172 0x0173 0x0174 0x0175 0x0176 0x0177  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	
		/*   Broadcast                                              SA31 = SA0    */
		/* Tx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0178 0x0179 0x017A 0x017B 0x017C 0x017D 0x017E 0x017F  */
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFE0D,0xFFFF,	

		/* ====================================================================== */
		/* RT2 "OWN ADDRESS" NON-BROADCAST RECEIVE MODE CODE & SUBADDRESS COMMANDS*/
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*       _____________________ MC15 - MC12 Rx (undefined)                 */
		/*      |  ____________________ MC11 - MC8 Rx (undefined)                 */
		/*      | |  ___________________ MC7 - MC4 Rx (undefined)                 */
		/*      | | |  __________________ MC3 - MC0 Rx (undefined)                */
		/*      | | | |  _________________ MC31 - MC28 Rx (reserved)              */
		/*      | | | | |  ________________ MC27 - MC24 Rx (reserved)             */
		/*      | | | | | |                                                       */
		/*      | | | | | |       __________ MC23 Rx (reserved)                   */
		/*      | | | | | |      |  _________ MC22 Rx (reserved)                  */
		/*      | | | | | |      | |  ________ MC21 Rx (override sel tx shutdown) */ 
		/*      | | | | | |      | | |  _______ MC20 Rx (selected tx shutdown)    */
		/*      | | | | | |      | | | |                                          */
		/*      | | | | | |      | | | |    ____ MC19 Rx (undefined)              */
		/*      | | | | | |      | | | |   |  ___ MC18 Rx (undefined)             */
		/*      | | | | | |      | | | |   | |  __ MC17 Rx (sync with data)       */
		/*      | | | | | |      | | | |   | | |  _ MC16 Rx (undefined)           */
		/*      | | | | | |      | | | |   | | | |                                */
		/*      | | | | | |  bin 1 1 0 0   1 1 0 1                                */
		/*      | | | | | |      -------   -------                                */
		/*   0x F F F F F F         C         D                                   */
		/*                                                                        */
		/*  Own Address                                                           */
		/* Rx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x0180 0x0181 0x0182 0x0183 0x0184 0x0185 0x0186 0x0187  */
		0xFFFF,0xFFCD,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0188 0x0189 0x018A 0x018B 0x018C 0x018D 0x018E 0x018F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0190 0x0191 0x0192 0x0193 0x0194 0x0195 0x0196 0x0197  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----12-----  -----13-----  -----14-----  -----15-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x0198 0x0199 0x019A 0x019B 0x019C 0x019D 0x019E 0x019F  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01A0 0x01A1 0x01A2 0x01A3 0x01A4 0x01A5 0x01A6 0x01A7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01A8 0x01A9 0x01AA 0x01AB 0x01AC 0x01AD 0x01AE 0x01AF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Rx Subaddress -----24-----  -----25-----  -----26-----  -----27-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01B0 0x01B1 0x01B2 0x01B3 0x01B4 0x01B5 0x01B6 0x01B7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                             SA31 = SA0    */
		/* Rx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01B8 0x01B9 0x01BA 0x01BB 0x01BC 0x01BD 0x01BE 0x01BF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFCD,
		
		/* ====================================================================== */
		/* RT2 "OWN ADDRESS" NON-BROADCAST TRANSMIT MODECODE & SUBADDRESS COMMANDS*/
		/* ====================================================================== */
		/*    Setting legal/illegal mode code commands for subaddresses           */
		/*    00 and 31. IMPORTANT: Must repeat values at both locations!         */
		/*                                                                        */
		/*    _________________ MC15 - MC12 Tx (reserved)                         */
		/*   |                                                                    */
		/*   |   ________________ MC11 Tx (reserved)                              */
		/*   |  |  _______________ MC10 Tx (reserved)                             */
		/*   |  | |  ______________ MC9 Tx (reserved)                             */
		/*   |  | | |  _____________ MC8 Tx (reset remote terminal)               */
		/*   |  | | | |                                                           */
		/*   |  | | | |   ____________ MC7 Tx (override inhibit term flag bit)    */
		/*   |  | | | |  |  ___________ MC6 Tx (inhibit term flag bit)            */
		/*   |  | | | |  | |  __________ MC5 Tx (override transmitter shutdown)   */
		/*   |  | | | |  | | |  _________ MC4 Tx (transmitter shutdown)           */
		/*   |  | | | |  | | | |                                                  */
		/*   |  | | | |  | | | |   _______ MC3 Tx (init self test) NOT IN DEMO PGM*/
		/*   |  | | | |  | | | |  |  ______ MC2 Tx (transmit status)              */
		/*   |  | | | |  | | | |  | |  _____ MC1 Tx (synchronize)                 */
		/*   |  | | | |  | | | |  | | |  ____ MC0 Tx (dyn bus control) CANNOT DO! */
		/*   |  | | | |  | | | |  | | | |                                         */
		/*   |  | | | |  | | | |  | | | |   __ MC31 - MC28 Tx (reserved)          */
		/*   |  | | | |  | | | |  | | | |  |  __ MC27 - MC24 Tx (reserved)        */
		/*   |  | | | |  | | | |  | | | |  | |  __ MC23-22 (res) & MC21-20 (undef)*/ 
		/*   |  | | | |  | | | |  | | | |  | | |                                  */
		/*   |  | | | |  | | | |  | | | |  | | |   ____ MC19 Tx (tx BIT word)     */
		/*   |  | | | |  | | | |  | | | |  | | |  |  ___ MC18 Tx (tx last command)*/
		/*   |  | | | |  | | | |  | | | |  | | |  | |  __ MC17 Tx (undefined)     */
		/*   |  | | | |  | | | |  | | | |  | | |  | | |  _ MC16 (tx vector word)  */
		/*   |  | | | |  | | | |  | | | |  | | |  | | | |                         */
		/*   |  1 1 1 0  0 0 0 0  1 0 0 1  | | |  0 0 1 0                         */
		/*   |  -------  -------  -------  | | |  -------                         */
		/*  0xF    E        0        9     F F F     2                            */
		/*                                                                        */
		/*  Own Address                                                           */
		/* Tx Subaddress -----00-----  -----01-----  -----02-----  -----03-----   */
		/*                 mode codes   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address  0x01C0 0x01C1 0x0102 0x01C3 0x01C4 0x01C5 0x01C6 0x01C7  */
		0xFE09,0xFFF2,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----04-----  -----05-----  -----06-----  -----07-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01C8 0x01C9 0x01CA 0x01CB 0x01CC 0x01CD 0x01CE 0x01CF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----08-----  -----09-----  -----10-----  -----11-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01D0 0x01D1 0x01D2 0x01D3 0x01D4 0x01D5 0x01D6 0x01D7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress  -----12-----  -----13-----  -----14-----  -----15-----  */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01D8 0x01D9 0x01DA 0x01DB 0x01DC 0x01DD 0x01DE 0x01DF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----16-----  -----17-----  -----18-----  -----19-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01E0 0x01E1 0x01E2 0x01E3 0x01E4 0x01E5 0x01E6 0x01E7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress -----20-----  -----21-----  -----22-----  -----23-----   */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01E8 0x01E9 0x01EA 0x01EB 0x01EC 0x01ED 0x01EE 0x01EF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                                           */
		/* Tx Subaddress  -----24-----  -----25-----  -----26-----  -----27-----  */
		/*                word counts   word counts   word counts   word counts   */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01F0 0x01F1 0x01F2 0x01F3 0x01F4 0x01F5 0x01F6 0x01F7  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	
		/*  Own Address                                             SA31 = SA0    */
		/* Tx Subaddress -----28-----  -----29-----  -----30-----  -----31-----   */
		/*                word counts   word counts   word counts   mode codes    */
		/*                Low    High   Low    High   Low    High   Low    High   */
		/*  RAM address	 0x01F8 0x01F9 0x01FA 0x01FB 0x01FC 0x01FD 0x01FE 0x01FF  */
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFE09,0xFFF2 };	

	//  End of illegal_table[256] declaration 


	// ====================================================================================== 

	// These parameters are defined in 613x_initialization.h

	i = RTTO_15U|NOTICE2|TRXDB|AUTO_SHUTDN|AUTO_SYNC|MC16OPT|AUTO_MC8_RESET;

	#if (!SUPPORT_BROADCAST)
	i |= BCASTINV;
	#endif


	#if (!UNDEF_MCODES_VALID)
	i |= UMCINV;
	#endif

	#if (USE_SMCP)
	i |= SMCP;
	#endif

	// HI-6130 uses host bus interface, HI-6131 uses host SPI interface. From 
	// here, we use bus interface to initialize HI-6130 registers and RAM tables,
	// or we use SPI to initialize HI-6131 registers and RAM tables       

	// ====================================================================================== 


	pH6130->RT2_CONFIG_REG = i;

	// do not overwrite previously initialized common features 
	j = pH6130->MASTER_CONFIG_REG & ~(RT2STEX);

	// if "bus shutdown" mode codes 4 & 20 should disable Tx only but Rx still 
	// operates normally (NOT RECOMMENDED) then OR in BSDTXO, affecting RT1 & RT2:
	pH6130->MASTER_CONFIG_REG = j|RT2ENA|BSDTXO; 
	pH6130->MASTER_CONFIG_REG = j|RT2ENA;

	j = pH6130->HDW_INT_ENABLE_REG & ~(RT2TTM);
	// enable RT2 address parity fail interrupt, but not time tag match interrupt 
	pH6130->HDW_INT_ENABLE_REG = j|RT2APF;
	
	j = pH6130->HDW_INT_OUTPUT_ENABLE_REG & ~(RT2TTM);
	// enable pin output for selected interrupts 
	pH6130->HDW_INT_OUTPUT_ENABLE_REG = j|RT2APF;
	
	// for demo, mode code 8 (reset RT) is handled differently for RT1 and RT2.
	// When MC8 occurs for RT1, the RT1_MC8 interrupt is decoded by this program's
	// interrupt handler, which asserts RT1RESET in reg 0x0002. When MC8 occurs for 
	// RT2, that terminal is configured for AUTO_MC8_RESET to automatically assert 
	// the RT2RESET in reg 0x0002 without host intervention.  
	j = pH6130->RT_INT_ENABLE_REG & 0x01F8;
	// enable RT2 interrupts, but not the Message Error interrupt 
	pH6130->RT_INT_ENABLE_REG = j|RT2_IXEQZ|RT2_IWA|RT2_IBR|RT2_MC8;

	j = pH6130->RT_INT_OUTPUT_ENABLE_REG & 0x01F8;
	// enable pin output for selected RT2 interrupts 
	pH6130->RT_INT_OUTPUT_ENABLE_REG = j|RT2_IXEQZ|RT2_IWA|RT2_IBR|RT2_MC8;
	
	pH6130->RT2_DESC_TBL_BASE_ADDR_REG = 0x0600; 
	pH6130->RT2_1553_STATUS_BITS_REG = 0x0000;	 
	pH6130->RT2_TTAG_UTILITY_REG = 0x0000;	 
	pH6130->RT2_BUSA_SELECT_REG = 0xAAAA;		 
	pH6130->RT2_BUSB_SELECT_REG = 0xBBBB;		 
	pH6130->RT2_BIT_WORD_REG = 0x0000;			 
	pH6130->RT2_ALT_BIT_WORD_REG = 0xABCD;			 


	// load the RT2 Descriptor Table
	// Starting at the table base address written above, copy the RT2 Descriptor 
	// Table array (declared above) into HI-6130 RAM. 
	
	j = pH6130->RT2_DESC_TBL_BASE_ADDR_REG << 1;	// left-shift offset

	// If using simplified mode command processing (SMCP), the program is only 
	// required to initialize Descriptor Word 1 (Control Words) for each mode command 
	// in the 2nd half of the descriptor table. It is not necessary to initialize 
	// Descriptor Words 2,3,4 in the 2nd (mode code) half of the Descriptor Table.
	// THE FIRST HALF OF THE TABLE FOR SUBADDRESS COMMANDS MUST BE FULLY INITIALIZED.  

	#if (USE_SMCP)

	// in subaddress command half of table, every word is written 
	for ( i = 0, k = (STARTING_ADDR + j); i < 256; i++) {
		*((unsigned int *)(k)) = descr_table_RT2[i];
		k += 2;
	}
	// in mode command half of table, just write Control Words, every 4th word 
	for ( ; i < 512; i++) {
		//  just write 4 host-maintained Control Word bits 
		*((unsigned int *)(k)) = descr_table_RT2[i] & 0xF000; // desc word 1
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 2 = 0
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 3 = 0
		k += 2;
		*((unsigned int *)(k)) = 0; // desc word 4 = 0
		k += 2;
		i += 3;
	}		

	#else // not using SMCP

	// every word in table is written 
	for ( i = 0, k = (STARTING_ADDR + j); i < 512; i++) {
		*((unsigned int *)(k)) = descr_table_RT2[i];
		k += 2;
	}

	#endif



	#if (ILLEGAL_CMD_DETECT)
	// copy the Illegalization Table array 
	// (declared above) into the HI-6130 RAM for RT2 

	// write address k starts at table base address 
	k = STARTING_ADDR + (RT2_ILLEGAL_TABLE_BASE_ADDR << 1);
	
	for ( i = 0; i < 256; i++) {	
		*((unsigned int *)(k)) = illegal_table[i];
		k += 2;
	}

	#endif // (ILLEGAL_CMD_DETECT)

	// ====================================================================================== 

	
}	// initialize_613x_RT2()





// 	This function loads dummy data into the limited set of RT2 transmit buffers 
//	assigned above during initialization. This is only used for testing.

void write_dummy_tx_data_RT2(void) {
	
	unsigned int i, j;
	static unsigned int a_data[32] = {0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,0x0808,
		0x0909,0x1010,0x1111,0x1212,0x1313,0x1414,0x1515,0x1616,
		0x1717,0x1818,0x1919,0x2020,0x2121,0x2222,0x2323,0x2424,
		0x2525,0x2626,0x2727,0x2828,0x2929,0x3030,0x3131,0x3232};

	static unsigned int b_data[32] = {0xF001,0xF002,0xF003,0xF004,0xF005,0xF006,0xF007,0xF008,
		0xF009,0xF00A,0xF00B,0xF00C,0xF00D,0xF00E,0xF00F,0xF010,
		0xF011,0xF012,0xF013,0xF014,0xF015,0xF016,0xF017,0xF018,
		0xF019,0xF01A,0xF01B,0xF01C,0xF01D,0xF01E,0xF01F,0xF020};


	unsigned int k;

	// FOR TESTING PING-PONG, A PAIR OF DPA/DPB Tx BUFFERS: EACH BUFFER RESERVES
	// SPACE FOR MSG INFO WORD AND TIME TAG WORD, PLUS 32 DATA WORDS 
	
	// first a 32-word buffer starting at offset = 0x4066 
	k = (STARTING_ADDR + (0x4066 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = a_data[i];
		k = k + 2;
	}
	
	// next a 32-word buffer starting at offset = 0x4088 
	k = (STARTING_ADDR + (0x4088 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = b_data[i];
		k = k + 2;
	}

	// ================================================================================= 

	// FOR TESTING INDEXED MODE, A BUFFER THAT HOLDS 32 32-WORD MESSAGES,
	// REQUIRING INTERLACED MSG INFO AND TIME TAG WORDS FOR EACH MESSAGE 

	// total 1088-word buffer starting at offset = 0x4532 
	// this is 32 contiguous segments of 34 words each, 
	k = (STARTING_ADDR + (0x4532 << 1));
	for ( j = 0; j < 32; j++) {
		// skip 2 addresses at top of 32-word segment for MsgInfo Word and TimeTag word
		k = k + 4; 
		// write the 32 data words...					
		for ( i = 0; i < 32; i++) {	
			*((unsigned int *)(k)) = b_data[i];
			k = k + 2;
		}
	}

	// ================================================================================= 

	// FOR TESTING CIRCULAR MODE 1, A BUFFER THAT HOLDS 32 32-WORD MESSAGES,
	// REQUIRING INTERLACED MSG INFO AND TIME TAG WORDS FOR EACH MESSAGE  

	// total 1088-word buffer starting at offset = 0x4DD6 
	// this is 32 contiguous segments of 34 words each, 
	k = (STARTING_ADDR + (0x4DD6 << 1));
	for ( j = 0; j < 32; j++) {
		// skip 2 addresses at top of 32-word segment for MsgInfo Word and TimeTag word
		k = k + 4; 
		// write the 32 data words...					
		for ( i = 0; i < 32; i++) {	
			*((unsigned int *)(k)) = a_data[i];
			k = k + 2;
		}
	}
	// follow with a 32-word safety pad in case of circ-1 buffer overrun
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = 0xBADD;
		k = k + 2;
	}
	
	// ================================================================================= 
	
	// FOR TESTING CIRCULAR MODE 2, A CONTIGUOUS 32 X 32-WORD DATA BLOCK 
	
	// total 8192-word buffer with offset range from 0x5600 to 0x75FF 
	k = (STARTING_ADDR + (0x5600 << 1));
	// write the 8192 data words using incrementing data pattern...
	for ( i = 0, j = 0; i < 8192; i++, j++) {	
		*((unsigned int *)(k)) = j;
		k = k + 2;
	}
	
	// ================================================================================= 

	// for unimplemented transmit SA's. a 32-word buffer starting at offset = 0x5258 
	k = (STARTING_ADDR + (0x5258 << 1));
	// skip 2 addresses for MsgInfo Word and TimeTag word
	k = k + 4;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(k)) = 0xDEAD;
		k = k + 2;
	}

	
	
}	// end write_dummy_tx_data_RT2()



#endif	  //------------ END RT2 ENABLED ------------

// 	This function determines whether Op Status register indicates 
//	RT address parity error/fail (RTAPF). If parity error, a local
//	error trap executes until reset (no return to calling function).
//	If RT address in Op Status register does not have parity error,
//      function returns "1" to calling routine.
//
//	parameter indicates "1" or "2" for RT1 or RT2. 
//	other parameter values = invalid, local error trap

char RTAddr_okay(char RTnum) {

	unsigned int k,j;

	//if ((!RTnum)||(RTnum > 2))  error_trap(2); FIXME
	#message "RTAddr_OKAY() Needs some form of error trapping"
	// Check whether Operational Status register indicates RT 
	// address parity error


	k = pH6130->HDW_PENDING_INT_REG;


	// register bits 4-3 are RT2APF-RT1APF 
	j = RTnum << 4 ;
	if (k & j) {

		/* The active RT address is found in the Operational Status register.
		Check register here for parity error; if yes, a perpetual "error trap". 
		is executed. There are four ways parity error can occur:
		a) If AUTOEN pin = 0 (no auto-initialization), the DIP switch setting 
		on the board has parity error.
		b) If AUTOEN pin = LOCK pin = 1 the DIP switch RT address applies but
		the switch setting has parity error. Coming out of reset, the DIP 
		switch RT address and parity were latched into the Op Status register.
		along with the LOCK bit. When auto-initialize followed, LOCK prevented
		overwrite of the register with the EEPROM value for reg 0x2.
		c) If AUTOEN pin = 1 and LOCK pin = 0, when coming out of reset, the DIP 
		switch RT address was latched into the Op Status register.
		along with the LOCK bit. When auto-initialize followed, the register
		was overwritten with the EEPROM value for reg 0x2. The stored EEPROM 
		value for register 0x2 has parity error!
		d) If the Op Status register LOCK bit equals 0, the host may have 
		written an RT address with parity error.
		For all 4 cases, perpetual red LED double-blink until reset occurs */
		//error_trap(2);	
		#message "Another error trap was here!"

		return 0;	// execution will not get here
	}

	else return 1;
}




//
//      The next function polls pushbutton SW1 to detect user request for status flag update.	
//      It reads BUSYBIT and TFLAG DIP switches, then updates "1553 Status Word Bits Register"
// 	*  if (BUSYBIT switch = 1) THEN (set BUSY bit), ELSE (reset BUSY bit)
//	*  if (TFLAG switch = 1) THEN (set TERMFLAG bit), ELSE (reset TERMFLAG bit)
//	Note: The SSYSF (Subsystem Fail) DIP switch is directly wired to HI-613x SSYSF input pin
//	so no button press is required to change this status bit.
//
//	primary purpose: during RT validation tests, it is desirable to have a simple way
//	to set/reset these status bits
// 
/*
void modify_RT_status_bits(void) {

	const Pin pinNSW1 = PIN_NSW1;

	// SW1 was pressed before this function call 
	// turn on green LED 
	AT91C_BASE_PIOA->PIO_CODR = nLEDG;

	// wait for button release
	while (!PIO_Get(&pinNSW1));

	// update the RT1 AND RT2 Status Word Bits registers based on DIP switch
	RTstatusUpdate();

	// debounce delay then turn off green LED
	Delay_x100ms(4);
	AT91C_BASE_PIOA->PIO_SODR = nLEDG;
}
*/


//
//      The next function tests RT1 and RT2 BUSYBIT and TFLAG DIP switch settings,
//	then updates the HI-613X RT1 and RT2 "1553 Status Word Bits Registers"
//	IF (BUSYBIT switch = 1) THEN (set BUSY bit), ELSE (reset BUSY bit)
//	IF (TFLAG switch = 1) THEN (set TERMFLAG bit), ELSE (reset TERMFLAG bit)
//
//
/*
void RTstatusUpdate(void) {

	unsigned int out_rt1,out_rt2;
	const Pin pinRT1TFLG = PIN_RT1TFLG;
	const Pin pinRT1BSY  = PIN_RT1BSY;
	const Pin pinRT2TFLG = PIN_RT2TFLG;
	const Pin pinRT2BSY  = PIN_RT2BSY;        
	
	out_rt1 = pH6130->RT1_1553_STATUS_BITS_REG & ~(BUSY|TERMFLAG);
	out_rt2 = pH6130->RT2_1553_STATUS_BITS_REG & ~(BUSY|TERMFLAG);

	
	// set BUSY status bit if "BUSYBIT" switch is high, else reset bit.	        
	if(PIO_Get(&pinRT1BSY)) out_rt1 |= BUSY;
	if(PIO_Get(&pinRT2BSY)) out_rt2 |= BUSY;
	// set Terminal Flag (TERMFLAG) status bit if "TFLAG" switch is high, else reset bit.	
	if(PIO_Get(&pinRT1TFLG)) out_rt1 |= TERMFLAG;
	if(PIO_Get(&pinRT2TFLG)) out_rt2 |= TERMFLAG;

	///#if (HOST_BUS_INTERFACE)
	pH6130->RT1_1553_STATUS_BITS_REG = out_rt1;
	pH6130->RT2_1553_STATUS_BITS_REG = out_rt2;

	
}  // end RTstatusUpdate()
*/

