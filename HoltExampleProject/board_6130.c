/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	board_6130.h
 *    brief     This file contains ARM Cortex M3 hardware initialization for
 *              the processor's External Bus Interface (EBI) for Holt HI-6130 
 *              projects. (Not used for HI-6131 projects.)
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

// Holt project headers
#include "device_6130.h"
#include "board_6130.h"
#include "613x_initialization.h"


extern const H6130 pH6130;


//------------------------------------------------------------------------------
// This function initializes and configures the HI-6130 External Bus Interface
//------------------------------------------------------------------------------
// based on Atmel function BOARD_ConfigurePsram(), this function sets up the
// external bus interface (EBI) and the characteristic for the /CS0 chip select
// used by the Holt HI-6130. Changes to Atmel version: [a] Output address range was 
// reduced to 64K bytes (32K words) to free-up upper address output pins for use
// as ordinary GPIO, [b] header file definition deletes output pin connection for
// byte-select outputs used for 8-bit external bus or byte-addressed (e.g flash) 
// memories (still used for internal flash access), and [c] enabled the NWAIT input...
void Configure_ARM_MCU_ExtBus(void) 
{
    const Pin pin6130[] = {PINS_EBI_6130};
    unsigned int tmp;

    // Open EBI clock
    AT91C_BASE_PMC->PMC_PCER = (1<< AT91C_ID_HSMC4);

    // Configure I/O
    PIO_Configure(pin6130, PIO_LISTSIZE(pin6130));

    // Setup the HI-6130 bus (HSMC4_EBI.CS0, 0x60000000 ~ 0x6000FFFF)
    AT91C_BASE_HSMC4_CS0->HSMC4_SETUP = 0
                        | ((1 <<  0) & AT91C_HSMC4_NWE_SETUP)    // 6130 0ns min [A]
                        | ((1 <<  8) & AT91C_HSMC4_NCS_WR_SETUP) // 6130 0ns min 
                        | ((1 << 16) & AT91C_HSMC4_NRD_SETUP)    // 6130 0ns min 
                        | ((1 << 24) & AT91C_HSMC4_NCS_RD_SETUP) // 6130 0ns min 
                        ;
    AT91C_BASE_HSMC4_CS0->HSMC4_PULSE = 0
                        | ((5 <<  0) & AT91C_HSMC4_NWE_PULSE)    // 6130 55ns min [B]
                        | ((5 <<  8) & AT91C_HSMC4_NCS_WR_PULSE) // 6130 55ns min
                        | ((6 << 16) & AT91C_HSMC4_NRD_PULSE)    // 6130 65ns min
                        | ((6 << 24) & AT91C_HSMC4_NCS_RD_PULSE)
                        ;
    AT91C_BASE_HSMC4_CS0->HSMC4_CYCLE = 0
                        | ((6 <<  0) & AT91C_HSMC4_NWE_CYCLE)    // [A]+[B]
                        | ((7 << 16) & AT91C_HSMC4_NRD_CYCLE)
                        ;

    tmp = AT91C_BASE_HSMC4_CS0->HSMC4_MODE & ~(AT91C_HSMC4_DBW);
    AT91C_BASE_HSMC4_CS0->HSMC4_MODE = tmp
                        | (AT91C_HSMC4_READ_MODE)  // read cycle controlled by NRD, not NCSx
                        | (AT91C_HSMC4_WRITE_MODE) // write cycle controlled by NWE, not NCSx
                        | (AT91C_HSMC4_BAT_BYTE_SELECT)
                        | (AT91C_HSMC4_DBW_WIDTH_SIXTEEN_BITS)
                        ;
                    //  | ((1 << 16) & AT91C_HSMC4_TDF_MODE) // 1 cycle data float enabled
                    //  | (AT91C_HSMC4_EXNW_MODE_NWAIT_ENABLE_READY) // NWAIT enabled
                    //  ;
}


// end of file

