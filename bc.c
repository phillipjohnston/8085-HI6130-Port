/*
* Bus Controller Library
* bc.c
*
* Phillip Johnston
* 8 October 2012
*/

#include "defs.h"
#include "init.h"
#include "bc.h"
#include "regdefs.h"

/******************************
* Global Variable Definitions *
******************************/
extern struct bc_reg_map_t * pH6130;
//extern unsigned int * pGPQ; //DECLARED IN BC.H
extern struct bc_inst_list * pBCil;
extern struct MsgBlock * pBCstack; 
extern struct MsgBlock2RT * pBCstack2RT; 

/***********************
* Function Definitions *
***********************/
void initialize_bc_instruction_list(void) {

	unsigned int i, j, len;
	
	static const unsigned int base_6130 = BC_STARTING_ADDR;
	unsigned int addr;
	
	static unsigned int inst_list[46] = {
		// test op codes WTG,XEQ,JMP, verify various msg block setups
		WTG|ALWAYS, 0x0000,			// wait for ext trigger, addr = BC_ILIST_BASE_ADDR = 0x1B70 
		XEQ|ALWAYS, MSG_BLK1_ADDR,		// 1
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK2_ADDR,		// 2
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK3_ADDR,		// 3
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK4_ADDR,		// 4
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK5_ADDR,		// 5
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK6_ADDR,		// 6
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK7_ADDR,		// 7
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK8_ADDR,		// 8 
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, RTRT_MSG_BLK1_ADDR,		// RT-RT 1
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, RTRT_MSG_BLK2_ADDR,		// RT-RT 2
		WTG|ALWAYS, 0x0000,			// wait for ext trigger
		XEQ|ALWAYS, MSG_BLK2_ADDR,		// 2
		JMP|ALWAYS, BC_ILIST_BASE_ADDR 
	};	// loop to top 

	
	// copy BC Instruction List (above) to RAM...
	
	// instruction list array size, 16-bit words
	len = sizeof(inst_list) / sizeof(unsigned int);
	//#message "LEN calculation needs to be checked to see it produces the correct size"
	
	// set the instruction list start address.
	// BC_ILIST_BASE_ADDR is defined in file 613x_bc.h 
	pH6130->INST_LIST_BASE_ADDR_REG = BC_ILIST_BASE_ADDR;
	
	// using the address just written into the base address register,
	// copy the BC Instruction List (declared above) into the HI-6130 RAM. 
	
	//  set up the address pointer, left-shift the list start address
	addr = base_6130 + (BC_ILIST_BASE_ADDR << 1);	
	
	// write instruction list
	for ( i = 0; i < len; i += 2) {
		
		// add the validation field and odd parity bit,
		// and write the adjusted op code word. 
		j = inst_list[i];
		// the next 4 lines determine odd parity 			
		j ^= j>>8;
		j ^= j>>4;
		j &= 0xF;
		if (!((0x6996 >> j) & 1))  { 
			// write the op code word with parity bit 15 = 1
			j = inst_list[i] + VP1;
			*((unsigned int *)(addr)) = j;
		}
		else {
			// write the op code word with parity bit 15 = 0
			j = inst_list[i] + VP0;
			*((unsigned int *)(addr)) = j;
		}
		
		addr += 2;
		// write parameter word that goes with just-written op code 
		*((unsigned int *)(addr)) = inst_list[i+1];
		addr += 2;
	}
	
}



