/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	device_6130.h
 *    brief     For projects using the Holt HI-6130 device with bus interface, 
 *              this file contains definitions applicable to all terminal modes
 *              (BC, RT, SMT, IMT). (Does not apply for SPI HI-6131 projects.)
 *              File includes structures (structs) for HI-6130 register addressing.
 *
 *		IMPORTANT: Edit file 613x_initialization.h to define either:
 *
 *		HOST_BUS_INTERFACE  YES  sets compilation for HI-6130 (bus)
 *		 		    NO   sets compilation for HI-6131 (SPI)
 *
 *		Most functional procedures in this project are written for
 *		both parallel bus or SPI host interface schemes, this runtime 
 *		switch determines which are procedures are compiled.
 *
 *              IMPORTANT: Because register addressing differs for Bus and 
 *              SPI interface, file "device_6131.h" contains definitions needed 
 *              for register addressing and functions for using the SPI HI-6131.
 *
 *	   	HOLT DISCLAIMER
 *      	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
 *      	KIND, EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 *      	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 *      	PURPOSE AND NONINFRINGEMENT. 
 *      	IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *      	OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *      	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *
 *              Copyright (C) 2009-2011 by  HOLT, Inc.
 *              All Rights Reserved
 */

                                           

//--------------------------------------------------------------------
// External Bus Interface (EBI) Initialization Data for ARM Cortex M3
//--------------------------------------------------------------------
//  The EBI CSR registers are initialized with values below during execution of
//  "Cstartup_HI6110_RT.c". The chip selects CSR0 - CSR3 have dedicated outputs.
//  Chip selects CSR4 - CSR7 are muxed with upper address lines. The EBI_MCR 
//  register may have to be written to override the reset state (all 4 are address
//  outputs. However, the Cstartup file appears to initialize all 4 as CS signals.
// ---------------------------------------------------

/**************************************************************
 *    Structs for Holt HI-6130 MIL-STD-1553 Protocol Device   *
 **************************************************************/

typedef volatile unsigned short HI6130_REG ; // half-word (16-bits) 

// Using 16-bit bus transfer, all register addresses are even, so bit 0 is not used. (Bit 0
// is used for 8-bit bus configuration.) This example uses 16-bit mode, so ARM EBI address 
// bits 14:0 convey HI-6130 register address bits 15:1. Thus, all register addresses on bus 
// are left-shifted one bit.
//
// The struct below contains HI-6130 registers used when configured as Bus Controller. 
// The register list is comprised of registers used exclusively by the Bus Controller, as
// well as registers shared by the BC, RT, SMT and IMT terminals, if enabled. The shared
// register list consists of hardware interrupt registers, the interrupt log buffer pointer,
// and the time tag configuration register. Other structs in this file contain comparable 
// register sets for RT, SMT and IMT terminals, as well as test mode. 
//
// Note:  These structs are not used for SPI-interface HI-6131 projects.
//
// BC register addressing example:             i = pH6130BC->MASTER_CONFIG_REG;
// assuming this declaration is within scope:  const H6130BC pH6130BC = HI6130BC_BASE;   

