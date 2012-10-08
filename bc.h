/*
* Bus Controller Library
* bc.h
*
* Phillip Johnston
* 8 October 2012
*/

#ifndef _HI6130_BC_H
#define _HI6130_BC_H

#define STARTING_ADDR 0x60000000
#message "FIX ME:  GPQ_BASE_BUS_ADDR for actual starting address in bc.h"


/**************
* Definitions *
**************/
//BC Control Word
#define TXTTMC17 1<<15 // only applies for mode code 17: transmit BC time tag count
#define MEMASK 1<<14 // if bit 0 = 0, Status Set occurs for RT Status Word Msg Error bit
#define SRQMASK 1<<13 // if bit 0 = 0, Status Set occurs for RT Status Word Svc Request bit
#define BSYMASK 1<<12 // if bit 0 = 0, Status Set occurs for RT Status Word Busy bit
#define SSFMASK 1<<11 // if bit 0 = 0, Status Set occurs for RT Status Word Subsystem Fail bit
#define TFMASK 1<<10 // if bit 0 = 0, Status Set occurs for RT Status Word Terminal Flag bit
#define RSVMASK 1<<9 // if bit 0 = 0, Status Set occurs for any RT Status Word Reserved bit 7-5
#define RTRYENA 1<<8 // if retry enabled in BC Config reg, retry occurs for Status Set
#define USEBUSA 1<<7 // if bit = 1 then use Bus A, if bit = 0 then use Bus B
#define USEBUSB 0<<7
#define SFTEST 1<<6 // if bit = 1 then use offline self-test
#define MSKBCR 1<<5 // if BCRME = 1 in BC Config, this bit INVERTED reflects expected BCR status, 
// mismatch when BCR = 1 causes status set
// if BCRME = 0 in BC Config, this bit reflects expected BCR status, 
// mismatch = status set
#define EOMINT 1<<4 // if BCEOM interrupt is enabled, this bit causes message EOM interrupt
// bit 3 reserved 
#define MCODE 1<<2 // select mode code message format
#define BCST 1<<1 // select broadcast message format
#define RT_RT 1<<0 // select RT-to-RT message format


//Command Word Entries
#define RX 0
#define TX 1<<10


//BC Op Codes
#define XEQ 0x01<<10
#define XQG 0x16<<10
#define JMP 0x02<<10
#define CAL 0x03<<10
#define IRQ 0x06<<10
#define RTN 0x04<<10
#define HLT 0x07<<10
#define DLY 0x08<<10
#define WFT 0x09<<10
#define CFT 0x0A<<10
#define CMT 0x0B<<10
#define LTT 0x0D<<10
#define LTH 0x18<<10
#define LFT 0x0E<<10
#define SFT 0x0F<<10
#define PTT 0x10<<10
#define PTH 0x19<<10
#define PTB 0x1A<<10
#define PBS 0x11<<10
#define PSI 0x12<<10
#define PSM 0x13<<10
#define WTG 0x14<<10
#define XQF 0x15<<10
#define XFG 0x17<<10
#define WMP 0x1B<<10
#define WMI 0x1C<<10
#define DSZ 0x1D<<10
#define FLG 0x0C<<10


// BC Condition Codes
#define LT 0
#define GP0 0
#define GTorEQ 16
#define nGP0 16
#define EQ 1
#define GP1 1
#define nEQ 17
#define nGP1 17
#define GP2 2
#define nGP2 18
#define GP3 3
#define nGP3 19
#define GP4 4
#define nGP4 20
#define GP5 5
#define nGP5 21
#define GP6 6
#define nGP6 22
#define GP7 7
#define nGP7 23
#define nRESP 8
#define RESP 24
#define FMTERR 9
#define nFMTERR 25
#define GBLOCK 10
#define nGBLOCK 26
#define MASKSTAT 11
#define nMASKSTAT 27
#define BADMSG 12
#define GOODMSG 28
#define x1RETRY 13
#define n1RETRY 29
#define x2RETRY 14
#define ALWAYS 15
#define NEVER 31


// op code word validation field with odd parity bit = 0 
#define VP0 0x0140

// op code word validation field with odd parity bit = 1 
#define VP1 0x8140

// message block addresses, non-RT-to-RT msgs have 8 words 
#define MSG_BLK1_ADDR 0x3E00
#define MSG_BLK2_ADDR 0x3E08
#define MSG_BLK3_ADDR 0x3E10
#define MSG_BLK4_ADDR 0x3E18
#define MSG_BLK5_ADDR 0x3E20
#define MSG_BLK6_ADDR 0x3E28
#define MSG_BLK7_ADDR 0x3E30
#define MSG_BLK8_ADDR 0x3E38

