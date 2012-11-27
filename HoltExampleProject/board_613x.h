/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	board_613x.h
 *    brief     This file contains definitions applicable to all terminal modes
 *              (BC, RT, SMT, IMT) in both Holt HI-6130 and HI-6131 projects. 
 *              This includes ARM processor port pin assignments and function
 *              prototypes for file board-613x.c 
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

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "613x_initialization.h"  // for HI-6130 vs. HI-6131 determination


/*------------------------------------------------------------------------------
//          ATMEL ARM Cortex M3 PROCESSOR I/O ASSIGNMENTS
//  Most ARM pin assignments are identical for the 2 Holt boards, but several 
//  signals use different pins for HI-6130 (BUS_INTERFACE) & HI-6131 (SPI_INTERFACE).
//
//  '*' indicates signal where HI-6130 and HI-6131 use different port pins
//------------------------------------------------------------------------------

  Function   Signal   Description        HI-6130  HI-6131
  =========  ======== ================   =======  =======
  PIO Input  nRT1MC8  (HI-613x status)    PA0      PA0
  PIO Input  nRT2MC8  (HI-613x status)    PA1      PA1
  PIO Input  MTPKTRDY (HI-613x status)    PB0      PB0
  PIO Input  nIRQ     (HI-613x status)    PB1      PB1
  PIO Input  READY    (HI-613x status)    PB5   *  PB2
  PIO Input  ACTIVE   (HI-613x status)    PC22  *  PC9
                                                      
  PIO Input  nSW1     (pushbutton)        PB17     PB17
  PIO Input  nSW2     (pushbutton)        PB18     PB18
                                                            
  PIO Input  BWID_SW     (DIP switch)     PB2   *  ---
  PIO Input  COPYREQ_SW  (DIP switch)     PB3      PB3
  PIO Input  AUTOEN_SW   (DIP switch)     PB4      PB4
  PIO Input  BENDI_SW    (DIP switch)     PB24  *  PB16
  PIO Input  RT1TFLG_SW  (DIP switch)     PC12     PC12
  PIO Input  RT1BUSY_SW  (DIP switch)     PC13     PC13
  PIO Input  RT2TFLG_SW  (DIP switch)     PC14     PC14
  PIO Input  RT2BUSY_SW  (DIP switch)     PC15     PC15
  PIO Input  RT1ENA_SW   (DIP switch)     PC16  *  PC11
  PIO Input  RT2ENA_SW   (DIP switch)     PC26     PC26
  PIO Input  MTRUN_SW    (DIP switch)     PC27     PC27
  PIO Input  BCENA_SW    (DIP switch)     PC28     PC28
                                                        
  PIO Output ACKIRQ   (HI-613x control)   PC17  *  PC10
  PIO Output EECOPY   (HI-613x control)   PC24     PC24
  PIO Output nMR      (HI-613x control)   PC25     PC25
  PIO Output BCTRIG   (HI-613x control)   PC29     PC29

  PIO Output nLEDA    Bus A LED          PC30      PC30
  PIO Output nLEDB    Bus B LED          PC31      PC31
  PIO Output nLEDG    Green LED          PA29      PA29
  PIO Output nLEDR    Red LED            PA30      PA30
  PIO Output nLEDY    Yellow LED         PA31      PA31

  Peripheral USART1 TXD1 (console)       PA20      PA20
  Peripheral USART1 RXD1 (console)       PA21      PA21
  Peripheral USART1 RTS1 (console)       PA22      PA22
  Peripheral USART1 CTS1 (console)       PA23      PA23

  Peripheral SPI MISO                    PA13      PA13
  Peripheral SPI MOSI                    PA14      PA14
  Peripheral SPI SCK                     PA15      PA15
  Peripheral SPI nSS                     PA16      PA16

  // HI-6130 Only (Bus)
  Peripheral EBI Data0                   PB09      ----
  Peripheral EBI Data1                   PB10      ----  
  Peripheral EBI Data2                   PB11      ----  
  Peripheral EBI Data3                   PB12      ----  
  Peripheral EBI Data4                   PB13      ----  
  Peripheral EBI Data5                   PB14      ----  
  Peripheral EBI Data6                   PB15      ----  
  Peripheral EBI Data7                   PB16      ----  
  Peripheral EBI Data8                   PB25      ----
  Peripheral EBI Data9                   PB26      ----
  Peripheral EBI Data10                  PB27      ----
  Peripheral EBI Data11                  PB28      ----
  Peripheral EBI Data12                  PB29      ----
  Peripheral EBI Data13                  PB30      ----
  Peripheral EBI Data14                  PB31      ----
  Peripheral EBI Data15                  PB06      ----
  ---------------------
  Peripheral EBI Addr0                   PB07      ----
  Peripheral EBI Addr1                   PB08      ----
  Peripheral EBI Addr2                   PC0       ----
  Peripheral EBI Addr3                   PC1       ----
  Peripheral EBI Addr4                   PC2       ----
  Peripheral EBI Addr5                   PC3       ----
  Peripheral EBI Addr6                   PC4       ----
  Peripheral EBI Addr7                   PC5       ----
  Peripheral EBI Addr8                   PC6       ----
  Peripheral EBI Addr9                   PC7       ----
  Peripheral EBI Addr10                  PC8       ----
  Peripheral EBI Addr11                  PC9       ----
  Peripheral EBI Addr12                  PC10      ----
  Peripheral EBI Addr13                  PC11      ----
  Peripheral EBI Addr14                  PC20      ----
  Peripheral EBI Addr15                  PC21      ----
  ---------------------
  Peripheral EBI nRE                     PB19      ----
  Peripheral EBI nCS0                    PB20      ----
  Peripheral EBI nWE                     PB23      ----
  Peripheral EBI nWAIT                   PC18      ----
 */