typedef struct H6130_BC_REGS {	             // EBI Bus Addresses
	HI6130_REG MASTER_CONFIG_REG; 
	HI6130_REG STATUS_AND_RESET_REG; 
        HI6130_REG dummy03[4];            // skip over 4 non-BC registers
	HI6130_REG HDW_PENDING_INT_REG; 
	HI6130_REG PENDING_INT_REG; 
        HI6130_REG dummy0F[2];            // skip over 2 non-BC registers
	HI6130_REG INT_COUNT_AND_LOG_ADDR_REG; 
        HI6130_REG dummy15[4];            // skip over 4 HI-6131 registers
	HI6130_REG HDW_INT_ENABLE_REG; 
	HI6130_REG INT_ENABLE_REG; 
        HI6130_REG dummy21[2];            // skip over 2 non-BC registers      
	HI6130_REG HDW_INT_OUTPUT_ENABLE_REG; 
	HI6130_REG BC_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG dummy29[29];           // skip over 29 non-BC registers        	    
	HI6130_REG BC_CONFIG_REG; 
	HI6130_REG BC_INST_LIST_BASE_ADDR_REG; 
	HI6130_REG BC_INST_LIST_POINTER; 
	HI6130_REG BC_FRAME_TIME_LEFT_REG; 
	HI6130_REG BC_TIME_TO_NEXT_MSG_REG; 
	HI6130_REG BC_CCODE_AND_GPF_REG; 
	HI6130_REG BC_GP_QUEUE_POINTER; 
	HI6130_REG TTAG_CONFIG_REG; 
        HI6130_REG dummy73[9];             // skip over 9 non-BC registers
	HI6130_REG BC_TTAG_COUNT_LOW; 
	HI6130_REG BC_TTAG_COUNT_HIGH; 
	HI6130_REG BC_TTAG_UTILITY_REG_LOW; 
	HI6130_REG BC_TTAG_UTILITY_REG_HIGH; 
	HI6130_REG BC_TTAG_MATCH_LOW; 
	HI6130_REG BC_TTAG_MATCH_HIGH; 
        HI6130_REG dummy91[5];             // skip over 5 non-BC registers
	HI6130_REG EEPROM_UNLOCK_REG; 
	HI6130_REG BC_LAST_MSG_BLOCK_ADDR_REG; 
	HI6130_REG BC_DEFAULT_WMI_REG; 
        
} HI6130BC, *H6130BC ;		

#define HI6130_BC_BASE	((H6130BC) 0x60000000) 	// using EBI chip select NCS0 CM3


//-----------------------------------------------------------------------------------


// The struct below contains HI-6130 registers used when configured as Remote Terminal. 
// The register list is comprised of registers used exclusively by the Remote Terminal, as
// well as registers shared by the BC, RT, SMT and IMT terminals, if enabled. The shared
// register list consists of hardware interrupt registers, the interrupt log buffer pointer,
// and the time tag configuration register. Other structs in this file contain comparable 
// register sets for RT, SMT and IMT terminals, as well as test mode. 
//
// Note:  These structs are not used for SPI-interface HI-6131 projects.
//
// RT1 or RT2 register addressing example:     i = pH6130RT->MASTER_CONFIG_REG;
// assuming this declaration is within scope:  const H6130RT pH6130RT = HI6130RT_BASE;   

// Using typedef declared earlier: typedef volatile unsigned short HI6130_REG;  

typedef struct H6130_RT_REGS {	          // EBI Bus Addresses 
	HI6130_REG MASTER_CONFIG_REG; 
	HI6130_REG STATUS_AND_RESET_REG; 
	HI6130_REG RT1_CURR_CMD_REG; 
	HI6130_REG RT1_CURR_CTRL_WORD_ADDR_REG; 
	HI6130_REG RT2_CURR_CMD_REG; 
	HI6130_REG RT2_CURR_CTRL_WORD_ADDR_REG; 
	HI6130_REG HDW_PENDING_INT_REG; 
        HI6130_REG dummy0D[2];            // skip over 2 non-RT registers
        HI6130_REG RT_PENDING_INT_REG; 
	HI6130_REG INT_COUNT_AND_LOG_ADDR_REG; 
        HI6130_REG dummy15[4];            // skip over 4 HI-6131 registers
	HI6130_REG HDW_INT_ENABLE_REG; 
        HI6130_REG dummy1F[2];            // skip over 2 non-RT registers
	HI6130_REG RT_INT_ENABLE_REG; 
	HI6130_REG HDW_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG dummy27[2];            // skip over 2 non-RT registers
	HI6130_REG RT_INT_OUTPUT_ENABLE_REG; 
	HI6130_REG RT1_CONFIG_REG; 
	HI6130_REG RT1_OP_STATUS_REG; 
	HI6130_REG RT1_DESC_TBL_BASE_ADDR_REG; 
	HI6130_REG RT1_1553_STATUS_BITS_REG; 
	HI6130_REG RT1_MSG_INFO_WD_ADDR_REG; 
	HI6130_REG RT1_BUSA_SELECT_REG; 
	HI6130_REG RT1_BUSB_SELECT_REG; 
	HI6130_REG RT1_BIT_WORD_REG; 
	HI6130_REG RT1_ALT_BIT_WORD_REG; 
	HI6130_REG RT2_CONFIG_REG; 
	HI6130_REG RT2_OP_STATUS_REG; 
	HI6130_REG RT2_DESC_TBL_BASE_ADDR_REG; 
	HI6130_REG RT2_1553_STATUS_BITS_REG; 
	HI6130_REG RT2_MSG_INFO_WD_ADDR_REG; 
	HI6130_REG RT2_BUSA_SELECT_REG; 
	HI6130_REG RT2_BUSB_SELECT_REG; 
	HI6130_REG RT2_BIT_WORD_REG; 
	HI6130_REG RT2_ALT_BIT_WORD_REG; 
        HI6130_REG dummy51[16];            // skip over 16 non-RT registers
	HI6130_REG TTAG_CONFIG_REG; 
        HI6130_REG dummy73[15];            // skip over 15 non-RT registers       
	HI6130_REG RT1_TTAG_COUNT; 
	HI6130_REG RT1_TTAG_UTILITY_REG; 
	HI6130_REG RT2_TTAG_COUNT; 
	HI6130_REG RT2_TTAG_UTILITY_REG; 
	HI6130_REG RT_CONFIG2_REG; 
	HI6130_REG EEPROM_UNLOCK_REG; 
       
} HI6130RT, *H6130RT ;		