// 	Next function initializes ten HI-613x Bus Controller Control/Status Blocks 
//      for test purposes, and initializes BC transmit data buffers used with RT receive commands. 
//
//	Msg Block 1  = Subaddress Tx  Command 03-1-30-00 (loopback subaddress) Bus A
//	Msg Block 2  = Subaddress Tx  Command 03-1-30-00 (loopback subaddress) Bus B
//	Msg Block 3  = Subaddress Rx  Command 03-0-30-00 (loopback subaddress) Bus A
//	Msg Block 4  = Subaddress BRx Command 31-0-05-11                       Bus B
//	Msg Block 5  = Subaddress BRx Command 31-0-30-00 (loopback subaddress) Bus A
//	Msg Block 6  = Mode Code  Tx  Command 03-1-31-02 (tx mode code 2)      Bus B
//	Msg Block 7  = Mode Code  Tx  Command 03-1-31-18 (tx mode code 18)     Bus A
//	Msg Block 8  = Mode Code  Rx  Command 03-0-31-21 (rx mode code 21)     Bus B
//      RT-RT Msg Block 1 = RT-RT msg Commands 04-0-30-02 03-1-05-02           Bus A
//      RT-RT Msg Block 2 = BRT-RT msg Commands 31-0-30-11 04-1-05-11          Bus B
// 

