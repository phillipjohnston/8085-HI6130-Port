/*
 *  file	613x_mt.c
 *
 *  brief	This file contains initialization functions and arrays for 
 *		writing initialization values to RAM tables inside the Holt 
 *		HI-6130 or HI-6131 device. 
 *
 *
 *		IMPORTANT: Edit file 613x_initialization.h to define either:
 *
 *		HOST_BUS_INTERFACE  YES  sets compilation for HI-6130 (bus)
 *		                    NO   sets compilation for HI-6131 (SPI)
 *
 *		Most functional procedures in this project are written for
 *		both parallel bus or SPI host interface schemes, this runtime 
 *		switch determines which are procedures are compiled.
 * 
 *
 *		HOLT DISCLAIMER
 *
 *		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *		EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *		OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *		NONINFRINGEMENT. 
 *		IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *		OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *		OTHERWISE,ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *		SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *		Copyright (C) 2011 by  HOLT, Inc.
 *		All Rights Reserved.
 *
 */

// standard Atmel/IAR headers
#include <pio/pio.h>

// Holt project headers
#include "613x_initialization.h"
#include "613x_regs.h"
#include "board_613x.h"
#include "613x_mt.h"
   
#include "board_6130.h"
#include "device_6130.h"

    extern const H6130 pH6130;
    extern const MTF pMTF;
    
    #if(SMT_ena)
        extern DSTK pDSTK;
        extern const ScSTK pScSTK;
        extern const AddrList pAddrList;
    #endif // (SMT_ena)
    
    #if(IMT_ena)
        extern PktHdr pPktHdr;
        extern IPMB pIPMB;
        extern const AddrList pAddrList;
    #endif // (IMT_ena)

#if (IMT_ena)

