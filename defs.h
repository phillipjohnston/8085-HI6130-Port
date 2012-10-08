/*
*   HI-6130 Definitions
*
*	6130defs.h
*
*	MICRO-C Port, 8-bit xfers
*	For the 8085 Microcontroller
*
*	Phillip Johnston
*	6 October 2012
*/

#include config.h

/***************
* Definitions *
***************/
#define HI6130_BC_BASE // ????? What's the base address?
#define HI6130_RT_BASE
#define HI6130_BM_BASE


/*******************************
* Register Map Definitions *
*******************************/

#ifdef I_AM_BC
struct bc_reg_map_t
{
	unsigned int MASTER_CONFIG_REG; 
	unsigned int STATUS_AND_RESET_REG; 
	unsigned int dummy03[4];            // skip over 4 non-BC registers
	unsigned int HDW_PENDING_INT_REG; 
	unsigned int PENDING_INT_REG; 
	unsigned int dummy0F[2];            // skip over 2 non-BC registers
	unsigned int INT_COUNT_AND_LOG_ADDR_REG; 
	unsigned int dummy15[4];            // skip over 4 HI-6131 registers
	unsigned int HDW_INT_ENABLE_REG; 
	unsigned int INT_ENABLE_REG; 
	unsigned int dummy21[2];            // skip over 2 non-BC registers      
	unsigned int HDW_INT_OUTPUT_ENABLE_REG; 
	unsigned int BC_INT_OUTPUT_ENABLE_REG; 
	unsigned int dummy29[29];           // skip over 29 non-BC registers        	    
	unsigned int BC_CONFIG_REG; 
	unsigned int BC_INST_LIST_BASE_ADDR_REG; 
	unsigned int BC_INST_LIST_POINTER; 
	unsigned int BC_FRAME_TIME_LEFT_REG; 
	unsigned int BC_TIME_TO_NEXT_MSG_REG; 
	unsigned int BC_CCODE_AND_GPF_REG; 
	unsigned int BC_GP_QUEUE_POINTER; 
	unsigned int TTAG_CONFIG_REG; 
	unsigned int dummy73[9];             // skip over 9 non-BC registers
	unsigned int BC_TTAG_COUNT_LOW; 
	unsigned int BC_TTAG_COUNT_HIGH; 
	unsigned int BC_TTAG_UTILITY_REG_LOW; 
	unsigned int BC_TTAG_UTILITY_REG_HIGH; 
	unsigned int BC_TTAG_MATCH_LOW; 
	unsigned int BC_TTAG_MATCH_HIGH; 
	unsigned int dummy91[5];             // skip over 5 non-BC registers
	unsigned int EEPROM_UNLOCK_REG; 
	unsigned int BC_LAST_MSG_BLOCK_ADDR_REG; 
	unsigned int BC_DEFAULT_WMI_REG;
}; //HI6130BC, *HI6130BC ;
#endif

#ifdef I_AM_RT
struct rt_reg_map_t
{
	unsigned int MASTER_CONFIG_REG; 
	unsigned int STATUS_AND_RESET_REG; 
	unsigned int RT1_CURR_CMD_REG; 
	unsigned int RT1_CURR_CTRL_WORD_ADDR_REG; 
	unsigned int RT2_CURR_CMD_REG; 
	unsigned int RT2_CURR_CTRL_WORD_ADDR_REG; 
	unsigned int HDW_PENDING_INT_REG; 
	unsigned int dummy0D[2];            // skip over 2 non-RT registers
	unsigned int RT_PENDING_INT_REG; 
	unsigned int INT_COUNT_AND_LOG_ADDR_REG; 
	unsigned int dummy15[4];            // skip over 4 HI-6131 registers
	unsigned int HDW_INT_ENABLE_REG; 
	unsigned int dummy1F[2];            // skip over 2 non-RT registers
	unsigned int RT_INT_ENABLE_REG; 
	unsigned int HDW_INT_OUTPUT_ENABLE_REG; 
	unsigned int dummy27[2];            // skip over 2 non-RT registers
	unsigned int RT_INT_OUTPUT_ENABLE_REG; 
	unsigned int RT1_CONFIG_REG; 
	unsigned int RT1_OP_STATUS_REG; 
	unsigned int RT1_DESC_TBL_BASE_ADDR_REG; 
	unsigned int RT1_1553_STATUS_BITS_REG; 
	unsigned int RT1_MSG_INFO_WD_ADDR_REG; 
	unsigned int RT1_BUSA_SELECT_REG; 
	unsigned int RT1_BUSB_SELECT_REG; 
	unsigned int RT1_BIT_WORD_REG; 
	unsigned int RT1_ALT_BIT_WORD_REG; 
	unsigned int RT2_CONFIG_REG; 
	unsigned int RT2_OP_STATUS_REG; 
	unsigned int RT2_DESC_TBL_BASE_ADDR_REG; 
	unsigned int RT2_1553_STATUS_BITS_REG; 
	unsigned int RT2_MSG_INFO_WD_ADDR_REG; 
	unsigned int RT2_BUSA_SELECT_REG; 
	unsigned int RT2_BUSB_SELECT_REG; 
	unsigned int RT2_BIT_WORD_REG; 
	unsigned int RT2_ALT_BIT_WORD_REG; 
	unsigned int dummy51[16];            // skip over 16 non-RT registers
	unsigned int TTAG_CONFIG_REG; 
	unsigned int dummy73[15];            // skip over 15 non-RT registers       
	unsigned int RT1_TTAG_COUNT; 
	unsigned int RT1_TTAG_UTILITY_REG; 
	unsigned int RT2_TTAG_COUNT; 
	unsigned int RT2_TTAG_UTILITY_REG; 
	unsigned int RT_CONFIG2_REG; 
	unsigned int EEPROM_UNLOCK_REG; 
};
#endif