void initialize_bc_msg_blocks(void) {
	
	static const unsigned int base_6130 = BC_STARTING_ADDR;
	unsigned int addr;
	
	unsigned int i;
	static unsigned int data[32] = {0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,0x0808,
		0x0909,0x1010,0x1111,0x1212,0x1313,0x1414,0x1515,0x1616,
		0x1717,0x1818,0x1919,0x2020,0x2121,0x2222,0x2323,0x2424,
		0x2525,0x2626,0x2727,0x2828,0x2929,0x3030,0x3131,0x3232};
		
	// ********************************************************************************
	// Msg Block 1  = Subaddress Tx  Command 03-1-30-00 (loopback subaddress) Bus A
	
	static unsigned int msg_block1[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word          
		RTRYENA|MEMASK|USEBUSA,

		//  Command Word (Transmit Command, RT --> BC )
		//  RT Addr Tx/Rx  SubAdd  WordCt 
		3<<11  | TX  | 30<<5 | 0,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5308,  0x0200,  0xABCD,  0xABCD,  0xABCD,  0xABCD };    

	// ********************************************************************************
	// Msg Block 2  = Subaddress Tx  Command 03-1-30-00 (loopback subaddress) Bus B

	static unsigned int msg_block2[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word          
		RTRYENA|MEMASK|USEBUSB,

		//  Command Word (Transmit Command, RT --> BC )
		//  RT Addr Tx/Rx  SubAdd  WordCt 
		3<<11  | TX  | 30<<5 | 0,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5308,  0x0200,  0xABCD,  0xABCD,  0xABCD,  0xABCD };    

	// ********************************************************************************
	// Msg Block 3  = Subaddress Rx  Command 03-0-30-00 (loopback subaddress) Bus A

	static unsigned int msg_block3[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//                         |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word          
		MEMASK|MSKBCR|USEBUSA,

		//  Command Word (Receive Command, BC --> RT )
		//  RT Addr Tx/Rx  SubAdd  WordCt 
		3<<11  | RX  | 30<<5 | 0,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5328,  700,      0xABCD,  0xABCD,  0xABCD,  0xABCD };   

	// ********************************************************************************
	// Msg Block 4  = Subaddress BRx Command 31-0-05-11 Bus B

	static unsigned int msg_block4[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//                         |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|BCST|USEBUSB,

		//  Command Word (Broadcast Receive Command, BC --> RTs )
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		31<<11 |  RX   | 1<<5 | 11,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5348,  0x333,    0xABCD,  0xABCD,  0xABCD,  0xABCD };   

	// ********************************************************************************
	// Msg Block 5  = Subaddress BRx Command 31-0-30-00 (loopback subaddress) Bus A

	static unsigned int msg_block5[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|BCST|USEBUSA,

		//  Command Word (Broadcast Receive Command, BC --> RTs )
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		31<<11 |  RX   |30<<5 | 0,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5368,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD };    

	// ********************************************************************************
	// Msg Block 6  = Mode Code  Tx  Command 03-1-31-02 (tx mode code 2) Bus B


	static unsigned int msg_block6[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|MCODE|USEBUSB,

		//  Command Word (Transmit Mode Command, MC0-MC15 ONLY )
		//  RT Addr  Tx/Rx  SubAdd  MCode 
		3<<11 |  TX   | 31<<5 | 2,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0xDEAD,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD };   

	// ********************************************************************************
	// Msg Block 7  = Mode Code  Tx  Command 03-1-31-18 (tx mode code 18)     Bus A

	static unsigned int msg_block7[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|MCODE|USEBUSA,

		//  Command Word (Transmit Mode Command, MC16-MC31 ONLY )
		//  RT Addr  Tx/Rx  SubAdd  MCode 
		3<<11  |  TX  | 31<<5 | 18,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x1B62,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD };    

	// ********************************************************************************
	// Msg Block 8  = Mode Code  Rx  Command 03-0-31-21 (rx mode code 21)     Bus B

	static unsigned int msg_block8[8] = {
		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|MCODE|TXTTMC17|USEBUSB,

		//  Command Word (Receive Mode Command, MC16-MC31 ONLY )
		//  RT Addr  Tx/Rx  SubAdd  MCode 
		3<<11  |  RX   | 31<<5 | 21,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x1B55,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD };   

	// ********************************************************************************
	// RT-RT Msg Block 1   to RxRT4 from TxRT3   04-0-30-02 03-1-05-02  Bus A

	static unsigned int rtrt_msg_block1[16] = {
		// This format is only used for RT-to-RT messages

		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|RT_RT|USEBUSA,

		//  Rx Command Word 
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		4<<11  |  RX  |  30<<5 | 2,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x5388,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD,  

		//  Tx Command Word 
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		3<<11  |  TX   | 1<<5 | 2,

		//  Rx RT
		//  Status   6 unused addresses for RT-to-RT only       
		0x0000,  0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD };

	// ********************************************************************************
	// RT-RT Msg Block 2   broadcast to RxRT31 from TxRT3    31-0-30-11 04-1-05-11   Bus B

	static unsigned int rtrt_msg_block2[16] = {
		// This format is only used for RT-to-RT messages 

		//  Contro1 Word Options = TXTTMC17|MEMASK|SRQMASK|BSYMASK|SSFMASK|TFMASK|RSVMASK
		//			   |RTRYENA|USEBUSA|USEBUSB|SFTEST|MSKBCR|EOMINT|MCODE|BCST|RT_RT 
		//  Contro1 Word
		MEMASK|MSKBCR|BCST|RT_RT|USEBUSB,

		//  Rx Command Word 
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		31<<11 |  RX   |30<<5 | 11,

		//  Data     Time to   TimeTag  Block    LoopBack RT
		//  Addr     NextMsg   Word     Status   Word     Status 
		0x53A8,  0,        0xABCD,  0xABCD,  0xABCD,  0xABCD,

		//  Tx Command Word 
		//  RT Addr  Tx/Rx  SubAdd  WordCt 
		4<<11  |  TX   | 1<<5 | 11,

		//  Rx RT
		//  Status   6 unused addresses for RT-to-RT only       
		0x0000,  0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD, 0xDEAD };

	// ********************************************************************************

	// Initialize BC Message Control / Status Blocks  

	//  set up the address pointer, left-shift the start address/offset 
	// these message blocks will be contiguous, stored back-to-back 
	addr = base_6130 + (MSG_BLK1_ADDR << 1);	

	// these non-RT-RT blocks need 8 words, must start with low nibble = 0x0 or 0x8 

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block1[i]; // 0x3E00 - 0x3E07
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block2[i]; // 0x3E08 - 0x3E0F
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block3[i]; // 0x3E10 - 0x3E17
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block4[i]; // 0x3E18 - 0x3E1F
		addr += 2;
	}


	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block5[i]; // 0x3E20 - 0x3E27
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block6[i]; // 0x3E28 - 0x3E2F
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block7[i]; // 0x3E30 - 0x3E37
		addr += 2;
	}

	for ( i = 0; i < 8; i++) {
		*((unsigned int *)(addr)) = msg_block8[i]; // 0x3E38 - 0x3E3F
		addr += 2;
	}
	
	// these next RT-RT blocks need 16 words, MUST start with low nibble = 0x0 

	for ( i = 0; i < 16; i++) {
		*((unsigned int *)(addr)) = rtrt_msg_block1[i]; // 0x3E40 - 0x3E4F
		addr += 2;
	}

	for ( i = 0; i < 16; i++) {
		*((unsigned int *)(addr)) = rtrt_msg_block2[i]; // 0x3E50 - 0x3E5F
		addr += 2;
	}


	// write dummy data into the transmit data buffers for the 3 receive subaddress commands 
	// REMEMBER: For Receive commands (that is RT receives), the BC IS TRANSMITTING...

	// a 32-word buffer for rx msg block 3
	addr = msg_block3[2] << 1;
	addr += base_6130;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(addr)) = data[i];
		addr += 2;
	}

	// a 32-word buffer for rx msg block 4
	addr = msg_block4[2] << 1;
	addr += base_6130;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(addr)) = data[i];
		addr += 2;
	}

	// a 32-word buffer for rx msg block 5
	addr = msg_block5[2] << 1;
	addr += base_6130;
	// write the 32 data words...
	for ( i = 0; i < 32; i++) {	
		*((unsigned int *)(addr)) = data[i];
		addr += 2;
	}

	// 16 mode data words for Rx MC16-MC31 
	addr = 0x1B50 << 1;
	addr += base_6130;
	for ( i = 0; i < 16; i++) {	
		// writes 0x1616 for Rx MC16 thru 0x3131 for Rx MC31
		*((unsigned int *)(addr)) = data[i+15];
		addr += 2;
	}
}

