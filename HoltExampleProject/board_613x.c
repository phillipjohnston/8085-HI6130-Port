/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	board_613x.c
 *    brief     This file contains functions applicable to all terminal modes
 *              (BC, RT, SMT, IMT) in both Holt HI-6130 and HI-6131 projects.  
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
 *              SPI interface, files "device_6130.h" and "device_6131.h" 
 *              contain the definitions needed for register addressing.
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


// standard Atmel/IAR headers
#include <pio/pio.h>
#include <pmc/pmc.h>
#include <intrinsics.h>

// Holt project headers
#include "613x_initialization.h"
#include "board_613x.h"
#include "613x_regs.h"
                   
#include "device_6130.h"
#include "board_6130.h"
extern const H6130 pH6130;

#if(CONSOLE_IO)
#include <stdio.h>
#include "console.h"
#endif


//------------------------------------------------------------------------------
/// Configure the PIO pins not used by ARM on-chip peripherals (USART, EBI, SPI).
/// This configures output pins used for HI-6130 control, LEDs, etc. and input
/// pins used for HI-6130 status, interrupts, DIP switches, pushbuttons, etc.
//------------------------------------------------------------------------------
void ConfigureGpio(void)
{
       const Pin pins[] = {
       PINS_BUTTON, 
       PINS_DIPSW,
       PINS_6130STATUS,
       PINS_LED,
       PINS_6130CONTROL 
     };
     
    PIO_Configure(pins, PIO_LISTSIZE(pins));
}

//------------------------------------------------------------------------------
// This function generates an active low reset pulse for the HI-613x 
//------------------------------------------------------------------------------
// Return occurs when HI-613x READY output is asserted. If AUTOEN = 0, this 
// occurs after chip init and RAM test. If AUTOEN = 1, auto-initialization from
// the serial EEPROM adds additional time before READY assertion...
void reset_613x(void) {
  
    const Pin pinREADY = PIN_READY;  
    char i = 1;
    
    // active low pulse
    AT91C_BASE_PIOC->PIO_CODR = nMR;  
    while (i--);       
    AT91C_BASE_PIOC->PIO_SODR = nMR;  
    
    // wait for HI-613x READY assertion, then return
    while (!PIO_Get(&pinREADY));		
}

//------------------------------------------------------------------------------
// This function determines whether auto-initialization occurred to load the
// HI-613x registers and RAM from serial EEPROM after Master Reset.
//------------------------------------------------------------------------------
//   *  If YES (auto-init occurred after reset) several error checks are performed. 
//      If errors are found, no return to the calling function (local error trapping). 
//      If error-free, function returns "0" to calling function.
//   *  If NO (auto-init was not performed) function returns "1" to calling function.
//
//   no return (error trap) if auto-init was performed with errors
//   otherwise returns a character:
//	1 (no auto-init occurred after /MR) 
//	0 (error-free auto-init was performed)
//
char autoinit_check(void) {

    const Pin pinAUTOEN = PIN_AUTOEN;
    unsigned short int read_value;

    // did we auto-initialize after master reset? (check for AUTOEN pin assertion)
    if (!PIO_Get(&pinAUTOEN)) {
        // NO, auto-initialize DID NOT occur after master reset
	return 1;
    }

    else {
	// YES, auto-initialize DID occur after master reset
	// check the Hardware Pending Int register for auto-initialization errors

	read_value = pH6130->HDW_PENDING_INT_REG;

            
	// EEPROM checksum error? (EECKE bit high)
	if (read_value & EECKE_INT) {

	    // yes, read EEPROM checksum (2's comp + 1) from register address 0x004E, which
            // is also used for the serial EEPROM unlock function.
	    // The value fetched is not used by this program, simply fetched for convenience.
			 
	    read_value = pH6130->EEPROM_UNLOCK_REG;


            // error trap during development: 
	    // a repeating 4-blink red LED means EEPROM Checksum Mismatch
            error_trap(4);
        }   // end: EEPROM checksum error

	// data mismatch error? (data written to device reads back differently)
	if (read_value & RAMIF_INT) {

	    // yes, fetch address of first-occurring EEPROM reg/RAM mismatch from reg 0x0024.
            // The primary funcion of this register is the RT2 Message Info Word Address.
	    // The value fetched is not used by this program, simply fetched for convenience.


		read_value = pH6130->RT2_MSG_INFO_WD_ADDR_REG;

            // error trap during development: repeating 3-blink red LED 
            // indicates data mismatch error 
            error_trap(3);
        }   // end: data mismatch error

        #if(RT1_ena || RT2_ena)
            // RT address parity error for either RT1 or RT2 Pending
            // interrupt is only generated if RT is enabled, i.e., pin RTxENA = 1
	    if (read_value & (RT1APF|RT2APF)) {
                // delay 800ms
		Delay_x100ms(8);
		// error trap during development: repeating 2-blink red LED
		// indicates RT address parity error
		error_trap(2);
            }   // end: RT1 or RT2 address parity error
	
          #endif

	  return 0;
        
    }	// end: auto-initialization DID occur after master reset
}	// end: autoinit_check()


