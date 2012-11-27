/*
* Bus Monitor Library
* bm.h
*
* Phillip Johnston
* 8 October 2012
*/

#define STARTING_ADDR 0x60000000

//------------------------------------------------------------------------------
//  MONITOR FILTER TABLE: NESTED STRUCTURE HEIRARCHY FOR HI-6130 BUS ADDRESSING
//------------------------------------------------------------------------------
//
//                                          This table is not relocatable...
#define MT_FILTER_TABLE_BASE_ADDR 0x0100 // SMT OR IMT monitor filter table fixed addr range 0x0100 to 0x017F
//
//
//                                ___________________ chip select CS0 base address for ARM Cortex M3
//                               |
//                               |             _______ table base address is doubled for bus addressing
//                               |            |
#define MT_FTABLE_BUS_ADDR (STARTING_ADDR+(MT_FILTER_TABLE_BASE_ADDR<<1))  

// starting with the most elementary structure

struct sublist {
	// building block only, no instances declared...
	// 32 single-bit "word count" fields spanning 2 16-bit words\
	unsigned int sublist_lower;
	unsigned int sublist_upper;
} ;

struct rtalist { 
	// building block only, no instances declared...
	struct sublist Rx;
	struct sublist Tx;
};


struct mt_fil_table {
	// here is the top level SMT/IMT Filter Table structure, 
	// we declare instances for table and its pointer
	struct rtalist RT0;
	struct rtalist RT1;
	struct rtalist RT2;
	struct rtalist RT3;
	struct rtalist RT4;
	struct rtalist RT5;
	struct rtalist RT6;
	struct rtalist RT7;
	struct rtalist RT8;
	struct rtalist RT9;
	struct rtalist RT10;
	struct rtalist RT11;
	struct rtalist RT12;
	struct rtalist RT13;
	struct rtalist RT14;
	struct rtalist RT15;
	struct rtalist RT16;
	struct rtalist RT17;
	struct rtalist RT18;
	struct rtalist RT19;
	struct rtalist RT20;
	struct rtalist RT21;
	struct rtalist RT22;
	struct rtalist RT23;
	struct rtalist RT24;
	struct rtalist RT25;
	struct rtalist RT26;
	struct rtalist RT27;
	struct rtalist RT28;
	struct rtalist RT29;
	struct rtalist RT30;
	struct rtalist RT31;
};

#define MT_FTABLE_BASE	((struct/**/mt_fil_table*)MT_FTABLE_BUS_ADDR) 	

// This declaration must be within scope:   const MTF pMTF = MT_FTABLE_BASE;

/***********************************************************************
*                                                                      *
*   S M T    S I M P L E    M O N I T O R    D E C L A R A T I O N S   *
*                                                                      *
************************************************************************/

#if (SMT_ena)

// preempt preprocessor warning: "duplicate definitions for pDSTK" pointer
// occurring for each "include" instance for this file in a C file...
#ifndef SMT_DS_PTR
#define SMT_DS_PTR

// -----------------------------------------------------------
//  HI-6130 BUS ADDRESSING OF THE SMT DATA STACK USING POINTER
// -----------------------------------------------------------

#define DATASTACK_START_ADDR 0x6000       // data stack is 8K (8,192) words total, RELOCATABLE
#define DATASTACK_END_ADDR   0x7FFF

#define DSTACKSIZE (DATASTACK_END_ADDR-DATASTACK_START_ADDR+1)

//                                  ___________________ chip select CS0 base address for ARM Cortex M3
//                                 |
//                                 |                   ____ stack start address is doubled for bus addressing
//                                 |                  |
#define DSTACK_BASE_BUS_ADDR (STARTING_ADDR+(DATASTACK_START_ADDR<<1))  

// declare pointer to the data stack,
// initialized to point at its base address...
//typedef unsigned int (*DSTK)[DSTACKSIZE] ;

/* One of these declaration forms must be within scope:   
DSTK *pDSTK; // initialize elsewhere
DSTK *pDSTK = (DSTK) DSTACK_BASE_BUS_ADDR;
*/

// a macro for addressing SMT Data Stack;
// "n" must be in the range 0 to (STACK_SIZE - 1) inclusive...
#define DSTAK(n) (*pDSTK)[n] 

#endif // #ifndef SMT_DS_PTR