#define HI6130RT_BASE	((H6130RT) 0x60000000) 	// using EBI chip select NCS0 CM3


//----------------------------------------------------------------------------------------


// The struct below contains HI-6130 registers used when configured as Simple Bus Monitor. 
// The register list is comprised of registers used exclusively by the Simple Bus Monitor, as
// well as registers shared by the BC, RT, SMT and IMT terminals, if enabled. The shared
// register list consists of hardware interrupt registers, the interrupt log buffer pointer,
// and the time tag configuration register. Other structs in this file contain comparable 
// register sets for RT, SMT and IMT terminals, as well as test mode. 
//
// Note:  These structs are not used for SPI-interface HI-6131 projects.
//
// SMT register addressing example:            i = pH6130SMT->MASTER_CONFIG_REG;
// assuming this declaration is within scope:  const H6130SMT pH6130SMT = HI6130SMT_BASE;   

// Using typedef declared earlier: typedef volatile unsigned short HI6130_REG;  

typedef struct H6130_SMT_REGS {	              // EBI Bus Addresses 
	HI6130_REG MASTER_CONFIG_REG; 
	HI6130_REG STATUS_AND_RESET_REG; 
        HI6130_REG dummy03[4];            // skip over 4 non-SMT registers
	HI6130_REG HDW_PENDING_INT_REG; 
        HI6130_REG dummy0D;               // skip over 1 non-SMT register
	HI6130_REG MT_PENDING_INT_REG; 
        HI6130_REG dummy11;               // skip over 1 non-SMT register
        HI6130_REG INT_COUNT_AND_LOG_ADDR_REG; 
        HI6130_REG dummy15[4];            // skip over 4 HI-6131 registers
	HI6130_REG HDW_INT_ENABLE_REG; 
        HI6130_REG dummy1F;               // skip over 1 non-SMT register
        HI6130_REG MT_INT_ENABLE_REG; 
        HI6130_REG dummy23[3];            // skip over 3 non-SMT registers
        HI6130_REG MT_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG dummy2B[19];           // skip over 19 non-SMT registers
        HI6130_REG MT_CONFIG_REG; 
        HI6130_REG dummy53[5];            // skip over 5 non-SMT registers
        HI6130_REG MT_ADDR_LIST_POINTER; 
	HI6130_REG MT_NEXT_MSG_STACK_ADDR_REG; 
	HI6130_REG MT_LAST_MSG_STACK_ADDR_REG; 
        HI6130_REG dummy63[7];             // skip over 7 non-SMT registers
        HI6130_REG TTAG_CONFIG_REG; 
	HI6130_REG MT_TTAG_COUNT_LOW; 
	HI6130_REG MT_TTAG_COUNT_MID; 
	HI6130_REG MT_TTAG_COUNT_HIGH; 
	HI6130_REG MT_TTAG_UTILITY_REG_LOW; 
	HI6130_REG MT_TTAG_UTILITY_REG_MID; 
	HI6130_REG MT_TTAG_UTILITY_REG_HIGH; 
	HI6130_REG MT_TTAG_MATCH_LOW; 
	HI6130_REG MT_TTAG_MATCH_MID; 
	HI6130_REG MT_TTAG_MATCH_HIGH; 
        HI6130_REG dummy85[11];            // skip over 11 non-SMT registers
        HI6130_REG EEPROM_UNLOCK_REG; 
       
} HI6130SMT, *H6130SMT ;		

