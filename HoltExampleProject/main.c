/* ----------------------------------------------------------------------------
 *            HOLT INTEGRATED CIRCUITS Applications Engineering
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Holt Integrated Circuits
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Holt's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.

 * ==============================================================================
 * Version 2.0
 * This version support the HI-6130 and HI-6140.
 * Seven Configurations are provided to build customized versions of this project.
 * These defines were previously included in the 613x_initialization.h header file to allow
 * customized builds. These are not in the preprocessor so they can be set by the IAR tool depending
 * on the 

//#define  BC_ena	1 // Bus Controller
//#define RT1_ena	1 // Remote Terminal 1
//#define RT2_ena	1 // Remote Terminal 2         *--------------------------------------*
//#define SMT_ena	1 // Simple Monitor Terminal   *   DO NOT set SMT_ena = IMT_ena = 1.  *
//#define IMT_ena	0 // IRIG-106 Monitor Terminal *   One or both of these 2 must be 0.  *
                                                       *--------------------------------------*
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY HOLT "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL HOLT BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */


//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
// standard Atmel/IAR headers
#include <board.h>
#include <pio/pio.h>
#include <tc/tc.h>
#include <irq/irq.h>
#include <utility/trace.h>
#include <intrinsics.h>

// Holt project headers
#include "board_613x.h"
#include "613x_bc.h"
#include "613x_mt.h"
#include "613x_rt.h"
#include "613x_regs.h"
#include "613x_initialization.h"
               
#include "board_6130.h"
#include "device_6130.h"

#if(CONSOLE_IO)
#include "console.h"
#endif


//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------
// HI-6130 register structure base address
// you can highlight pH6130 below and drag it into a 
// Watch Window in IAR Embedded Workbench for ARM:
const H6130 pH6130 = HI6130_BASE;

#if(RT1_ena)
// you can highlight pRT1d or pRT1i below and drag into a 
// Watch Window in IAR Embedded Workbench for ARM:
const RT1d pRT1d = RT1_DTABLE_BASE;
const RT1i pRT1i = RT1_ITABLE_BASE;
#endif // (RT1_ena)

#if(RT2_ena)
// you can highlight pRT2d or pRT2i below and drag into a 
// Watch Window in IAR Embedded Workbench for ARM:
const RT2d pRT2d = RT2_DTABLE_BASE;
const RT2i pRT2i = RT2_ITABLE_BASE;
#endif // (RT2_ena)

#if(BC_ena)
// you can highlight pGPQ or pBCil below and drag into a 
// Watch Window in IAR Embedded Workbench for ARM:
GPQ pGPQ = (GPQ) GPQ_BASE_BUS_ADDR;
const BCil pBCil = BC_ILIST_BASE;
const BCstack pBCstack = BC_STACK_BASE; 
const BCstack2RT pBCstack2RT = BC_2RTSTACK_BASE; 
#endif // (BC_ena)

#if(SMT_ena)
DSTK pDSTK = (DSTK) DSTACK_BASE_BUS_ADDR;
const ScSTK pScSTK = SMT_CMDSTACK_BASE;
const MTF pMTF = MT_FTABLE_BASE;
const AddrList pAddrList = MT_ALIST_BASE;
#endif // (SMT_ena)

#if(IMT_ena)
const MTF pMTF = MT_FTABLE_BASE;
const AddrList pAddrList = MT_ALIST_BASE;
PktHdr pPktHdr;
IPMB pIPMB;
#endif // (IMT_ena)


//------------------------------------------------------------------------------
//         Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// Application entry point. ARM I/O and the Holt HI-613x device are configured,
/// then execution waits in an endless loop.

