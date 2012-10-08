/*
* Bus Monitor Library
* bm.h
*
* Phillip Johnston
* 8 October 2012
*/


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
#define MT_FTABLE_BUS_ADDR (0x60000000 + (MT_FILTER_TABLE_BASE_ADDR << 1))  

// starting with the most elementary structure

struct sublist {
	// building block only, no instances declared...
	// 32 single-bit "word count" fields spanning 2 16-bit words
	unsigned subadd0 : 1;
	unsigned subadd1 : 1;
	unsigned subadd2 : 1;
	unsigned subadd3 : 1;
	unsigned subadd4 : 1;
	unsigned subadd5 : 1;
	unsigned subadd6 : 1;
	unsigned subadd7 : 1;
	unsigned subadd8 : 1;
	unsigned subadd9 : 1;
	unsigned subadd10 : 1;
	unsigned subadd11 : 1;
	unsigned subadd12 : 1;
	unsigned subadd13 : 1;
	unsigned subadd14 : 1;
	unsigned subadd15 : 1;
	unsigned subadd16 : 1;
	unsigned subadd17 : 1;
	unsigned subadd18 : 1;
	unsigned subadd19 : 1;
	unsigned subadd20 : 1;
	unsigned subadd21 : 1;
	unsigned subadd22 : 1;
	unsigned subadd23 : 1;
	unsigned subadd24 : 1;
	unsigned subadd25 : 1;
	unsigned subadd26 : 1;
	unsigned subadd27 : 1;
	unsigned subadd28 : 1;
	unsigned subadd29 : 1;
	unsigned subadd30 : 1;
	unsigned subadd31 : 1;
} ;

struct rtalist { 
	// building block only, no instances declared...
	struct sublist Rx;
	struct sublist Tx;
};


typedef struct mt_fil_table {
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
} mt_filter_table, *MTF;

#define MT_FTABLE_BASE	((MTF) MT_FTABLE_BUS_ADDR) 	

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

#define DSTACKSIZE (DATASTACK_END_ADDR - DATASTACK_START_ADDR + 1)

//                                  ___________________ chip select CS0 base address for ARM Cortex M3
//                                 |
//                                 |                   ____ stack start address is doubled for bus addressing
//                                 |                  |
#define DSTACK_BASE_BUS_ADDR (0x60000000 + (DATASTACK_START_ADDR << 1))  

// declare pointer to the data stack,
// initialized to point at its base address...
typedef volatile unsigned int (*DSTK)[DSTACKSIZE] ;

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
#define MT_ALIST_BUS_ADDR (0x60000000 + (MT_ALIST_BASE_ADDR << 1))

// declare the SMT Address List (structure) and pointer
typedef struct A_List {
	volatile unsigned int cmdStkStart;  // command stack start address
	volatile unsigned int cmdStkNext;   // command stack next messsage block address
	volatile unsigned int cmdStkEnd;    // command stack end address
	volatile unsigned int cmdStkInt;    // optional cmd stack fill warning interrupt address
	volatile unsigned int dataStkStart; // data stack start address
	volatile unsigned int dataStkNext;  // data stack next messsage data address
	volatile unsigned int dataStkEnd;   // data stack end address
	volatile unsigned int dataStkInt;   // optional data stack fill warning interrupt address
} addList, *AddrList;

#define MT_ALIST_BASE  ((AddrList) MT_ALIST_BUS_ADDR) 	

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
#define CSTACK_BASE_BUS_ADDR (0x60000000 + (CMDSTACK_START_ADDR << 1))  


// used below: response time word has two 8-bit fields
struct respTime {
	unsigned _1 : 8;
	unsigned _2 : 8;
} ;  

//----------------------------------------------------------------------
#ifdef SMT_TTAG_HI_RES
// using 48-bit timebase
// Command Stack message block for 48-bit timetag has eight 16-bit words

// number of blocks reserved in command stack
#define SMT_BLKS ((CMDSTACK_END_ADDR - CMDSTACK_START_ADDR + 1) / 8) 


// declare the SMT command stack (an array of structures) and pointer
typedef struct SMsgBlock {
	volatile unsigned int ttagLow;
	volatile unsigned int ttagMid;
	volatile unsigned int ttagHigh;
	volatile unsigned int blockStatus;
	struct respTime gaptime;
	volatile unsigned int msgBytes;
	volatile unsigned int dataPtr;
	volatile unsigned int cmdWord;
} SMT_cmdStack[SMT_BLKS], (* ScSTK)[SMT_BLKS];

#define SMT_CMDSTACK_BASE ((ScSTK) CSTACK_BASE_BUS_ADDR)

/* This declaration must be within scope:   
const ScSTK pScSTK = SMT_CMDSTACK_BASE; 
*/

//----------------------------------------------------------------------
#else // not SMT_TTAG_HI_RES
// using 16-bit timebase
// Command Stack message block for 16-bit timetag has four 16-bit words

// number of blocks reserved in command stack
#define SMT_BLKS ((CMDSTACK_END_ADDR - CMDSTACK_START_ADDR + 1) / 4) 


