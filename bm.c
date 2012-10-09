/*
* Bus Monitor Library
* bm.c
*
* Phillip Johnston
* 8 October 2012
*/

// standard Atmel/IAR headers
#include "config.h"
#include "defs.h"
#include "regdefs.h"
#include "bm.h"

extern struct bm_reg_map_t * pH6130;
extern struct mt_fil_table * pMTF;

#if(SMT_ena)
//extern unsigned int * pDSTK[DSTACKSIZE];
//extern struct SMsgBlock * pScSTK[SMT_BLKS];
extern unsigned int ** pDSTK;
extern struct SMsgBlock ** pScSTK;
extern struct A_List * pAddrList;
#endif // (SMT_ena)

///#if(HOST_BUS_INTERFACE && SMT_ena)
#if(SMT_ena)

void SMT_bus_addressing_examples(void) {

	unsigned int j,k;    

	// -----------------------------------------
	// SMT Address List bus addressing examples
	// -----------------------------------------
	// all bus addressing destinations for this feature are shown here...
	// The base address for the address list is declared in file 613x_mt.h

	// in demo, SMT Address List start addr is 0x0100
	pAddrList->cmdStkStart = 0x5400;  // write command stack start address
	k = pAddrList->cmdStkNext;        // command stack next messsage block address
	j = pAddrList->cmdStkEnd;         // read command stack end address
	k = pAddrList->cmdStkInt;         // read cmd stack address for optional level-attained interrupt
	j = pAddrList->dataStkStart;      // read data stack start address
	k = pAddrList->dataStkNext;       // read data stack next messsage data address
	j = pAddrList->dataStkEnd;        // read data stack end address
	k = pAddrList->dataStkInt;        // read data stack address for optional level-attained interrupt

	// ------------------------------------------
	// SMT Command Stack bus addressing examples
	// ------------------------------------------
	// in demo, SMT command stack start addr 0x5400
	// The stack start and end addresses are declared in file 613x_mt.h
	
	// all bus addressing destinations for this feature are shown here, for msg blk 0,
	// with dfferences shown, based on time tag resolution...

	#ifndef SMT_TTAG_HI_RES         // IF SMT IS USING 16-BIT TIME BASE    
	k = (*pScSTK)->blockStatus;   // read SMT message block 0 block status word
	(*pScSTK)->timeTag = 0x1234;  // read SMT message block 0 16-bit time tag
	// (normally would read not write time tag value)
	j = (*pScSTK)->dataPtr;       // read SMT message block 0 data pointer
	(*pScSTK)->cmdWord = 0xABCD;  // write SMT message block 0 command word
	// (normally would read not write command word value)
	
	#else // ifdef SMT_TTAG_HI_RES  // IF SMT IS USING 48-BIT TIME BASE    
	(*pScSTK)[0].ttagLow = 0xBEEF;  // write SMT message block 0 low word of 48-bit time tag
	// (normally would read not write this time tag value)
	j = (*pScSTK)[0].ttagLow;       // read SMT message block 0 low word of 48-bit time tag
	j = (*pScSTK)[0].ttagMid;       // read SMT message block 0 mid word of 48-bit time tag
	j = (*pScSTK)[0].ttagHigh;      // read SMT message block 0 high word of 48-bit time tag
	j = (*pScSTK)[0].blockStatus;   // read SMT message block 0 block status word
	j = (*pScSTK)[0].gaptime._1;    // read SMT message block 0 response word (gap 1 byte)
	(*pScSTK)[0].gaptime._2 = 250;  // write SMT message block 0 response word (gap 2 byte) to 25 us
	// (normally would read not write a gaptime value)
	j = (*pScSTK)[0].msgBytes;      // read SMT message block 0 number of *BYTES*
	j = (*pScSTK)[0].dataPtr;       // read SMT message block 0 data pointer
	j = (*pScSTK)[0].cmdWord;       // read SMT message block 0 command word
	#endif

	// or, using a macro, CSTAK(n) defined as (*pScSTK)[n] 
	j = (&CSTAK(1))->blockStatus;        // read SMT message block 1 block status word
	k = (&CSTAK(1))->dataPtr;            // read SMT message block 1 data pointer
	(&CSTAK(1))->dataPtr = k;            // write SMT message block 1 data pointer

	
	// ---------------------------------------
	// SMT Data Stack bus addressing examples
	// ---------------------------------------
	// in demo, SMT data stack start addr is 0x6000, 8K (8,192) words are allocated
	// The stack start and end addresses are declared in file 613x_mt.h
	
	// first modify pDSTK to point at word 0 for the message data block of interest, (usually
	// NOT the first word in the allocated stack, as below) then change [index] to access
	// data words for that message. Using this method to retrieve data for a single message,
	// the index range is 1 to 36, but entire stack can be indexed, as demonstrated here...
	
	pDSTK = (unsigned int *) DSTACK_BASE_BUS_ADDR; // reset pointer to stack word 0
	j = (*pDSTK)[2];              // read SMT data stack word 2
	(*pDSTK)[2] = 0x4567;         // write SMT data stack word 2
	k = (*pDSTK)[2];              // read SMT data stack word 2 again
	(*pDSTK)[2] = j;              // restore original SMT data stack word 2
	k = (*pDSTK)[8191];           // read last SMT data stack word allocated in demo program

	// or, using a macro, DSTAK(n) defined as (*pDSTK)[n] where n = 0 to (STACK_SIZE - 1)
	k = DSTAK(25);                // read general purpose queue word 25
	DSTAK(2) = j;                 // write general purpose queue word 2

	
	// ------------------------------------------------
	//     SMT Filter Table bus addressing examples
	// ------------------------------------------------
	// The filter table base address is declared in file 613x_mt.h.
	// in demo, MT filter table start addr 0x0100
	
	//     ____invariant
	//    |     _____RT0 through RT31
	//    |    |   _____Tx or Rx
	//    |    |  |      ____ subadd0 through subadd31
	//    |    |  |     |
	j = pMTF->RT5.Tx.sublist_upper & 0x8000;   // read MT filter table bit: RT address 5 Tx subaddress 31
	pMTF->RT5.Tx.sublist_upper = 1 << 15;   // set same bit (this Tx subaddress for RT5 will be ignored by monitor)
	j = pMTF->RT5.Tx.sublist_upper & 0x8000;   // read same bit again
	j = pMTF->RT19.Tx.sublist_upper & 0x0008;   // read MT filter table bit: RT address 19 Tx subaddress 4
	pMTF->RT0.Tx.sublist_lower = 1 << 7;    // set MT filter table bit: RT address 0 Rx subaddress 8
	
	// preempt warnings: variable was set but never used
	j=j;
	k=k;
	
}   // end SMT_bus_addressing_examples()