#define HI6130SMT_BASE	((H6130SMT) 0x60000000) 	// using EBI chip select NCS0 CM3


//-----------------------------------------------------------------------------------------


// The struct below contains HI-6130 registers used when configured as IRIG-106 Monitor Terminal. 
// The register list is comprised of registers used exclusively by the IRIG-106 Monitor Terminal,
// as well as registers shared by the BC, RT, SMT and IMT terminals, if enabled. The shared
// register list consists of hardware interrupt registers, the interrupt log buffer pointer,
// and the time tag configuration register. Other structs in this file contain comparable 
// register sets for RT, SMT and IMT terminals, as well as test mode. 
//
// Note:  These structs are not used for SPI-interface HI-6131 projects.
//
// IMT register addressing example:            i = pH6130IMT->MASTER_CONFIG_REG;
// assuming this declaration is within scope:  const H6130IMT pH6130IMT = HI6130IMT_BASE;   

// Using typedef declared earlier: typedef volatile unsigned short HI6130_REG;  

typedef struct H6130_IMT_REGS {	              // EBI Bus Addresses 
	HI6130_REG MASTER_CONFIG_REG; 
	HI6130_REG STATUS_AND_RESET_REG; 
        HI6130_REG dummy03[4];            // skip over 4 non-IMT registers
        HI6130_REG HDW_PENDING_INT_REG; 
        HI6130_REG dummy0D;               // skip over 1 non-IMT register
        HI6130_REG MT_PENDING_INT_REG; 
        HI6130_REG dummy11;               // skip over 1 non-IMT register
	HI6130_REG INT_COUNT_AND_LOG_ADDR_REG; 
        HI6130_REG dummy15[4];            // skip over 4 HI-6131 registers
	HI6130_REG HDW_INT_ENABLE_REG; 
        HI6130_REG dummy1F;               // skip over 1 non-IMT register
	HI6130_REG MT_INT_ENABLE_REG; 
        HI6130_REG dummy23;               // skip over 1 non-IMT register
	HI6130_REG HDW_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG dummy27;               // skip over 1 non-IMT register
	HI6130_REG MT_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG dummy2B[19];           // skip over 19 non-IMT registers
        HI6130_REG MT_CONFIG_REG; 
	HI6130_REG IMT_MAX_1553_MSGS; 
	HI6130_REG IMT_MAX_1553_WORDS; 
	HI6130_REG IMT_MAX_PKT_TIME; 
	HI6130_REG IMT_MAX_GAP_TIME; 
	HI6130_REG IMT_CHANNEL_ID; 
	HI6130_REG MT_ADDR_LIST_POINTER; 
	HI6130_REG MT_NEXT_MSG_STACK_ADDR_REG; 
	HI6130_REG MT_LAST_MSG_STACK_ADDR_REG; 
        HI6130_REG dummy63[15];           // skip over 7 non-IMT registers
        HI6130_REG TTAG_CONFIG_REG; 
	HI6130_REG MT_TTAG_COUNT_LOW; 
	HI6130_REG MT_TTAG_COUNT_MID; 
	HI6130_REG MT_TTAG_COUNT_HIGH; 
	HI6130_REG MT_TTAG_UTILITY_REG_LOW; 
	HI6130_REG MT_TTAG_UTILITY_REG_MID; 
	HI6130_REG MT_TTAG_UTILITY_REG_HIGH; 
	HI6130_REG MT_TTAG_MATCH_LOW; 
	HI6130_REG MT_TTAG_MATCH_MID; 
	HI6130_REG MT_TTAG_MATCH_HIGH; 
        HI6130_REG dummy85[11];           // skip over 11 non-IMT registers       
	HI6130_REG EEPROM_UNLOCK_REG; 
       
} HI6130IMT, *H6130IMT ;		