// declare the SMT command stack (an array of structures) and pointer
typedef struct SMsgBlock {
	volatile unsigned int blockStatus;
	volatile unsigned int timeTag;
	volatile unsigned int dataPtr;
	volatile unsigned int cmdWord;
} SMT_cmdStack[SMT_BLKS], (* ScSTK)[SMT_BLKS];

#define SMT_CMDSTACK_BASE ((ScSTK) SMT_CSTACK_BASE_BUS_ADDR)

/* This declaration must be within scope:   
const ScSTK pScSTK = SMT_CMDSTACK_BASE; 
*/

#endif // not SMT_TTAG_HI_RES


// a macro
#define CSTAK(n) (*pScSTK)[n] 


#endif // (SMT_ena)

/***************************************************************************
*                                                                          *
*   I M T    I R I G - 1 0 6    M O N I T O R    D E C L A R A T I O N S   *
*                                                                          *
****************************************************************************/

#if (IMT_ena)

//-----------------------------------------------------------------
//  IMT MONITOR ADDRESS LIST: STRUCTURE FOR HI-6130 BUS ADDRESSING
//-----------------------------------------------------------------
//
//                                   This feature is relocatable...
#define MT_ALIST_BASE_ADDR 0x00B0 // IMT monitor 8-word Address List
//
//
//                                ___________________ chip select CS0 base address for ARM Cortex M3
//                               |
//                               |             _______ base address is doubled for bus addressing
//                               |            |
#define MT_ALIST_BUS_ADDR (0x60000000 + (MT_ALIST_BASE_ADDR << 1))

// declare the IMT Address List (structure) and pointer
typedef struct A_List {
	volatile unsigned int stkStartAddr;    // stack start address
	volatile unsigned int currPacketAddr;  // start address for current packet
	volatile unsigned int stkEndAddr;      // stack end address
	volatile unsigned int stkIntAddr;      // stack address for optional level-attained interrupt 
	volatile unsigned int lastBSWAddr;     // address of last-written block status word
	volatile unsigned int dummy[2];        // 2 unused words
	volatile unsigned int pktOvrWordsInt;  // number of words to packet overrun, optional warning interrupt 
} addList, *AddrList;

#define MT_ALIST_BASE  ((AddrList) MT_ALIST_BUS_ADDR) 	

// This declaration must be within scope:   const AddrList pAddrList = MT_ALIST_BASE;


//-------------------------------------------------------------
//  IMT MONITOR: HI-6130 PACKET HEADER POINTER BUS ADDRESSING
//-------------------------------------------------------------

// preempt preprocessor warning: "duplicate definitions for PktHdr" pointer
// occurring for each "include" instance for this file in a C file...
#ifndef PKT_HDR
#define PKT_HDR

// 3 nested structs for "pointer to packet header" struct...

struct hWord6 {
	// two 8-bit fields 
	unsigned dtypeVer : 8;
	unsigned seqNum : 8;
} ;

struct hWord7 {
	// two 8-bit fields 
	unsigned pktFlags : 8;
	unsigned dataType : 8;
} ;

struct hWord13 {
	// three fields
	unsigned numMsgsHi : 8;
	unsigned reserved  : 6;
	unsigned ttagBits  : 2;
} ;

// struct encompasses IRIG-106 packet header (words 0-11)
// plus 2 words of Channel Specific Data (words 12-13)
typedef struct PktHdrCSD {
	volatile unsigned int pktSync;
	volatile unsigned int ChannID;
	volatile unsigned int pktLenLo;
	volatile unsigned int pktLenHi;
	volatile unsigned int dataLenLo;
	volatile unsigned int dataLenHi;
	struct hWord6 word6;
	struct hWord7 word7;
	volatile unsigned int relTimeLow;
	volatile unsigned int relTimeMid;
	volatile unsigned int relTimeHigh;
	volatile unsigned int hdrChecksum;
	volatile unsigned int numMsgsLo;
	struct hWord13 word13;
} *PktHdr;

// This declaration must be within scope:   PktHdr pPktHdr; // when using IMT

// macro converts HI-6130 RAM addr to ARM bus address
#define PktBusAddr(n) ((PktHdr)(0x60000000 + (n << 1)))



// IMT Intra-Packet Msg Block addressing structure consists of 
// the Intra-Packet Time Stamp (4 words, a 64-bit value) and
// the Intra-Packet Data Header (3 words, 4 fields)

// used below: response time word has two 8-bit fields
struct respTime {
	unsigned _1 : 8;
	unsigned _2 : 8;
} ;  

typedef struct IntraPktMsgBlk {
	// Intra-Packet Time Stamp (4 words, a 64-bit value)
	volatile unsigned int msgTime0;
	volatile unsigned int msgTime1;
	volatile unsigned int msgTime2;
	volatile unsigned int msgTime3;
	// Intra-Packet Data Header (3 words, 4 fields)
	volatile unsigned int blkStatWd;
	struct respTime gap;
	volatile unsigned int msgLength;
} *IPMB;

// This declaration must be within scope:   IPMB pIPMB; // when using IMT

// macro converts HI-6130 RAM addr to ARM bus address
#define MsgBusAddr(n) ((IPMB)(0x60000000 + (n << 1)))


#endif // ifndef PKT_HDR

#endif // (IMT_ena)


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
void initialize_613x_MT(void);


// End of File 