//	This function disables the Holt HI-613x BC by writing 
//	the Master Configuration Register to reset the BCENA bit.
//
void bc_disable(void) {
	// read-modify-write Master Config register
	unsigned int j;
	j = ((pH6130->MASTER_CONFIG_REG) & ~BCENA);
	pH6130->MASTER_CONFIG_REG = j;

}

// 	If the BCENA pin is high, this function enables the Holt 
//	HI-613x BC by writing the Master Configuration Register 
//	to set the BCENA bit. Then BC operation only begins after
//	the BCSTART bit or BCTRIG pin gets a rising edge.
//
void bc_enable(void) {
	unsigned int j;
	j = (pH6130->MASTER_CONFIG_REG) | BCENA;
	pH6130->MASTER_CONFIG_REG = j;
}

// 	This function enables and starts 
//	the Holt HI-6130 BC by writing the Master Configuration 
//	Register to set the BCENA and BCSTART bits.
//
void bc_start(void) {
		
	// read-modify-write Master Config register
	unsigned int j;
	j = (pH6130->MASTER_CONFIG_REG) | BCENA | BCSTRT ;
	pH6130->MASTER_CONFIG_REG = j;
}

/*
initialize_6130_BC():
	This function initializes the Holt HI-6130 BC by writing configuration registers in the device. 
	Only BC mode option bits are affected. The program has already initialized the common parameters
	shared by the BC, RT1, RT2 and/or Bus Monitor.
*/
void initialize_6130_BC(void) 
{
	pH6130->CONFIG_REG = BCTO_138U|BCGTE|ETTSYN|TTSYNEN|BCRME|BCRE;//|BSYNDV|MENDV|RESBS|CHKCWFMT|BCGCE|BC2RE|BCR1A|BCR2A;
	//pH6130->INST_LIST_BASE_ADDR_REG = OP_START_ADDR;
	//pH6130->GP_QUEUE_POINTER = 0x00C0;  // default (64 locations)
	pH6130->INT_ENABLE_REG = BCWDT|SELMSG|BCGPQ|BCRETRY|CSTKERR|BCTRAP|STATSET|BCIRQMASK|BCMERR|BCEOM;
	pH6130->INT_OUTPUT_ENABLE_REG = BCWDT|SELMSG|BCGPQ|BCRETRY|CSTKERR|STATSET|BCIRQMASK|BCMERR|BCEOM; //|BCTRAP
	
}