/// IAR intrinsic functions used in this program:
///         __disable_irq();
///         __enable_irq();
/// These require intrinsics.h header file
//------------------------------------------------------------------------------
void main(void) {
  
    unsigned short j, ttconfig = 0, runbits = 0;
    const Pin pinAUTOEN =  PIN_AUTOEN;
    const Pin pinCOPYREQ = PIN_CPYREQ;
    const Pin pinNMR = PIN_NMR;
    #if(RT1_ena||RT2_ena)
    const Pin pinNSW1 = PIN_NSW1;
    #endif
    
    __disable_interrupt(); // until initialization is complete

    // first priority: reset pin to 0 for HI-613x nMR, then configure that MCU pin as output
    AT91C_BASE_PIOC->PIO_CODR = nMR;  
    PIO_Configure(&pinNMR,1);
          
    // Configure ARM's other general purpose I/O pins and timer(s) 
    ConfigureGpio();
    init_timer();
    // enable the MCU nRST external reset
    AT91C_BASE_RSTC->RSTC_RMR= 0xA5000F01; 
    
    // Initialize processor for selected interface to HI-613X device,
    Configure_ARM_MCU_ExtBus();

        
    #if (CONSOLE_IO)
        ConfigureUsart1();
        printf("\n\n\n\n\n\n\r Holt Integrated Circuits HI-6130_40 Project \n\r");
        printf(" Ver: 2.0     Compiled: %s %s    ", __DATE__, __TIME__ );
    #endif

    // Anywhere from 0 to 4 terminals might be enabled by the 4 DIP switches on
    // the evaluation board. Next function call checks states for BC, RT1, RT2,
    // and MT DIP switches. For each switch = 1 (terminal enabled) the function
    // verifies that the corresponding compiler run-time switch is also ON. For
    // example: if BCENA DIP switch = 1 then BC_ENA (in file 613x_initialization.h)
    // must be defined ON (non-zero). Error trap occurs (flashing the red LED) if:
    //
    //  * one or more terminals has switch = 1 but compiler run-time switch defined OFF 
    //  * one or more terminals has switch = 0 but compiler run-time switch defined ON
    //  * all 4 DIP switches = 0, nothing is enabled
    // 
    // If error-free, function writes terminal enable bits into the Master Configuration 
    // register, then returns a 16-bit word having a run bit set for each enabled
    // DIP switch, used later to start terminal operation after initialization.
        
    // program writes these bits to the Master Configuration register later...
    runbits = enable_check();
        
    // Next function call uses an in-line delay. This affects completion time for hard reset 
    // (ARM microprocessor power-up reset (POR), or assertion of the board's RESET button. 
    // This delay does not affect time for "HI-613x only" hard reset, or HI-613x soft reset.

    //  flash an LED to indicate how initialization will proceed: 
    if(PIO_Get(&pinAUTOEN)){
       // AUTOEN == 1 (self init from EEPROM)
       #if (CONSOLE_IO)
          printf("\n\n\r       Auto-Initializing from EEPROM  \n\n\r");
       #endif
       Flash_Red_LED();
    }    
    else {   
       // AUTOEN == 0 (host initialization)
       #if (CONSOLE_IO)
          printf("\n\n\r       Host is Initializing Regs & RAM \n\n\r");
       #endif
       Flash_Green_LED();
    }
        
    // Next actions are determined by states of the AUTOEN (auto-initialize) 
    // DIP switch and EECOPY Request DIP switch (labeled "COPYRQ"). AUTOEN is a 
    // HI-613x input, COPYRQ is a microprocessor input. The two switches COPYRQ and 
    // AUTOEN should never both be high (There is no point in auto-initializing the
    // HI-613x from EEPROM then copying the just-initialized setup to EEPROM again!) 

    //           D I P    S W I T C H    S E T T I N G    T A B L E
    //  ------------------------------------------------------------------------		
    //            |      AUTOEN = 0         |          AUTOEN = 1
    //  ----------|-------------------------|-----------------------------------
    //    COPYRQ  | Host initializes 613x   |   EEPROM initializes 613x
    //     = 0    | No EEPROM involvement   |   No Host involvement
    //            | Normal host initialize  |   Normal auto-initialize sequence
    //  ----------|-------------------------|-----------------------------------
    //    COPYRQ  | Host initializes 613x   |   EEPROM initializes 613x
    //      = 1   | then EECOPY to EEPROM   |   then EECOPY to EEPROM 
    //            | (Normal EEPROM copy)    |   (Senseless sequence)
    //	------------------------------------------------------------------------
		
    // Reset HI-613x. If device AUTOEN pin is high, it will auto-initialize
    // the device registers and RAM from serial EEPROM. In this case, execution
    // comes back here only after the HI-613x READY output goes high...
    reset_613x();

    // If error-free auto-init occurred after master reset, next function call 
    // returns "0". if auto-initialize occurred with errors, the function call 
    // does NOT return (local error trap). If auto-initialize was not enabled,
    // this program initializes device RAM and registers below.
	
    // Although possible to do so, "intialize from EEPROM" process in this program 
    // does not automatically start execution for terminal devices. Instead,
    // whether auto-init or host init, the program simulaneously starts execution for 
    // all enabled devices (BC,MT,RT1,RT2) when "runbits" are written into the Master
    // Configuration register below...          
               
    if(autoinit_check()) {

        // if function call above returned with non-zero result, the terminal was 
        // NOT automatically initialized after reset, BUT numerous registers now
        // contain non-zero default values. The host will now initialize...
      
        // having met enable_check() requirements above, write the terminal 
        // enable bits for BC or RT(s) into the Master Config register 0. (The
        // MT has only a run bit, not an enable bit.)
        pH6130->MASTER_CONFIG_REG = runbits & (BCENA|RT1ENA|RT2ENA);

        
        // modify runbits to just contain "start execution" bits, to be written last
        runbits &= ~(BCENA|RT1ENA|RT2ENA);
            
        // Select common configuration options that apply to all BC, MT, RT1, RT2. 
        initialize_613x_shared();
        
        // Terminal-specific options are initialized next... 
        // Below, variable "ttconfig" is used for time tag counter configuration. 
	// If enabled, the MT has a dedicated 16-bit or 48-bit time counter.
	// MT time tag resolution is selected by MT Configuration Reg, bit 0. 
	// We select the MT counter clock source below, factored into "ttconfig".
		
	// The BC,RT1 and RT2 have independent time tag counters, but share 
	// a common clock source, selected below, factored into "ttconfig". The RT1 
	// and RT2 time counters are always 16-bit. The BC time counter is
	// either 16- or 32-bit, selected below.        
          
        //------------------------------------------------------------------------
        
        #if(BC_ena)

	    initialize_613x_BC();
            initialize_bc_msg_blocks();
	    initialize_bc_instruction_list();

            // select BC time tag resolution, either 16-bit or 32-bit (BTTAG16 or BTTAG32)
            
            #ifdef BC_TTAG_HI_RES
                ttconfig |= BTTAG32;
            #else
                ttconfig |= BTTAG16;
            #endif

            // Select internally-generated time tag clock used by BC,RT1 and RT2
            // Choices are 2U,4U,8U,16U,32U or 64U (microseconds)
            ttconfig |= TTAG_64U;

        #endif	// end BC init. 

        //------------------------------------------------------------------------

        #if(RT1_ena)
	    // verify RT1 Op Status Reg shows a valid RT address. This function call
	    // does not return if register reflects address parity error or if the
	    // register reads back zero value, an invalid result (local error trap)
            
	    if (RTAddr_okay(1)) {
           
                // host now initializes HI-613x regs, RAM tables, for RT1 
                initialize_613x_RT1();				
                
	        // write test data to assigned transmit buffers
	        write_dummy_tx_data_RT1();
                
	        // RT1 and RT2 always use 16-bit time tag resolution.
                // if not already selected above for BC, (i.e. if BC is not used), 
                // select RT1 and RT2 time tag clock which otherwise is shared with BC.
                // Choices are 2U,4U,8U,16U,32U or 64U (microseconds)
	        if(!ttconfig) ttconfig = TTAG_64U;
            }			
         
        #endif    // end RT1 init. RT1 does not start until the RT1STEX register bit is set

        //------------------------------------------------------------------------

        #if(RT2_ena)

	    // verify RT2 Op Status Reg shows a valid RT address. This function call
	    // does not return if register reflects address parity error or if the
	    // register reads back zero value, an invalid result (local error trap)
	    if (RTAddr_okay(2)) {
           
                // host now initializes HI-613x regs, RAM tables, for RT2
                initialize_613x_RT2();				

	        // write test data to assigned transmit buffers
	        write_dummy_tx_data_RT2();
				
	        // RT1 and RT2 always use 16-bit time tag resolution.
	        // if not already selected above for BC or RT1, (i.e. if BC & RT1 not used), 
                // select RT2 time tag clock which otherwise is shared with BC and RT1.
                // Choices are 2U,4U,8U,16U,32U or 64U (microseconds)
	        if(!ttconfig) ttconfig = TTAG_64U;
           }			
         
        #endif    // end RT2 init. RT2 does not start until the RT2STEX register bit is set

        //------------------------------------------------------------------------
     
        #if(SMT_ena||IMT_ena)

            // Host now initializes Simple Monitor or IRIG-106 Bus Monitor, as selected in 
            // file 613x_initialization.h. Next call traps error if MTRUN pin disables MT.
            initialize_613x_MT();				

            // dedicated MT time tag clock selection:

            #if(IMT_ena) 
                // IRIG-106 monitor (IMT) always uses 48-bit time tag resolution.
                // two IRIG-106 monitor time tag clock options: MTTAG_OFF or MTTAG_100N (100ns)
                // "OFF" is helpful in debug, to prevent "maximum packet time" end-of-packet.
                j = MTTAG_OFF; // MTTAG_100N;
                ttconfig |= j;

            #else 
	        // SMT simple monitor time tag clock options: MTTAG_OFF, MTTAG_100N (100ns)
                // or MTTAG_2U,4U,8U,16U,32U or 64U (microseconds)
      	        ttconfig |= MTTAG_64U;
                
                // SMT can use 16-bit or 48-bit time tag resolution, selected in
                // the MT Configuration register...

                #ifdef SMT_TTAG_HI_RES
                    j = SMT_TTAG48;
                #else
                    j= SMT_TTAG16;
                #endif

                // read-modify-write the MT Configuration Register
                pH6130->MT_CONFIG_REG |= j;              
                
            #endif
						
        #endif	// end MT init. Monitor does not start until MTENA register bit is set below
         
        //------------------------------------------------------------------------
        
        // write the Time Tag Configuration Register
        pH6130->TTAG_CONFIG_REG = ttconfig;              
  
	if (PIO_Get(&pinCOPYREQ)) {
            // "COPY REQUEST" DIP switch is high. Write serial EEPROM using parameter 0 
	    // so the various start bits in the Master Configuraton Register are 
            // NOT automatically set by subsequent auto-initialization cycles.
           #if (CONSOLE_IO)
                printf("       Copying Registers & RAM to EEPROM  \n\r");                       
                printf("    Be sure to turn off COPYREQ DIP switch. \n\n\r");                       
            #endif
	    write_init_eeprom();
        }
          
    }   // end:if (autoinit_check())
            
    // flash both green bus LEDs to signify initialization complete
    AT91C_BASE_PIOC->PIO_CODR = nLEDA|nLEDB;  // LEDs ON
    Delay_x100ms(3);
    AT91C_BASE_PIOC->PIO_SODR = nLEDA|nLEDB;  // LEDs OFF
        
    // we disabled interrupts during initialization, 
    // now enable them before starting terminal execution
    __enable_interrupt();
    
    // write the Master Configuraton Register to start the
    // terminal(s) initialized above, to begin operation.
    // do not overwrite previously initialized common features       
	pH6130->MASTER_CONFIG_REG |= runbits;         
      
      #if (CONSOLE_IO)
          show_menu();
          // Infinite loop
          while (1) {
              // poll USART1 to detect and act on console key input at computer keyboard...
              chk_key_input();
              
              #if(RT1_ena||RT2_ena)
                  // if MCU board SW1 button is pressed, update RT1 and RT2 status bits
                  // based on Terminal Flag and Busy DIP switch settings
                  if(!PIO_Get(&pinNSW1)) modify_RT_status_bits();
              #endif

          }   // end while 
          
          
      #else // not using console IO...
          // Infinite loop
          while (1) {
              #if(BC_ena)
                  bc_switch_tests();
              #endif // BC_ena
                  
              #if(RT1_ena||RT2_ena)
                  // if MCU board SW1 button is pressed, update RT1 and RT2 status bits
                  // based on Terminal Flag and Busy DIP switch settings
                  if(!PIO_Get(&pinNSW1)) modify_RT_status_bits();
              #endif
                  
          }   // end while

      #endif
   
}     // end main()


// end of file