#define HI6130IMT_BASE	((H6130IMT) 0x60000000) 	// using EBI chip select NCS0 CM3


//--------------------------------------------------------------------------------


// This struct contains HI-6130 registers used in test mode. These locations are
// multiplexed with other functions when operating normally, i.e., not test mode.
//
// Test mode register addressing example:     i = pH6130TST->TEST_CONTROL_REG;
// assuming this declaration is within scope: const H6130TST pH6130TST = HI6130TST_BASE;   

// Using typedef declared earlier: typedef volatile unsigned short HI6130_REG;  

typedef struct H6130_TST_REGS {	             // EBI Bus Addresses 
	HI6130_REG dummy00[2];           // skip over 2 non-test registers        
	HI6130_REG LOOPBACK_RX_TEST_DATA_REG; 
        HI6130_REG dummy05[24];          // skip over 24 non-test registers
        HI6130_REG RAM_BIST_FAIL_ADDR_REG; 
        HI6130_REG dummy37[3];           // skip over 3 non-test registers
        HI6130_REG LOOPBACK_TX_TEST_DATA_REG; 
        HI6130_REG dummy3F[4];           // skip over 4 non-test registers
        HI6130_REG RAM_TEST_FAIL_ADDR_REG; 
        HI6130_REG dummy49[3];           // skip over 3 non-test registers
        HI6130_REG LOOPBACK_AND_BIST_TEST_CONTROL_REG; 
        HI6130_REG dummy51[36];          // skip over 36 non-test registers
        HI6130_REG TEST_CONTROL_REG; 
	HI6130_REG CHECKSUM_FAIL_ADDRESS_REG; 
       
} HI6130TST, *H6130TST ;		

#define HI6130TST_BASE	((H6130TST) 0x60000000) 	// using EBI chip select NCS0 CM3



//------------------------------------------------------------------------------------


// This struct contains all HI-6130 registers. If using one mode, it may be more convenient
// to instead use one of the smaller structs above, for each terminal types: BC, RT, SMT or IMT.
//
// Register addressing example:               i = pH6130->MASTER_CONFIG_REG;
// assuming this declaration is within scope: const H6130 pH6130 = HI6130_BASE;   

// Using typedef declared earlier: typedef volatile unsigned short HI6130_REG;  