//----------------------------------------------------------------------------------------
// This function initializes parameters	shared by HI-613x BC, RT1, RT2 and/or Bus Monitor.
//----------------------------------------------------------------------------------------
// the initialized parameters are time tag setup, hardware interrupts, etc
//     THIS FUNCTION SHOULD BE CALLED BEFORE CALLING 
//     THE BC, RT1, RT2 AND/OR BUS MONITOR INIT FUNCTIONS

void initialize_613x_shared(void) {

		// Select common configuration options that apply to all
		// BC,MT,RT1,RT2. Terminal-specific options are initialized 
		// elsewhere. The following options may be selected here:
                //   TXINHA = Tx inhibit bus A
                //   TXINHB = Tx inhibit bus B
                //   SSR8 = single strobe read if 8-bit wide bus 
                //   (PULSE_INT or LEVEL_INT) select interrupt type
  
                pH6130->MASTER_CONFIG_REG |= PULSE_INT; // |TXINHA|TXINHB;

		// enable these hardware interrupts: 
                // EEPROM checksum error
                // auto-init RAM init fail (mismatches EEPROM)
                // bus A and bus B loopback failures
	        pH6130->HDW_INT_ENABLE_REG = EECKE_INT|RAMIF_INT|LBFA|LBFB;           
}	// end: initialize_613x_shared()


//------------------------------------------------------------------------------
// This function initializes Timer Counter 0 (TC0) for use in delay calls.
//------------------------------------------------------------------------------
void init_timer(void) {
    
    unsigned int i;
    
    // Enable TC0 peripheral clock
    PMC_EnablePeripheral(AT91C_ID_TC0);

    // INITIALIZE TIMER-COUNTER 0 FOR POLLED RC COMPARE OPERATION
    // ----------------------------------------------------------
    // Disable timer-counter clock and interrupts
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;
    AT91C_BASE_TC0->TC_IDR = 0xFFFFFFFF;
    
    // Clear timer0 status register by reading it
    i = AT91C_BASE_TC0->TC_SR;
    // suppress compiler warning: variable "i" was set, never used
    i = i;
    
    // Set Timer Counter mode. In TC_CMR, set the WAVE bit for waveform
    // operating mode and set the CPCSTOP bit so Register C match stops the
    // timer clock. The AT91C_TC_CLKS_TIMER_DIVx_CLOCK field selects clock 
    // prescaler (MCLK div-by-N).  Other bits are reset...
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_WAVE | AT91C_TC_CPCSTOP 
                                           | AT91C_TC_CLKS_TIMER_DIV1_CLOCK; // for MCLK/2
    //                                     | AT91C_TC_CLKS_TIMER_DIV2_CLOCK; // for MCLK/8
    //                                     | AT91C_TC_CLKS_TIMER_DIV3_CLOCK; // for MCLK/32
    //                                     | AT91C_TC_CLKS_TIMER_DIV4_CLOCK; // for MCLK/128
    
    // Enable TC clock. Timer-counter will not start counting until triggered
    // outside this function
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN;
    
    // No AIC interrupt initialization used for timer0, its status reg is polled.
    //
    // To modify to use a timer0 "RC compare" interrupt, add these steps:
    //IRQ_ConfigureIT(AT91C_ID_TC0, 0, TC0_IrqHandler); // identify int svc function
    //AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    //IRQ_EnableIT(AT91C_ID_TC0);

}   // end init_timer()