//-----------------------------------------------------------------
//  SMT MONITOR ADDRESS LIST: STRUCTURE FOR HI-6130 BUS ADDRESSING
//-----------------------------------------------------------------
//
//                                   This feature is relocatable...
#define MT_ALIST_BASE_ADDR 0x00B0 // SMT monitor 8-word Address List
//
//
//                                ___________________ chip select CS0 base address for ARM Cortex M3
//                               |
//                               |             _______ base address is doubled for bus addressing
//                               |            |
#define MT_ALIST_BUS_ADDR (STARTING_ADDR+(MT_ALIST_BASE_ADDR<<1))

// declare the SMT Address List (structure) and pointer
struct A_List {
	unsigned int cmdStkStart;  // command stack start address
	unsigned int cmdStkNext;   // command stack next messsage block address
	unsigned int cmdStkEnd;    // command stack end address
	unsigned int cmdStkInt;    // optional cmd stack fill warning interrupt address
	unsigned int dataStkStart; // data stack start address
	unsigned int dataStkNext;  // data stack next messsage data address
	unsigned int dataStkEnd;   // data stack end address
	unsigned int dataStkInt;   // optional data stack fill warning interrupt address
};

#define MT_ALIST_BASE  ((struct/**/A_List*) MT_ALIST_BUS_ADDR) 	

// This declaration must be within scope:   const AddrList pAddrList = MT_ALIST_BASE;


//---------------------------------------------------------------------------
//  SMT COMMAND STACK: AN ARRAY OF MESSAGE BLOCKS FOR HI-6130 BUS ADDRESSING
//---------------------------------------------------------------------------

#define CMDSTACK_START_ADDR  0x5400       // command stack is 3,064 words total, RELOCATABLE
#define CMDSTACK_END_ADDR    0x5FF8 

// The Simple Monitor Command Stack is an array of stored fixed-length blocks, 1 block per message. Below,
// message block definition differs for 16-bit vs. 48-bit SMT timebase, specified in 613x_initialization.h

//                                  ______________ chip select CS0 base address for ARM Cortex M3
//                                 |
//                                 |                   ____ stack start address is doubled for bus addressing
//                                 |                  |
#define CSTACK_BASE_BUS_ADDR (STARTING_ADDR+(CMDSTACK_START_ADDR<<1))  


// used below: response time word has two 8-bit fields
struct respTime {
	unsigned char _1; //8-bits
	unsigned char _2;
} ;  

//----------------------------------------------------------------------
#ifdef SMT_TTAG_HI_RES
// using 48-bit timebase
// Command Stack message block for 48-bit timetag has eight 16-bit words

// number of blocks reserved in command stack
#define SMT_BLKS ((CMDSTACK_END_ADDR-CMDSTACK_START_ADDR+1)/8) 


// declare the SMT command stack (an array of structures) and pointer
struct SMsgBlock {
	unsigned int ttagLow;
	unsigned int ttagMid;
	unsigned int ttagHigh;
	unsigned int blockStatus;
	struct respTime gaptime;
	unsigned int msgBytes;
	unsigned int dataPtr;
	unsigned int cmdWord;
};

#define SMT_CMDSTACK_BASE ((struct/**/SMsgBlock*)CSTACK_BASE_BUS_ADDR)

/* This declaration must be within scope:   
const ScSTK pScSTK = SMT_CMDSTACK_BASE; 
*/

//----------------------------------------------------------------------
#else // not SMT_TTAG_HI_RES
// using 16-bit timebase
// Command Stack message block for 16-bit timetag has four 16-bit words

// number of blocks reserved in command stack
#define SMT_BLKS ((CMDSTACK_END_ADDR-CMDSTACK_START_ADDR+1)/4) 


// declare the SMT command stack (an array of structures) and pointer
struct SMsgBlock {
	unsigned int blockStatus;
	unsigned int timeTag;
	unsigned int dataPtr;
	unsigned int cmdWord;
};

#define SMT_CMDSTACK_BASE ((struct/**/SMsgBlock*)SMT_CSTACK_BASE_BUS_ADDR)

/* This declaration must be within scope:   
const ScSTK pScSTK = SMT_CMDSTACK_BASE; 
*/

#endif // not SMT_TTAG_HI_RES


// a macro
#define CSTAK(n) (*pScSTK)[n] 


#endif // (SMT_ena)


//------------------------------------------------------------------------------
//      Global Function Prototypes
//------------------------------------------------------------------------------

//  for HI-6130 projects only, bus addressing examples, IMT
void IMT_bus_addressing_examples(void);

//  for HI-6130 projects only, bus addressing examples, SMT
void SMT_bus_addressing_examples(void);

//
//   Function call initializes Bus Monitor
//
void initialize_6130_MT(void);


// End of File 