// message block addresses, RT-to-RT msgs have 16 words 
#define RTRT_MSG_BLK1_ADDR 0x3E40
#define RTRT_MSG_BLK2_ADDR 0x3E50 // thru 0x3E5F


/*********************************************************************************************
* Here are macros and structures that simplify bus addressing for the Bus Controller *
* Instruction List and arrays of Message Control/Status Blocks in RAM. This applies for *
* HI-6130 (HOST_BUS_INTERFACE) only. The HI-6131 (SPI INTERFACE) is not memory-mapped so *
* other addressing methods are used. Elsewhere, examples demonstrate HI-6130 bus addressing *
* using these structures. Use of these structures is optional. For example, the functions *
* that load demo initialization values into RAM do so without using these structs. *
**********************************************************************************************/

// preempt preprocessor warning: "duplicate definitions for pGPQ" pointer
// occurring for each "include" instance for this file in a C file...
#ifndef BC_GPQ_PTR
#define BC_GPQ_PTR

//-------------------------------------------------------------------------------
// POINTER FOR HI-6130 BUS ADDRESSING OF THE BC GENERAL PURPOSE QUEUE 
//-------------------------------------------------------------------------------

#define GP_QUEUE_ADDR 0x0C00 // thru 0x0C3F, always 64 words, RELOCATABLE
// starting RAM address for 64-word BC General Purpose Queue. 
// Default value after reset 0x0C00 is preloaded in the BC General
// Purpose Queue register 0x0038. Value can be overwritten, with
// restrictions. See data sheet if relocating queue.

// ___________________ chip select CS0 base address for ARM Cortex M3
// |
// | _______ Queue start address is doubled for bus addressing
// | |
#define GPQ_BASE_BUS_ADDR (STARTING_ADDR+(GP_QUEUE_ADDR<<1)) 

// declare pointer to the 64-word BC general purpose queue
// initialized to point at queue base address...
unsigned int *pGPQ;

/* One of these declaration forms must be within scope: 
GPQ pGPQ; // initialize elsewhere
GPQ pGPQ = (GPQ) GPQ_BASE_BUS_ADDR;
*/

#endif // ifndef BC_GPQ_PTR

// memory-mapped i/o macro for addressing gen purpose queue;
// "n" must be in the 0 to 63 range inclusive...
#define GPQ(n) (*pGPQ)[n] 


//--------------------------------------------------------------------------------
// BC INSTRUCTION LIST: AN ARRAY OF 2-WORD STRUCTURES FOR HI-6130 BUS ADDRESSING
//--------------------------------------------------------------------------------

#define BC_ILIST_BASE_ADDR 0x1B70 // thru 0x1BFF allocated, 90 words total, RELOCATABLE
// starting RAM address for BC instruction list. Initialization
// should copy this value into the BC Instruction List Start Addr
// register 0x0033. No need to copy to pointer reg 0x0034, read-only.


#define NUM_OPS 32 // number of op code & parameter word pairs in BC instruction list;
// demo program can handle 45 maximum without memory reallocation

// ___________________ chip select CS0 base address for ARM Cortex M3
// |
// | _______ instruction list address is doubled for bus addressing
// | |
#define BC_ILIST_BUS_ADDR (STARTING_ADDR+(BC_ILIST_BASE_ADDR<<1)) 

struct bc_inst_list {
	unsigned int opCode;
	unsigned int param;
};

#define BC_ILIST_BASE ((struct/**/bc_inst_list*)BC_ILIST_BUS_ADDR) // 0x600036E0
//This declaration must be within scope: const BCil pBCil = BC_ILIST_BASE;

// a macro that simplifies instruction list addressing
#define ILIST(n) (*pBCil)[n] 


//------------------------------------------------------------------------
// ARRAYS OF BC MESSAGE CONTROL/STATUS BLOCKS FOR HI-6130 BUS ADDRESSING
//------------------------------------------------------------------------

// BC Message Control/Status Blocks are 16 words for RT-RT messages, 8 words if not RT-RT. 
// Stored in arrays, separate arrays are used for RT-RT and non-RT-RT stacks. Furthermore, 
// message block definition differs for 16-bit vs. 32-bit BC timebase, specified in 613x_initialization.h

#define MSG_ARRAY_START_ADDR 0x3E00 // thru 0x3E3F allocated, 64 words total, RELOCATABLE
// starting RAM address for array of 8-word BC control/status blocks
// for non-RT-RT messages. Demo program allocates space for eight msgs.