void IMT_bus_addressing_examples(void) {
  
    unsigned short j,k;    
  
    // -----------------------------------------
    // IMT Address List bus addressing examples
    // -----------------------------------------
    // all bus addressing destinations for this feature are shown here...
    // The base address for the address list is declared in file 613x_mt.h
    
    // in demo, IMT Address List start addr is 0x00B0
    pAddrList->stkStartAddr = 0x5400; // write stack start address (NOT address list start!)
    k = pAddrList->currPacketAddr;    // read start address for current packet
    j = pAddrList->stkEndAddr;        // read stack end address
    k = pAddrList->stkIntAddr;        // read stack address for optional level-attained interrupt
    j = pAddrList->lastBSWAddr;       // read address of last-written block status word
    k = pAddrList->pktOvrWordsInt;    // number of words to packet overrun, optional warning interrupt
    
    // ------------------------------------------
    // IMT Packet Header bus addressing examples
    // ------------------------------------------
    
    // applies when packet header is enabled
    if (!(pH6130->MT_CONFIG_REG & IMT_HDR_OFF)) {
      
        // use macro to set bus address pointer value. 
        // the macro PktBusAddr(n) is defined ((PktHdr)(0x60000000 + (n << 1)))
      
        // this header address only applies for the first packet after reset!
        pPktHdr = PktBusAddr(0x5400);
    
        // alternate way to set bus address to RAM addr 0x5400
        // without using the macro...
        // pPktHdr = ((PktHdr)(0x60000000 + (0x5400 << 1)));
    
        // read the IRIG-106 packet header data
        // all of the packet header data fields are listed here...
        j = pPktHdr->pktSync;
        k = pPktHdr->ChannID;
        j = pPktHdr->pktLenLo;    // half of 32-bit value: packet length in bytes
        k = pPktHdr->pktLenHi;    // half of 32-bit value: packet length in bytes
        j = pPktHdr->dataLenLo;   // half of 32-bit value: data length in bytes
        k = pPktHdr->dataLenHi;   // half of 32-bit value: data length in bytes
        j = pPktHdr->word6.dtypeVer;
        k = pPktHdr->word6.seqNum;
        j = pPktHdr->word7.pktFlags;
        k = pPktHdr->word7.dataType;
        j = pPktHdr->relTimeLow;   // one third of 48-bit value: relative time count
        k = pPktHdr->relTimeMid;   // one third of 48-bit value: relative time count
        j = pPktHdr->relTimeHigh;  // one third of 48-bit value: relative time count
        k = pPktHdr->hdrChecksum;
        j = pPktHdr->numMsgsLo;        // low 16 bits of 24-bit value
        k = pPktHdr->word13.numMsgsHi; // high 8 bits of 24-bit value
        j = pPktHdr->word13.ttagBits;
        /* write works too, but probably not a good idea
        pPktHdr->pktSync = 0xEB25;                      */
        
    }   // end if
        
    // ---------------------------------------------------
    // IMT Intra-Packet Msg Block bus addressing examples
    // ---------------------------------------------------
    // refer to data sheet for details. This structure consists of 
    // the Intra-Packet Time Stamp (4 words, a 64-bit value) and
    // the Intra-Packet Data Header (3 words, 4 fields)
    
    // use macro MsgBusAddr(n) defined as ((IPMB)(0x60000000 + (n << 1))) 
    // to set bus address pointer value. Example points at first packet after reset.
    
    if (pH6130->MT_CONFIG_REG & IMT_HDR_OFF) {
        // packet header disabled
        pIPMB = MsgBusAddr(0x5400);
    }
    else {  
        // 14-word packet header enabled
        pIPMB = MsgBusAddr(0x540E);
    }
    
    // all of the intra-packet time stamp and data header fields are listed here...

    // Intra-Packet Time Stamp (4 words, most significant word always zero)
    j = pIPMB->msgTime0;  // read 64-bit msg time stamp, bits 0-15
    j = pIPMB->msgTime1;  // read 64-bit msg time stamp, bits 16-31
    j = pIPMB->msgTime2;  // read 64-bit msg time stamp, bits 32-47
    j = pIPMB->msgTime3;  // read 64-bit msg time stamp, bits 48-63, always 0x0000
   // Intra-Packet Data Header (3 words, 4 fields)
    j = pIPMB->blkStatWd; // reaad message block status word
    j = pIPMB->gap._1;    // read message response time, 0.1us res, 25.5us max
    j = pIPMB->gap._2;    // read message response time #2, 0.1us res, 25.5us max, RT-RT only
    j = pIPMB->msgLength; // read # of *BYTES* in msg data block, range 2-72, 1-36 words

    
    // ------------------------------------------------
    //     IMT Filter Table bus addressing examples
    // ------------------------------------------------
    // The filter table base address is declared in file 613x_mt.h.
    // in demo, MT filter table start addr 0x0100
    
    //     ____invariant
    //    |     _____RT0 through RT31
    //    |    |   _____Tx or Rx
    //    |    |  |      ____ subadd0 through subadd31
    //    |    |  |     |
    j = pMTF->RT5.Tx.subadd31;   // read MT filter table bit: RT address 5 Tx subaddress 31
    pMTF->RT5.Tx.subadd31 = 1;   // set same bit (this Tx subaddress for RT5 will be ignored by monitor)
    j = pMTF->RT5.Tx.subadd31;   // read same bit again
    j = pMTF->RT19.Tx.subadd4;   // read MT filter table bit: RT address 19 Tx subaddress 4
    pMTF->RT0.Tx.subadd8 = 1;    // set MT filter table bit: RT address 0 Rx subaddress 8
                
    // preempt warnings: variable was set but never used
    j=j;
    k=k;

}   // end IMT_bus_addressing_examples()


#endif // (IMT_ena)


//================================================================================================


///#if(HOST_BUS_INTERFACE && SMT_ena)
#if(SMT_ena)