/*
//----------------------------------------------------------------------------
//  Function Name       : start_timer0(delay_count)
//  Object              : load register C with compare value then start timer0 
//  Input Parameters    : delay_count
//  Output Parameters   : none
// ----------------------------------------------------------------------------
// 
//  This function clears and starts timer0, then returns to calling program. Once
//  started, timer0 will stop counting and set the timer0 CPCS status flag when
//  count = <Register C>. Because the CPCS flag is polled by the calling program,
//  the timer0 interrrupt is not used.
// 
//  IMPORTANT valid range for delay_count: delay_count < 65,536 (decimal), 0x10000
//  See function "Delay_us()" below, for example using start_timer0()  

void start_timer0(unsigned short int delay_count) {
    
    unsigned short int i;
    // Operating mode already loaded for timer0
    // timer0 status is polled so we do not use interrupt
    
    // Load delay_count into the TC_RC register
    AT91C_BASE_TC0->TC_RC = delay_count;
    // Read timer0 status register to reset its CPCS flag
    i = AT91C_BASE_TC0->TC_SR;
    // Suppress warning: variable "i" was set, never used
    i = i;    
    // Start timer0: Clock Enable and software trigger, which clears the timer 
    // and up-count begins. Counting stops when Register C match occurs.
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
} 
*/

// --------------------------------------------------------------------------------------
//  Function Name       : Delay_us()
//  Object              : in-line delay
//  Input Parameters    : num_us = integer number of microseconds <  2731 
//  Output Parameters   : none
// ---------------------------------------------------------------------------------------
void Delay_us(unsigned short int num_us) {

    const Pin pinLEDY = PIN_LEDY;
    unsigned short i;
    
//  Previous call for function "init_timer()" is assumed.
//  Use timer0 to generate an n_uS in-line delay. Can verify delay by pulse on LED4.
//  In this function, timer0 status is polled, okay for in-line delays.
//  While less overhead than using a vectored-interrupt delay, the pulse
//  generated is slightly longer than the set value because we poll timer0 status...
//    
//  TC0 Channel Mode Register prescaler selected, 48MHz    Load #     Max t (int)
//  ----------------------------------------------------   -------    -----------
//  AT91C_TC_CLKS_TIMER_DIV1_CLOCK for MCLK/2     48/2     24/1us     2,730us     <--- used
//  AT91C_TC_CLKS_TIMER_DIV2_CLOCK for MCLK/8     48/8     6/1us      10,922us
//  AT91C_TC_CLKS_TIMER_DIV3_CLOCK for MCLK/32    48K/32   1500/1ms   43ms
//  AT91C_TC_CLKS_TIMER_DIV4_CLOCK for MCLK/128   48K/128  375/1ms    174ms
   
    // using 48MHz MCLK and div-by-2 prescaler...
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_WAVE | AT91C_TC_CPCSTOP 
                                           | AT91C_TC_CLKS_TIMER_DIV1_CLOCK; // for MCLK/2

    // Load delay_count into the TC_RC register
    AT91C_BASE_TC0->TC_RC = num_us * (BOARD_MCK / 2000000);
    
    // Read timer0 status register to reset its CPCS flag
    i = AT91C_BASE_TC0->TC_SR;
    // Suppress warning: variable "i" was set, never used
    i = i;    
    // Start timer0: Clock Enable and software trigger, which clears the timer 
    // and up-count begins. Counting stops when Register C match occurs.
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
    
   // Turn ON LED, logic-0 
    PIO_Clear(&pinLEDY);
    
    // poll timer0 register C compare status until 0 -> 1 transition
    while (!(AT91C_BASE_TC0->TC_SR & AT91C_TC_CPCS)) ;
    
    // time-out occurred, turn OFF LED, logic-01
    PIO_Set(&pinLEDY);

}