#ifdef I_AM_BM
struct bm_reg_map_t
{
	unsigned int MASTER_CONFIG_REG; 
	unsigned int STATUS_AND_RESET_REG; 
	unsigned int dummy03[4];            // skip over 4 non-SMT registers
	unsigned int HDW_PENDING_INT_REG; 
	unsigned int dummy0D;               // skip over 1 non-SMT register
	unsigned int MT_PENDING_INT_REG; 
	unsigned int dummy11;               // skip over 1 non-SMT register
	unsigned int INT_COUNT_AND_LOG_ADDR_REG; 
	unsigned int dummy15[4];            // skip over 4 HI-6131 registers
	unsigned int HDW_INT_ENABLE_REG; 
	unsigned int dummy1F;               // skip over 1 non-SMT register
	unsigned int MT_INT_ENABLE_REG; 
	unsigned int dummy23[3];            // skip over 3 non-SMT registers
	unsigned int MT_INT_OUTPUT_ENABLE_REG; 
	unsigned int dummy2B[19];           // skip over 19 non-SMT registers
	unsigned int MT_CONFIG_REG; 
	unsigned int dummy53[5];            // skip over 5 non-SMT registers
	unsigned int MT_ADDR_LIST_POINTER; 
	unsigned int MT_NEXT_MSG_STACK_ADDR_REG; 
	unsigned int MT_LAST_MSG_STACK_ADDR_REG; 
	unsigned int dummy63[7];             // skip over 7 non-SMT registers
	unsigned int TTAG_CONFIG_REG; 
	unsigned int MT_TTAG_COUNT_LOW; 
	unsigned int MT_TTAG_COUNT_MID; 
	unsigned int MT_TTAG_COUNT_HIGH; 
	unsigned int MT_TTAG_UTILITY_REG_LOW; 
	unsigned int MT_TTAG_UTILITY_REG_MID; 
	unsigned int MT_TTAG_UTILITY_REG_HIGH; 
	unsigned int MT_TTAG_MATCH_LOW; 
	unsigned int MT_TTAG_MATCH_MID; 
	unsigned int MT_TTAG_MATCH_HIGH; 
	unsigned int dummy85[11];            // skip over 11 non-SMT registers
	unsigned int EEPROM_UNLOCK_REG; 
};
#endif

#ifdef USE_TEST_REGS
struct test_reg_map_t
{
	unsigned int dummy00[2];           // skip over 2 non-test registers        
	unsigned int LOOPBACK_RX_TEST_DATA_REG; 
	unsigned int dummy05[24];          // skip over 24 non-test registers
	unsigned int RAM_BIST_FAIL_ADDR_REG; 
	unsigned int dummy37[3];           // skip over 3 non-test registers
	unsigned int LOOPBACK_TX_TEST_DATA_REG; 
	unsigned int dummy3F[4];           // skip over 4 non-test registers
	unsigned int RAM_TEST_FAIL_ADDR_REG; 
	unsigned int dummy49[3];           // skip over 3 non-test registers
	unsigned int LOOPBACK_AND_BIST_TEST_CONTROL_REG; 
	unsigned int dummy51[36];          // skip over 36 non-test registers
	unsigned int TEST_CONTROL_REG; 
	unsigned int CHECKSUM_FAIL_ADDRESS_REG;
};
#endif