typedef struct H6130_REGS {	          // EBI Bus Addresses 
	HI6130_REG MASTER_CONFIG_REG; 
        HI6130_REG STATUS_AND_RESET_REG; 
        HI6130_REG RT1_CURR_CMD_REG; 
        HI6130_REG RT1_CURR_CTRL_WORD_ADDR_REG; 
        HI6130_REG RT2_CURR_CMD_REG; 
        HI6130_REG RT2_CURR_CTRL_WORD_ADDR_REG; 
        HI6130_REG HDW_PENDING_INT_REG; 
        HI6130_REG BC_PENDING_INT_REG; 
        HI6130_REG MT_PENDING_INT_REG; 
        HI6130_REG RT_PENDING_INT_REG; 
        HI6130_REG INT_COUNT_AND_LOG_ADDR_REG; 
        HI6130_REG dummy15[4];            // skip over 4 HI-6131 registers
	HI6130_REG HDW_INT_ENABLE_REG; 
        HI6130_REG BC_INT_ENABLE_REG; 
        HI6130_REG MT_INT_ENABLE_REG; 
        HI6130_REG RT_INT_ENABLE_REG; 
        HI6130_REG HDW_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG BC_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG MT_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG RT_INT_OUTPUT_ENABLE_REG; 
        HI6130_REG RT1_CONFIG_REG; 
        HI6130_REG RT1_OP_STATUS_REG; 
        HI6130_REG RT1_DESC_TBL_BASE_ADDR_REG; 
        HI6130_REG RT1_1553_STATUS_BITS_REG; 
        HI6130_REG RT1_MSG_INFO_WD_ADDR_REG; 
        HI6130_REG RT1_BUSA_SELECT_REG; 
        HI6130_REG RT1_BUSB_SELECT_REG; 
        HI6130_REG RT1_BIT_WORD_REG; 
        HI6130_REG RT1_ALT_BIT_WORD_REG; 
        HI6130_REG RT2_CONFIG_REG; 
        HI6130_REG RT2_OP_STATUS_REG; 
        HI6130_REG RT2_DESC_TBL_BASE_ADDR_REG; 
        HI6130_REG RT2_1553_STATUS_BITS_REG; 
        HI6130_REG RT2_MSG_INFO_WD_ADDR_REG; 
        HI6130_REG RT2_BUSA_SELECT_REG; 
        HI6130_REG RT2_BUSB_SELECT_REG; 
        HI6130_REG RT2_BIT_WORD_REG; 
        HI6130_REG RT2_ALT_BIT_WORD_REG; 
        HI6130_REG MT_CONFIG_REG; 
        HI6130_REG IMT_MAX_1553_MSGS; 
        HI6130_REG IMT_MAX_1553_WORDS; 
        HI6130_REG IMT_MAX_PKT_TIME; 
        HI6130_REG IMT_MAX_GAP_TIME; 
        HI6130_REG IMT_CHANNEL_ID; 
        HI6130_REG MT_ADDR_LIST_POINTER; 
        HI6130_REG MT_NEXT_MSG_STACK_ADDR_REG; 
        HI6130_REG MT_LAST_MSG_STACK_ADDR_REG; 
        HI6130_REG BC_CONFIG_REG; 
        HI6130_REG BC_INST_LIST_BASE_ADDR_REG; 
        HI6130_REG BC_INST_LIST_POINTER; 
        HI6130_REG BC_FRAME_TIME_LEFT_REG; 
        HI6130_REG BC_TIME_TO_NEXT_MSG_REG; 
        HI6130_REG BC_CCODE_AND_GPF_REG; 
        HI6130_REG BC_GP_QUEUE_POINTER; 
        HI6130_REG TTAG_CONFIG_REG; 
        HI6130_REG MT_TTAG_COUNT_LOW; 
        HI6130_REG MT_TTAG_COUNT_MID; 
        HI6130_REG MT_TTAG_COUNT_HIGH; 
        HI6130_REG MT_TTAG_UTILITY_REG_LOW; 
        HI6130_REG MT_TTAG_UTILITY_REG_MID; 
        HI6130_REG MT_TTAG_UTILITY_REG_HIGH; 
        HI6130_REG MT_TTAG_MATCH_LOW; 
        HI6130_REG MT_TTAG_MATCH_MID; 
        HI6130_REG MT_TTAG_MATCH_HIGH; 
        HI6130_REG BC_TTAG_COUNT_LOW; 
        HI6130_REG BC_TTAG_COUNT_HIGH; 
        HI6130_REG BC_TTAG_UTILITY_REG_LOW; 
        HI6130_REG BC_TTAG_UTILITY_REG_HIGH; 
        HI6130_REG BC_TTAG_MATCH_LOW; 
        HI6130_REG BC_TTAG_MATCH_HIGH; 
        HI6130_REG RT1_TTAG_COUNT; 
        HI6130_REG RT1_TTAG_UTILITY_REG; 
        HI6130_REG RT2_TTAG_COUNT; 
        HI6130_REG RT2_TTAG_UTILITY_REG; 
        HI6130_REG RT_CONFIG2_REG; 
       	HI6130_REG EEPROM_UNLOCK_REG; 
        HI6130_REG BC_LAST_MSG_BLOCK_ADDR_REG; 
        HI6130_REG BC_DEFAULT_WMI_REG; 
       
} HI6130, *H6130 ;		

#define HI6130_BASE	((H6130) 0x60000000) 	// using EBI chip select NCS0 CM3



// end of file