// --------------------------------------------------------------------------------------
//  Function Name       : Delay_ms()
//  Object              : in-line delay
//  Input Parameters    : num_us = integer number of milliseconds < 175
//  Output Parameters   : none
// ---------------------------------------------------------------------------------------
void Delay_ms(unsigned short int num_ms) {

    unsigned short i;
    
//  Use timer0 to generate an n_uS in-line delay. Can verify delay by pulse on LED4.
//  In this example timer0 status is polled, okay for in-line delays.
//  While less overhead than using a vectored-interrupt delay (below), the pulse
//  generated is slightly longer than the set value because we poll timer0 status...
//    
//  TC0 Channel Mode Register prescaler selected, 48MHz    Load #     Max t (int)
//  ----------------------------------------------------   -------    -----------
//  AT91C_TC_CLKS_TIMER_DIV1_CLOCK for MCLK/2     48/2     24/1us     2,730us
//  AT91C_TC_CLKS_TIMER_DIV2_CLOCK for MCLK/8     48/8     6/1us      10,922us
//  AT91C_TC_CLKS_TIMER_DIV3_CLOCK for MCLK/32    48K/32   1500/1ms   43ms
//  AT91C_TC_CLKS_TIMER_DIV4_CLOCK for MCLK/128   48K/128  375/1ms    174ms     <--- used
   
    // using 48MHz MCLK and div-by-2 prescaler...
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_WAVE | AT91C_TC_CPCSTOP 
                                           | AT91C_TC_CLKS_TIMER_DIV4_CLOCK; // for MCLK/128

    // Load delay_count into the TC_RC register
    AT91C_BASE_TC0->TC_RC = num_ms * (BOARD_MCK / 128000);	    
    
    // Read timer0 status register to reset its CPCS flag
    i = AT91C_BASE_TC0->TC_SR;
    // Suppress warning: variable "i" was set, never used
    i = i;    
    // Start timer0: Clock Enable and software trigger, which clears the timer 
    // and up-count begins. Counting stops when Register C match occurs.
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
    
   // Turn ON yellow LED, logic-0, measure pulse time to check delay time 
   // AT91C_BASE_PIOA->PIO_CODR = nLEDY; 
    
    // poll timer0 register C compare status until 0 -> 1 transition
    while (!(AT91C_BASE_TC0->TC_SR & AT91C_TC_CPCS)) ;
    
    // time-out occurred, turn OFF yellow LED, logic-01
    AT91C_BASE_PIOA->PIO_SODR = nLEDY; 

}

// ---------------------------------------------------------------------------------------
// multiple 100ms in-line delay
// ---------------------------------------------------------------------------------------
void Delay_x100ms(char num) {  
  for(; 0 < num ; num--) {
      Delay_ms(100);
  }
}

// ---------------------------------------------------------------------------------------
// flash red LED for 400ms
// ---------------------------------------------------------------------------------------
void Flash_Red_LED(void) {
    AT91C_BASE_PIOA->PIO_CODR = nLEDR; 
    Delay_x100ms(4);
    AT91C_BASE_PIOA->PIO_SODR = nLEDR; 
}


// ---------------------------------------------------------------------------------------
// flash green LED for 400ms
// ---------------------------------------------------------------------------------------
void Flash_Green_LED(void) {
    AT91C_BASE_PIOA->PIO_CODR = nLEDG; 
    Delay_x100ms(4);
    AT91C_BASE_PIOA->PIO_SODR = nLEDG; 
}



// ---------------------------------------------------------------------------------------
//\ brief	Next function flashes red LED forever:
//		groups of N flashes where N = 2-7, else N = 1 
//		No return to calling function.
//
//		# flashes 	Error type
//		2		Op Status register RT Address parity error
//		3		auto-init data mismatch error
//		4		auto-init EEPROM checksum failure
//		5		Op Status register reads back zero, something broken
//		6		EECOPY failure: for example an invalid unlock code
//              7               Terminal(s) enabled in pgm have negated enable pin(s)
// ---------------------------------------------------------------------------------------
void error_trap(char count) {

    const Pin pinLEDR = PIN_LEDR;
    int i;

    // brief delay to separate from auto-init red LED blink after reset
    Delay_x100ms(12);
	
    if ((count > 7)||(count < 2)) {
        count = 1;
    }
    
    while(1) {
        // perpetually blink red LED in groups of N flashes
	for (i = 0; i < count; i++) {
	    PIO_Set(&pinLEDR);
	    Delay_x100ms(2);  
	    PIO_Clear(&pinLEDR);
	    Delay_x100ms(2);
        }
        // 1.2 sec delay
        Delay_x100ms(12);
    }
}