void SMT_bus_addressing_examples(void) {
  
    unsigned short j,k;    

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
    k = (*pScSTK)[0].blockStatus;   // read SMT message block 0 block status word
    (*pScSTK)[0].timeTag = 0x1234;  // read SMT message block 0 16-bit time tag
                                    // (normally would read not write time tag value)
    j = (*pScSTK)[0].dataPtr;       // read SMT message block 0 data pointer
    (*pScSTK)[0].cmdWord = 0xABCD;  // write SMT message block 0 command word
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
    j = CSTAK(1).blockStatus;        // read SMT message block 1 block status word
    k = CSTAK(1).dataPtr;            // read SMT message block 1 data pointer
    CSTAK(1).dataPtr = k;            // write SMT message block 1 data pointer

    
    // ---------------------------------------
    // SMT Data Stack bus addressing examples
    // ---------------------------------------
    // in demo, SMT data stack start addr is 0x6000, 8K (8,192) words are allocated
    // The stack start and end addresses are declared in file 613x_mt.h
    
    // first modify pDSTK to point at word 0 for the message data block of interest, (usually
    // NOT the first word in the allocated stack, as below) then change [index] to access
    // data words for that message. Using this method to retrieve data for a single message,
    // the index range is 1 to 36, but entire stack can be indexed, as demonstrated here...
    
    pDSTK = (DSTK) DSTACK_BASE_BUS_ADDR; // reset pointer to stack word 0
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
    j = pMTF->RT5.Tx.subadd31;   // read MT filter table bit: RT address 5 Tx subaddress 31
    pMTF->RT5.Tx.subadd31 = 1;   // set same bit (this Tx subaddress for RT5 will be ignored by monitor)
    j = pMTF->RT5.Tx.subadd31;   // read same bit again
    j = pMTF->RT19.Tx.subadd4;   // read MT filter table bit: RT address 19 Tx subaddress 4
    pMTF->RT0.Tx.subadd8 = 1;    // set MT filter table bit: RT address 0 Rx subaddress 8
    
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
void initialize_613x_MT(void) {
	

    const unsigned long base_6130 = 0x60000000;
    unsigned long addr;
        
    unsigned short i,j;
    
    
    #if(SMT_ena)
    volatile unsigned short smt_addr_list[8] = {

    //  =============  Command Stack ==============
    //  Start     Current   End       Interrupt
    //  Address   Address   Address   Address
        0x5400,   0x5400,   0x5FFF,   0x5DFF, // end - 512 
		
    //  ==============  Data Stack  ================
    //  Start     Current   End       Interrupt 
    //  Address   Address   Address   Address   
        0x6000,   0x6000,   0x7FFF,   0x7DFF }; // end - 512 

    #else // (IMT_ena)
     volatile unsigned short imt_addr_list[8] = {

    //  =============  Combined Stack ==============
    //  Start     Current   End       Interrupt
    //  Address   Address   Address   Address
        0x5400,   0x5400,   0x6400,   0, 

    //  =============  Combined Stack ==============
    //  Last Msg  Reserved  Reserved  Interrupt N Words
    //  Address   Address   Address   before End-of-Stack
        0,        0,        0,        512 };
     
    #endif

    volatile unsigned short mt_filter_table[128] = {

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

	    *((volatile unsigned short *)(addr)) = mt_filter_table[i];
	    addr += 2;
        }

	pH6130->MT_ADDR_LIST_POINTER = 0x00B0;

	// ================== Simple Monitor ======================= 

        #if(SMT_ena)

	    // read back address just written and shift left
            j = (pH6130->MT_ADDR_LIST_POINTER << 1);

            // initialize MT address list using array declared at top of function 
            for ( i = 0, addr = base_6130 + j; i < 8; i++) {
              
                *((volatile unsigned short *)(addr)) = smt_addr_list[i];
                
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
            pH6130->MT_INT_ENABLE_REG = STKROVR|DSTKROVR|STKADRSS|DSTKADRSS|MSG_ERR|MT_EOM;

            // Output Enable for hardware ints  
            pH6130->MT_INT_OUTPUT_ENABLE_REG = STKROVR|DSTKROVR|STKADRSS|DSTKADRSS|MSG_ERR|MT_EOM;

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

            pH6130->MT_CONFIG_REG = i;

	// end SMT_ena 

        // ================== IRIG-106 Monitor ======================= 

	#else // (IMT_ena) 

            // read back address just written and shift left 
            j = (pH6130->MT_ADDR_LIST_POINTER) << 1;

            // initialize MT address list using array declared at top of function 
            for ( i = 0, addr = base_6130 + j; i < 8; i++) {

		*((volatile unsigned short *)(addr)) = imt_addr_list[i];
		addr += 2;
            }

            // In addition to these packet size limits, a stack rollover trips packet finalization... 
            pH6130->IMT_MAX_1553_MSGS = 4545; // max possible in 100ms = 4,545
            pH6130->IMT_MAX_1553_WORDS = 0;   // 32320);
            pH6130->IMT_MAX_PKT_TIME = 0;     // 10000); // 10us resolution
            pH6130->IMT_MAX_GAP_TIME = 0;     // max deadtime = 10(N-2)us
            pH6130->IMT_CHANNEL_ID = 0xABCD;

            // Set up IRIG-106 MT interrupts. 
            // 
            // FULL_EOP  = stack full, end-of-packet (words left < 64)
            // FULL_OFS  = stack offset from FULL_EOP interrupt 
            //             (IRIG-106 HEADER MUST BE ENABLED)
            //             (MT address list word 7 contains # words offset)
            // MAXWORDS  = end-of-packet, hit 1553 word count limit per pkt
            // MAXMSGS   = end-of-packet, hit message count limit per pkt
            // MAXGAP    = end-of-packet, exceeded max bus deadtime per pkt
            // MAXTIME   = end-of-packet, hit maximum allowed pkt time
            // HPKTSTOP  = host packet stop interrupt
            // MT_EOM    = end-of-message interrupt
            // STKROVR   = stack end-address written, ptr rolled-over to start address 
            //             (MT address list word 2 contains stack end address)
            // STKADRSS  = stack address written, equal to MT address list word 3
            // PKTRDY    = packet ready interrupt
		
            // Interrupt Enable, vectored ints and polled ints  
            pH6130->MT_INT_ENABLE_REG = FULL_EOP|MAXWORDS|MAXMSGS|MAXGAP|MAXTIME|HPKTSTOP|PKTREADY|STKROVR; 
                                        // not used: |FULL_OFS|STKADRSS|MT_EOM);

            // Output enable for vectored ints  
            pH6130->MT_INT_OUTPUT_ENABLE_REG = FULL_EOP|MAXWORDS|MAXMSGS|MAXGAP|MAXTIME|HPKTSTOP|PKTREADY|STKROVR; 
                                               // not used: |FULL_OFS|STKADRSS|MT_EOM);

            // Optional: assert IMTA bit in the Master Config Reg 0 
            // so the ACTIVE pin reflects MT activity 
            j = pH6130->MASTER_CONFIG_REG;
            pH6130->MASTER_CONFIG_REG = j|IMTA;
		
            // Config options for IRIG-106 monitor, "i" will be written to the
            // MT Configuration Reg. Configure the options for IRIG-106.
            // The IMT mode automatically uses TTAG48. 
            i = SELECT_IMT;

            // Choose 1 from each of the following choices:
            //		 
            // GAPCHKON or GAPCHKOFF
            // STOR_INVWD or STOP_INVWD
            // PKTSTRT_CW or PKTSTRT_ENA
            // IMT_HDR_ON or IMT_HDR_OFF
            // EXTD_STATUS or IRIG_STATUS
            // IMT_CKSUM_ON or IMT_CKSUM_OFF
            // REC_CW or REC_CWDW or REC_CS or REC_CSDS
            // TAG_LWLB or TAG_FWFB or TAG_FWLB or TAG_OFF
            // MTTO_15U or MTTO_20U or MTTO_58U or MTTO_138U 
            // IMT_DTYPE9 or IMT_DTYPE4 or IMT_DTYPE5 or IMT_DTYPE7  
            //  -----------------------------------------------
            //  Note: IMT time tag OFF option is selected in main.c

            i |= GAPCHKOFF|STOR_INVWD|PKTSTRT_CW
                 |IMT_HDR_OFF|EXTD_STATUS|IMT_CKSUM_ON
                 |REC_CSDS|TAG_OFF|MTTO_20U|IMT_DTYPE9;

            pH6130->MT_CONFIG_REG = i;

	#endif // (IMT_ena)

    // ====================================================================================== 

}   // end initialize_613x_MT(void)


// end of file 