//------------------------------------------------------------------------------
///      PIO definitions for Holt HI-6130 and HI-6131 Evaluation Boards
///
/// The constants are named using the following convention: * for a constant 
/// that defines a single Pin instance, and PINS_* for a list of Pin instances.
///
/// Push button input definitions

#define PIN_NSW1    {1 << 17, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH}
#define PIN_NSW2    {1 << 18, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH}
//---------------------------------------------------------------
#define PINS_BUTTON PIN_NSW1, PIN_NSW2


/// DIP switch input definitions

//------ port pins for these 3 signals differ by board type ------
///#if(HOST_BUS_INTERFACE) // HI-6130...
  #define PIN_BWID    {1 << 2, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
  #define PIN_BENDI   {1 << 24, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
  #define PIN_RT1ENA  {1 << 16, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
///#else    // (SPI interface) HI-6131...
///  #define PIN_BENDI   {1 << 16, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
///  #define PIN_RT1ENA  {1 << 11, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
///#endif

//-------------------------------------------------------------
#define PIN_CPYREQ  {1 << 3, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
#define PIN_AUTOEN  {1 << 4, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
#define PIN_MTRUN   {1 << 27, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_BCENA   {1 << 28, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_RT2ENA  {1 << 26, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_RT1TFLG {1 << 12, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_RT1BSY  {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_RT2TFLG {1 << 14, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}
#define PIN_RT2BSY  {1 << 15, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}

//-------------------------------------------------------------
///#if(HOST_BUS_INTERFACE) // HI-6130 includes BWID (8-bit vs 16-bit bus width option)
  #define PINS_DIPSW  PIN_BWID, PIN_BENDI, PIN_CPYREQ, PIN_AUTOEN,  \
                      PIN_MTRUN, PIN_BCENA, PIN_RT1ENA, PIN_RT2ENA, \
                      PIN_RT1TFLG, PIN_RT1BSY, PIN_RT2TFLG, PIN_RT2BSY
/*
#else    // SPI interface HI-6131 does not have a BWID pin or DIP switch
  #define PINS_DIPSW  PIN_BENDI, PIN_CPYREQ, PIN_AUTOEN, PIN_MTRUN,   \
                      PIN_BCENA, PIN_RT1ENA, PIN_RT2ENA, PIN_RT1TFLG, \
                      PIN_RT1BSY, PIN_RT2TFLG, PIN_RT2BSY
#endif
*/
//-------------------------------------------------------------



/// Definitions for status inputs coming from the HI-6130 or HI-6131

#define PIN_NRT1MC8 {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT}
#define PIN_NRT2MC8 {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT}
#define PIN_MTPKRDY {1 << 0, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
#define PIN_NIRQ    {1 << 1, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}

  #define PIN_READY   {1 << 5, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT}
  #define PIN_ACTIVE  {1 << 22, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT}


//-------------------------------------------------------------
#define PINS_6130STATUS PIN_NRT1MC8, PIN_NRT2MC8, PIN_MTPKRDY, \
                        PIN_NIRQ, PIN_READY, PIN_ACTIVE



/// LED output pin definitions, 1 = OFF

#define PIN_LEDA     {1 << 30, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LEDB     {1 << 31, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LEDG     {1 << 29, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LEDY     {1 << 30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LEDR     {1 << 31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PINS_LED     PIN_LEDA, PIN_LEDB, PIN_LEDG, PIN_LEDR, PIN_LEDY

// used for direct addressing
#define nLEDA     1 << 30 // on PIOC
#define nLEDB     1 << 31 // on PIOC
#define nLEDG     1 << 29 // on PIOA
#define nLEDY     1 << 30 // on PIOA
#define nLEDR     1 << 31 // on PIOA



/// Definitions for control outputs going to the HI-6130

//------- ACKIRQ signal port pin differs by board type -------
  #define PIN_ACKIRQ {1 << 17, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}

//-------------------------------------------------------------
#define PIN_HSEL   {1 << 23, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_EECOPY {1 << 24, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_NMR    {1 << 25, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_BCTRIG {1 << 29, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
#define PINS_6130CONTROL PIN_ACKIRQ, PIN_HSEL, PIN_EECOPY, PIN_NMR, PIN_BCTRIG

// used for direct addressing
#define nMR         1 << 25 // on PIOC


/// USART1 used for console I/O
#define PIN_USART_RXD    {0x1 << 21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART_TXD    {0x1 << 20, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART_CTS    {0x1 << 23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
#define PIN_USART_RTS    {0x1 << 22, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

#define PINS_USART       PIN_USART_RXD, PIN_USART_TXD, PIN_USART_CTS, PIN_USART_RTS

// UN-REM NEXT WHEN REPLACING BOARD.H WITH BOARD_6130.H
//#define BOARD_USART_BASE     AT91C_BASE_US1
//#define BOARD_ID_USART       AT91C_ID_US1



//------------------------------------------------------------------------------
//      Global Function Prototypes
//------------------------------------------------------------------------------

void ConfigureGpio(void);
void reset_613x(void);
char autoinit_check(void);
void initialize_613x_shared(void);
void init_timer(void);
void Delay_us(unsigned short int num_us);
void Delay_ms(unsigned short int num_ms);
void Delay_x100ms(char num);
void Flash_Red_LED(void);
void Flash_Green_LED(void);
void error_trap(char count);
unsigned short enable_check(void);
void write_init_eeprom(void);

// end of file