//--------------------------------------------------------------------------------
// This function checks states for BC, RT1, RT2, and MT DIP switches. For each 
// DIP switch = 1 (terminal enabled) the function verifies that the corresponding 
// compiler run-time switch is also ON. For example: if BCENA DIP switch = 1 
// then BC_ENA (in file 613x_initialization.h) must be defined ON (non-zero). 
// If one or more switch-enabled terminals has a compiler run-time switch 
// defined OFF (or if all 4 DIP switches = 0) an error trap occurs, perpetually
// flashing the red LED (no return). If error-free, function returns a 16-bit word 
// with two "run bits" set for each enabled DIP switch, except MT has 1 bit.
//---------------------------------------------------------------------------------
unsigned short enable_check(void) {
  short int runbits = 0;
    const Pin pinBCENA = PIN_BCENA;
    const Pin pinMTRUN = PIN_MTRUN;
    const Pin pinRT1ENA = PIN_RT1ENA;
    const Pin pinRT2ENA = PIN_RT2ENA;
        
    if(PIO_Get(&pinBCENA)) {
        // BCENA pin high, trap error if compiler runtime switch BC_ena is FALSE
        #if(!BC_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: BCENA pin high, but software BC_ena false.\n\r");
            #endif
            error_trap(3);
        #endif
        runbits |= BCENA|BCSTRT;
    }
    else {
        // BCENA pin low, trap error if compiler runtime switch BC_ena is TRUE
        #if(BC_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: BCENA pin low, but software BC_ena true.\n\r");
            #endif
            error_trap(3);
        #endif 
    }      
    if(PIO_Get(&pinRT1ENA)) {
        // RT1ENA pin high, trap error if compiler runtime switch RT1_ena is FALSE
        #if(!RT1_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: RT1ENA pin high, but software RT1_ena false.\n\r");
            #endif 
            error_trap(3);
        #endif
        runbits |= RT1ENA|RT1STEX;
    }
    else {
        // RT1ENA pin low, trap error if compiler runtime switch RT1_ena is TRUE
        #if(RT1_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: RT1ENA pin low, but software RT1_ena true.\n\r");
            #endif 
            error_trap(3);
        #endif 
    }      
    if(PIO_Get(&pinRT2ENA)) {
        // RT2ENA pin high, trap error if compiler runtime switch RT2_ena is FALSE
        #if(!RT2_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: RT2ENA pin high, but software RT2_ena false.\n\r");
            #endif 
            error_trap(3);
        #endif
        runbits |= RT2ENA|RT2STEX;
    }
    else {
        // RT2ENA pin low, trap error if compiler runtime switch RT2_ena is TRUE
        #if(RT2_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: RT2ENA pin low, but software RT2_ena true.\n\r");
            #endif 
            error_trap(3);
        #endif 
    }      
    if(PIO_Get(&pinMTRUN)) {
        // MTENA pin high, trap error if compiler runtime 
        // switches SMT_ena and IMT_ena are both FALSE
        #if(!SMT_ena && !IMT_ena) 
            #if(CONSOLE_IO)
                printf("\n\r Error: MTRUN pin high, but software SMT_ena & IMT_ena both false.\n\r");
            #endif 
           error_trap(3);
        #endif
        // monitor does not have separate enable and run bits
        runbits |= MTENA;
    }
    else {
        // MTRUN pin low, trap error if either compiler 
        // runtime switch SMT_ena or IMT_ena is TRUE
        #if(SMT_ena || IMT_ena)
            #if(CONSOLE_IO)
                printf("\n\r Error: MTRUN pin low, but software SMT_ena or IMT_ena is true.\n\r");
            #endif 
            error_trap(3);
        #endif 
    }      
    // trap error if nothing is enabled
    if(!runbits) {
        #if(CONSOLE_IO)
            printf("\n\r Error: BC, RT1, RT2, IMT, SMT are all disabled.\n\r");
        #endif 
        error_trap(2);
    }
    
    // write enable bits into the Master Configuration register
    //pH6130->MASTER_CONFIG_REG = runbits[0];
    
    return runbits;
}    

