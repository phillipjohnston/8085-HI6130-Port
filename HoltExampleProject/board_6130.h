/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	board_6130.h
 *    brief     This file contains ARM Cortex M3 hardware definitions for 
 *              initializing the processor External Bus Interface (EBI) 
 *              for Holt HI-6130 projects. (Not used for HI-6131 projects.)
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

//------------------------------------------------------------------------------
//               Atmel ARM Cortex M3 EBI Hardware Definitions
//------------------------------------------------------------------------------


/// The EBI (External Bus Interface) is used only with HI-6130, not SPI HI-6131.
/// For the Holt Evaluation Board, this section replaces EBI definitions found in 
/// Atmel file board.h which references their SAM3U Evaluation Board. Names were 
/// changed so both files can coexist in IAR example project structure, and redefined
/// so the Holt files do not affect operation of the Atmel example projects. 
///
/// The original Atmel pin names: PIN_EBI_DATA_BUS, PIN_EBI_NCS0, PIN_EBI_NRD, 
/// PIN_EBI_NWE, PIN_EBI_PSRAM_ADDR_BUS, PIN_EBI_PSRAM_NBS (Holt only uses NBS0), 
/// PIN_EBI_A1, PIN_EBI_NCS2 (not used here) and PIN_EBI_LCD_RS (not used here).
///
/// Holt EBI revised the PSRAM BUS definitions in the original Atmel board.h file:
/// (1) Reduced output address range from 512K (Addr 19:0) to 64K bytes (Addr 15:0)
///     which frees up GPIO pins for other uses.
/// (2) Added NWAIT for flexibility in bus timing, allowing NWAIT to slow the first
///     read cycle in multi-word sequential fetches, while all words after the first
///     word are much faster because they are prefetched.
/// (3) Removed byte-select NBS1 which is never used by the HI-6120. Byte-select NBS0
///     (ADDR 0) is included becase Holt demo board supports either 8-bit wide bus or
///     16-bit wide bus. 

#define PIN_EBI_DATABUS             {0xfe01fe00, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A}, \
                                        {1 << 6, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B}
#define PIN_EBI_NCS_0               {1 << 20, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_EBI_NREAD               {1 << 19, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_EBI_NWRITE              {1 << 23, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_EBI_NWAIT               {1 << 18, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_EBI_ADDRBUS             {0x0300fff, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_EBI_ADDR1               {1 << 8, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_PULLUP}
#define PIN_EBI_ADDR0               {1 << 7, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_PULLUP}

#define PINS_EBI_6130              PIN_EBI_DATABUS, PIN_EBI_NCS_0, PIN_EBI_NREAD, PIN_EBI_NWRITE, \
                                        PIN_EBI_NWAIT, PIN_EBI_ADDRBUS, PIN_EBI_ADDR1, PIN_EBI_ADDR0
/// HI-6130 base address
#define BOARD_EBI_6130             0x60000000
#define BOARD_6130_SIZE            (0x10000)  // HI-6130 address range is 64K Bytes



//------------------------------------------------------------------------------
//      Global Function Prototypes
//------------------------------------------------------------------------------

void Configure_ARM_MCU_ExtBus(void);


// end of file