#define MSG_ARRAY_RTRT_START_ADDR 0x3E40 // thru 0x3E5F allocated, 32 words total, RELOCATABLE
// starting RAM address for array of 16-word BC control/status blocks
// for RT-RT messages. Demo program allocates space for two msgs.

#define NUM_BLKS 8 // number of non-RT-RT message blocks (8 words each)
#define NUM_2RTBLKS 2 // number of RT-RT message blocks (16 words each)

// ______________ chip select CS0 base address for ARM Cortex M3
// |
// | ____ array start addresses are doubled for bus addressing
// | |
#define MSG_BASE_BUS_ADDR (STARTING_ADDR+(MSG_ARRAY_START_ADDR<<1)) 
#define MSG_2RT_BASE_BUS_ADDR (STARTING_ADDR+(MSG_ARRAY_RTRT_START_ADDR<<1)) 


//----------------------------------------------------------------------
#ifdef BC_TTAG_HI_RES
// using 32-bit timebase

// declare the BC block array for non-RT-RT messages, and pointer
struct MsgBlock {
	// non-RT-RT message block, eight 16-bit words
	unsigned int ctrlWord;
	unsigned int cmdWord;
	unsigned int dataPtr;
	unsigned int timeNextMsg;
	unsigned int ttagLow;
	unsigned int blockStatus;
	unsigned int ttagHigh;
	unsigned int RTstatus;
};

#define BC_STACK_BASE ((struct/**/MsgBlock*)MSG_BASE_BUS_ADDR)

// declare separate BC block array for RT-RT messages, and pointer
struct MsgBlock2RT {
	// RT-RT message block, sixteen 16-bit words
	unsigned int ctrlWord;
	unsigned int RxCmdWord;
	unsigned int dataPtr;
	unsigned int timeNextMsg;
	unsigned int ttagLow;
	unsigned int blockStatus;
	unsigned int ttagHigh;
	unsigned int TxRTstatus;
	unsigned int TxCmdWord;
	unsigned int RxRTstatus;
	unsigned int dummy[6];
};

#define BC_2RTSTACK_BASE ((struct/**/MsgBlock2RT*)MSG_2RT_BASE_BUS_ADDR)

/* These declarations must be within scope: 
const BCstack pBCstack = BC_STK_BASE; 
const BCstack2RT pBCstack2RT = BC_2RTSTK_BASE; 
*/

#endif

//----------------------------------------------------------------------
#ifndef BC_TTAG_HI_RES

// not BC_TTAG_HI_RES, using 16-bit timebase

// declare the BC block array for non-RT-RT messages, and pointer
struct MsgBlock {
	// non-RT-RT message block, eight 16-bit words
	unsigned int ctrlWord;
	unsigned int cmdWord;
	unsigned int dataPtr;
	unsigned int timeNextMsg;
	unsigned int timetag;
	unsigned int blockStatus;
	unsigned int loopbackWord;
	unsigned int RTstatus;
};

#define BC_STK_BASE ((struct/**/MsgBlock*)MSG_BASE_BUS_ADDR)

// declare separate BC block array for RT-RT messages, and pointer
struct MsgBlock2RT {
	// RT-RT message block, sixteen 16-bit words
	unsigned int ctrlWord;
	unsigned int RxCmdWord;
	unsigned int dataPtr;
	unsigned int timeNextMsg;
	unsigned int timetag;
	unsigned int blockStatus;
	unsigned int loopbackWord; 
	unsigned int TxRTstatus;
	unsigned int TxCmdWord;
	unsigned int RxRTstatus;
	unsigned int dummy[6];
};

#define BC_2RTSTK_BASE ((struct/**/MsgBlock2RT*)MSG_2RT_BASE_BUS_ADDR)

/* These declarations must be within scope: 
const BCstack pBCstack = BC_STK_BASE; 
const BCstack2RT pBCstack2RT = BC_2RTSTK_BASE; 
*/

#endif // not BC_TTAG_HI_RES

// macros that simplify msg stack addressing
#define MSTAK(n) (*pBCstack)[n]
#define MSTAK2RT(n) (*pBCstack2RT)[n]


/************************
* Function Declarations *
************************/
void initialize_6130_BC(void);
void initialize_bc_msg_blocks(void);
void initialize_bc_instruction_list(void);
void bc_disable(void);
void bc_enable(void);
void bc_start(void);

void BC_bus_addressing_examples(void);
void bc_trigger(void);
void bc_switch_tests(void);
void SW2_BCtest (void);
char SW1_BC_Trigger(void);


// End of File 

#endif //_HI6130_BC_H