//----------------------------------------------------------------------
// This function copies registers and RAM to the serial EEPROM
// subsequently used for auto-initialization after master reset. 
//
// IMPORTANT: Make sure interrupts are disabled during execution !!!
//----------------------------------------------------------------------
void write_init_eeprom(void) {
  
        const Pin pinLEDY = PIN_LEDY;
        const Pin pinREADY = PIN_READY;
        const Pin pinEECOPY = PIN_EECOPY;
	short unsigned int code1 = 0xA00A, code2 = 0x5FF5;

	// IMPORTANT: make sure interrupts are disabled !!! 
	
	// The default code1/code2 EEPROM unlock values do not start
	// any of the 4 terminal devices when auto-init is complete, 
	// following future master reset with the AUTOEN pin asserted.
	// To start an enabled terminal, the host must then set BCSTRT, 
        // MTRUN, RT1STEX or RT2STEX in Master Configuration register 0
        // after READY assertion, following auto-initialization.
        //
	// If the EEPROM is written with unlock codes modified below,
	// selected terminal devices can be auto-enabled without host assist.
	// To modify these settings, edit file 613x_initialization.h 

	if (( BC_ena == 1) && ( BC_autostart ==1)) {	
		// IC automatically asserts BCSTRT bit 13 in reg 0, after 
		// auto-initialization. The copied EEPROM image should already 
		// have BCENA bit 12 asserted in reg 0, but not BCSTRT bit 13.
		code1 ^= 0x0C00;
		code2 ^= 0x0C00;
	} 
	if (( RT1_ena == 1) && ( RT1_autostart ==1)) {	
		// IC automatically asserts RT1STEX bit 4 in reg 0, after 
		// auto-initialization. The copied EEPROM image should already 
		// have RT1ENA bit 6 asserted in reg 0, but not RT1STEX bit 4.
		code1 ^= 0x0030;
		code2 ^= 0x0030;
	} 
	if (( RT2_ena == 1) && ( RT2_autostart ==1)) {	
		// IC automatically asserts RT2STEX bit 5 in reg 0, after 
		// auto-initialization. The copied EEPROM image should already 
		// have RT2ENA bit 7 asserted in reg 0,but not RT2STEX bit 5.
		code1 ^= 0x00C0;
		code2 ^= 0x00C0;
	} 
	if (( SMT_ena | IMT_ena ==1) && ( SMT_IMT_autostart ==1)) {	
		// IC automatically asserts MTENA bit 8 in reg 0, after
		// auto-initialization. The monitor begins recording bus activity 
		// upon detecting the next Manchester word (or sync+bits) as 
		// set by bits 6-5 in the MT Configuration reg 0x0029 
		code1 ^= 0x0300;
		code2 ^= 0x0300;
	}


	    // write unlock code #1 
	    pH6130->EEPROM_UNLOCK_REG = code1;
	    // write unlock code #2 
	    pH6130->EEPROM_UNLOCK_REG = code2;         
	// assert HI-613x EECOPY input for 1 ms minimum,
	// and EEPROM copy will begin after EECOPY falling edge
	PIO_Set(&pinEECOPY);
        Delay_x100ms(11);
	PIO_Clear(&pinEECOPY);

	// turn on yellow LED while EEPROM programming is underway
        PIO_Clear(&pinLEDY);

	// READY pin should have dropped to 0 after EECOPY pulse ended
        if(PIO_Get(&pinREADY)) {
            #if(CONSOLE_IO)
                printf("\n\r Error: READY pin should have dropped to 0 after EECOPY pulse ended.\n\r");
            #endif 
            error_trap(6); // flash red LED 6x groups
        }

	// wait for READY pin which signifies completion of EEPROM 
	// copy from HI-613X registers and RAM. The device EE1K 
	// input pin determines whether 1K or 32K x 16 gets copied 
	// to EEPROM. The copy process can take up to 3 seconds...
	while(!PIO_Get(&pinREADY)) ;

	// turn off the yellow LED
        PIO_Set(&pinLEDY);

}	// end: write_init_eeprom()

// end of file

