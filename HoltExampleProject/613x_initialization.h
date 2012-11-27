/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	613x_initialization.h
 *    brief     This file contains prototype functions and macro definitions
 * 		used by routines in 613x_initialization.c file, and elsewhere
 *
 *	   	HOLT DISCLAIMER
 *      	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
 *      	KIND,EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 *      	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 *      	PURPOSE AND NONINFRINGEMENT. 
 *      	IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *      	OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *      	OTHERWISE,ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *
 *              Copyright (C) 2009-2011 by  HOLT, Inc.
 *              All Rights Reserved
 */

//------------------------------------------------------------------------------
//  Macro definitions 
//------------------------------------------------------------------------------


//    brief	Macro for selecting parallel bus interface for connection with HI-6130,
//		or host SPI interface for connection to HI-6131
//              If HOST_BUS_INTERFACE is defined as NO or OFF, SPI interface is used 
//



//    brief	Macros for enabling or disabling HI-613x terminal devices when building
//		the C project. Corresponding bit is "1" for enabled devices, or "0" for
//		disabled terminal devices 
//     NOTE:    ENABLED FUNCTIONS MUST HAVE CORRESPONDNG DEVICE ENABLE PINS CONNECTED HIGH
//              (CONTROLLED BY 4 DIP SWITCHES), OTHERWISE PROGRAM EXECUTION GOES TO ERROR TRAP



/*  These defines are set in the project C/C++ options preprocessor configuration menu
    for each Configuraton.*/

#define HOST_BUS_INTERFACE YES // YES = HI-6130 host bus interface
	    			 //  NO = HI-6131 host SPI interface

#define  BC_ena	1 // Bus Controller
#define RT1_ena	1 // Remote Terminal 1
#define RT2_ena	1 // Remote Terminal 2    
#define SMT_ena	1 // Simple Monitor Terminal  
#define IMT_ena	0 // IRIG-106 Monitor Terminal
                                                    



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

#define BC_autostart		0
#define RT1_autostart		0
#define RT2_autostart		0
#define SMT_IMT_autostart	0



//    brief	Macros that select time tag counter resolution for bus controller (16- or 32-bits)
//              and SMT simple monitor terminal (16- or 48-bits). IMT always uses 48-bit time tag.
//
#define BC_TTAG_HI_RES  // high resolution is 32-bit time tag. Comment-out line for low res 16-bit count.
#define SMT_TTAG_HI_RES // high resolution is 48-bit time tag. Comment-out line for low res 16-bit count.



//    brief	Macro for enabling/disabling console i/o. 
//              YES for enhanced feedback while debugging, 
//              NO to reduce compiled program size
//
#define CONSOLE_IO  YES  	// YES = enable
				//  NO = disable

//    brief	Macro for enabling/disabling RT1/RT2 "illegal command detection" per MIL-STD-1553
//
#define ILLEGAL_CMD_DETECT  YES // YES = HI-6130 host bus interface
				//  NO = "in form" response for all valid commands


//    brief	Macro for enabling/disabling RT1/RT2 broadcast commands per MIL-STD-1553
//
#define SUPPORT_BROADCAST   YES // YES = commands to RT31 are valid
				//  NO = commands to RT31 are invalid, ignored


//    brief	Macro for enabling/disabling RT1/RT2 undefined mode commands per MIL-STD-1553
//
#define UNDEF_MCODES_VALID  YES // YES = undefined & reserved mode commands are valid
				//  NO = undefined & reserved mode commands are invalid, ignored


//    brief	Macro for enabling/disabling RT1/RT2 simplified mode command processing
//
#define USE_SMCP	YES     // YES = mode command results are stored in descriptor table 
				//       itself, not in assigned RAM buffers
				//  NO = descriptor table is initialized to store mode command
				//       results in assigned RAM buffers

//    brief	Macro for enabling/disabling irq-driven bus activity LEDs based on ACTIVE signal
//
#define BUS_ACTIVITY_LEDS  YES	// YES = enables falling edge IRQ when ACTIVE subsides
				//		 at end of valid message. Bus activity LEDs are
				//		 flashed by the IRQ. For messages generating a
				//		 INTREQ message IRQ, that IRQ is serviced 
				//		 first, then the ACTIVITY IRQ. The service routine 
				//		 for this ACTIVITY irq takes about 40us. 
                                //  NO = Bus activity LEDs are disabled.




//    brief	misc macro list
//
#define BUS_A	0
#define BUS_B	1
#define redLED	2
#define YES     1
#define NO      0

//------------------------------------------------------------------------------
//      Global Function Prototypes
//------------------------------------------------------------------------------

// End of File