#endif  // (HOST_BUS_INTERFACE && SMT_ena)




// The following function initializes either the Simple or IRIG-106 monitor
// (SMT or IMT) as enabled in file 613x_initialization.h. Same file also
// specifies HI-6130 (bus) or HI-6131 (SPI) configuration...
//
// One of four combinations is initialized:
// (1) SMT for HI-6130 (bus) applications
// (2) IMT for HI-6130 (bus) applications
// (3) SMT for HI-6131 (SPI) applications
// (4) IMT for HI-6130 (SPI) applications
//
void initialize_6130_MT(void) {
	

	static const unsigned int base_6130 = STARTING_ADDR;
	unsigned int addr;
	
	unsigned int i,j;
	
	
	#if(SMT_ena)
	static unsigned int smt_addr_list[8] = {

		//  =============  Command Stack ==============
		//  Start     Current   End       Interrupt
		//  Address   Address   Address   Address
		0x5400,   0x5400,   0x5FFF,   0x5DFF, // end - 512 
		
		//  ==============  Data Stack  ================
		//  Start     Current   End       Interrupt 
		//  Address   Address   Address   Address   
		0x6000,   0x6000,   0x7FFF,   0x7DFF }; // end - 512 
	
	#endif

	static unsigned int mt_filter_table[128] = {

		// bit = 0: all msgs to that subaddress are recorded, 
		// bit = 1: all messages to that subaddress are ignored.
		// default: (even if table not written) all messages are recorded  
		// RxSA = receive subaddresses,     TxSA = tranmsit subaddresses   
		//====================================================================
		// RT address 0   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 1   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 2   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 3   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 4   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 5   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 6   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 7   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 8   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 9   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 10   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 11   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 12   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 13   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 14   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 15   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 16   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 17   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 18   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 19   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 20   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 21  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 22   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 23  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 24  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 25  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 26  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 27  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 28  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 29   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 30  

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000,
		//====================================================================
		// RT address 31   

		//   ____RxSA 15-12  ____TxSA 15-12  ____RxSA 31-28  ____TxSA 31-28
		//  |               |               |               |
		//  | ___RxSA 11-8  | ___TxSA 11-8  | ___RxSA 27-24 | ___TxSA 27-24
		//  ||              ||              ||              ||
		//  || __RxSA 7-4   || __TxSA 7-4   || __RxSA 23-20 || __TxSA 23-20
		//  |||             |||             |||             |||
		//  ||| _RxSA 3-0   ||| _TxSA 3-0   ||| _RxSA 19-16 ||| _TxSA 19-16
		//  ||||            ||||            ||||            ||||
		0x0000,         0x0000,         0x0000,         0x0000  };
	// ====================================================================
	// end of mt_filter_table[128]


	// HI-6130 uses host bus interface. From 
	// here, we use bus interface to initialize HI-6130 registers and RAM tables. 

	// ======================================================================================



	// Initialize MT Filter table in RAM using values in array above.
	// Skip this if all messages shall be recorded (since reset clears RAM)

	for ( i = 0, addr = base_6130 + (0x0100 << 1); i < 128; i++) {

		*((unsigned int *)(addr)) = mt_filter_table[i];
		addr += 2;
	}

	pH6130->ADDR_LIST_POINTER = 0x00B0;

	// ================== Simple Monitor ======================= 

	#if(SMT_ena)

	// read back address just written and shift left
	j = (pH6130->ADDR_LIST_POINTER << 1);

	// initialize MT address list using array declared at top of function 
	for ( i = 0, addr = base_6130 + j; i < 8; i++) {
		
		*((unsigned int *)(addr)) = smt_addr_list[i];
		
		addr += 2;
	}

	// Set up SMT interrupts:
	//
	// STKROVR   = command stack end-address written, ptr rolled-over to start address
	//             (MT address list word 2 contains fixed end address)
	// DSTKROVR  = data stack end-address written, ptr rolled-over to start address
	//             (MT address list word 6 contains fixed end address)
	// STKADRSS  = command stack addr match, level-attained int
	//             (MT address list word 3 contains fixed int address)
	// DSTKADRSS = data stack address match,
	//             (MT address list word 7 contains fixed int address)
	// MSG_ERR   = detected RT status word with message error
	// MT_EOM    = end-of-message interrupt

	// Interrupt Enable, both hardware ints and polled ints  
	pH6130->INT_ENABLE_REG = STKROVR|DSTKROVR|STKADRSS|DSTKADRSS|MSG_ERR|MT_EOM;

	// Output Enable for hardware ints  
	pH6130->INT_OUTPUT_ENABLE_REG = STKROVR|DSTKROVR|STKADRSS|DSTKADRSS|MSG_ERR|MT_EOM;

	// Optional: assert IMTA bit in the Master Config Reg 0 
	// so the ACTIVE pin reflects MT activity 
	j = pH6130->MASTER_CONFIG_REG;
	pH6130->MASTER_CONFIG_REG = j|IMTA;
	
	// Config options for Simple monitor, "i" will be written to the
	// MT Configuration Reg. The option bit SELECT_SMT must be 1. 
	i = SELECT_SMT;

	// Choose 1 from each of the following choices:
	//   GAPCHKON or GAPCHKOFF
	//   STOR_INVWD or STOP_INVWD
	//   EXTD_STATUS or IRIG_STATUS
	//   REC_CW or REC_CWDW or REC_CS or REC_CSDS
	//   TAG_LWLB or TAG_FWFB or TAG_FWLB or TAG_OFF
	//   MTTO_15U or MTTO_20U or MTTO_58U or MTTO_138U 
	//  -----------------------------------------------
	//  Note: SMT time tag clock is selected in main.c,
	//  as well as this SMT time tag resolution option:
	//   SMT_TTAG16 or SMT_TTAG48

	i |= GAPCHKOFF|STOR_INVWD|EXTD_STATUS|REC_CSDS|TAG_OFF|MTTO_20U;

	pH6130->CONFIG_REG = i;

	// end SMT_ena 

	// ================== IRIG-106 Monitor ======================= 

	// ====================================================================================== 

}   // end initialize_613x_MT(void)


// end of file 

