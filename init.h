/*
*	HI-6130 Initialization Settings / Macros
*	6130_initialization.h
*
*	Phillip Johnston
*	6 October 2012
*/

#include config.h

/************************
*  Macro Definitions  *
************************/
//    brief	Macros for enabling or disabling HI-613x terminal devices when building
//		the C project. Corresponding bit is "1" for enabled devices, or "0" for
//		disabled terminal devices 
//     NOTE:    ENABLED FUNCTIONS MUST HAVE CORRESPONDNG DEVICE ENABLE PINS CONNECTED HIGH
//              (CONTROLLED BY 4 DIP SWITCHES), OTHERWISE PROGRAM EXECUTION GOES TO ERROR TRAP


//    brief	This next group applies only when using auto-initialization. It selects 
//		how the IC logic writes the serial EEPROM (at the EECOPY process).
//              When auto-initialization is subsequently enabled, the auto-init
//              process first copies data into the registers and RAM, without asserting
//              BCENA, RT1ENA, RT2ENA or MTRUN in the Master Configuration register 0.
//
//              If all 4 auto-start bits below are logic-0, auto-init is complete and READY
//              is asserted. Terminal execution does not start for initialized devices until
//              the host sets the BCENA, RT1ENA, RT2ENA or MTRUN in Master Configuration 
//              register 0. 
//
//		If any of the 4 auto-start bits below are logic-1, at the completion of 
//              auto-init the device logic automatically sets BCENA, RT1ENA, RT2ENA or MTRUN 
//              bits corresponding to the autostart flags set below. When READY is asserted,
//              those terminal devices are already running. If there are remaining logic-0
//		autostart bits below, upon READY assertion the host must set the corresponding
//		register 0 ENA or RUN bit to enable those terminal devices.  

#define BC_autostart 0
#define RT1_autostart 0
#define RT2_autostart 0
#define SMT_IMT_autostart 0


/*******************
* Misc. Macros *
*******************/
#define BUS_A 0
#define BUS_B	1
#define YES 1
#define NO 0


/*
*	Time Tag Counter Resolution
*/
#define BC_TTAG_HI_RES  // high resolution is 32-bit time tag. Comment-out line for low res 16-bit count.
#define SMT_TTAG_HI_RES // high resolution is 48-bit time tag. Comment-out line for low res 16-bit count.

/*
*	Macro for enabling/disabling RT1/RT2 "illegal command detection" per MIL-STD-1553
*/
#define ILLEGAL_CMD_DETECT YES // YES = HI-6130 host bus interface
//  NO = "in form" response for all valid commands

/*
* 	Macro for enabling/disabling RT1/RT2 broadcast commands per MIL-STD-1553
*/
#define SUPPORT_BROADCAST YES // YES = commands to RT31 are valid
//  NO = commands to RT31 are invalid, ignored

/*
*	Macro for enabling/disabling RT1/RT2 undefined mode commands per MIL-STD-1553
*/
#define UNDEF_MCODES_VALID YES // YES = undefined & reserved mode commands are valid
//  NO = undefined & reserved mode commands are invalid, ignored

/*
* Macro for enabling/disabling RT1/RT2 simplified mode command processing
*/
#ifdef SMCP_OK
#define USE_SMCP YES     // YES = mode command results are stored in descriptor table 
												// itself, not in assigned RAM buffers
#else
#define USE_SMCP NO			//  NO = descriptor table is initialized to store mode command
												//  results in assigned RAM buffers
#endif
