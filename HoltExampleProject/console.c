/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	console.c
 *    object    HyperTerminal-style console I/O for reference design, Holt
                HI-6130 or HI-6131 Evaluation Board based on the Atmel Cortex M-3
 
 *    brief     This file contains functions for UART initialization, console
 *              screen text display and keyboard input when using a program 
 *              like HyperTerminal. As a debug aid, this function is optional
 *              and is enabled or disabled in file 613x_initialization.h :
 *
 *		     CONSOLE_IO  YES  enables console I/O for debug.
 *                               NO   disables it, reducing program size
 *
 *              Settings: 115200 baud, 8 data bits, 1 stop bit, no parity, 
 *              hardware flow control OFF
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
#include <usart/usart.h>
#include <stdio.h>
#include <pio/pio.h>
#include <pmc/pmc.h>

// Holt project headers
#include "613x_regs.h"
#include "board_613x.h"
#include "613x_bc.h"
#include "613x_mt.h"
#include "613x_initialization.h"
#include "console.h"

#include "device_6130.h"
#include "board_6130.h"
extern const H6130 pH6130;


//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

static unsigned short waddr = 0;
static unsigned char watch = 0;


//------------------------------------------------------------------------------
//         Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures USART: hardware flow control OFF, asynchronous, 8 bits, 1 stop
/// bit, no parity, 115200 baud, then enables USART transmitter and receiver.
//------------------------------------------------------------------------------
void ConfigureUsart1(void)
{
  const Pin pins[] = {PINS_USART};
  
  unsigned int mode = AT91C_US_USMODE_NORMAL // USMODE_HWHSH enables RTS/CTS handshaking
                        | AT91C_US_CLKS_CLOCK
                        | AT91C_US_CHRL_8_BITS
                        | AT91C_US_PAR_NONE
                        | AT91C_US_NBSTOP_1_BIT
                        | AT91C_US_CHMODE_NORMAL;

    // configure PIO pins used by USART1
    PIO_Configure(pins, PIO_LISTSIZE(pins));  
  
    // Enable the peripheral clock in the PMC
    PMC_EnablePeripheral(BOARD_ID_USART);

    // Configure the USART in the desired mode @ 115200 baud
    USART_Configure(BOARD_USART_BASE, mode, 115200, BOARD_MCK);

   // Enable USART receiver & transmitter
    USART_SetTransmitterEnabled(BOARD_USART_BASE, 1);
    USART_SetReceiverEnabled(BOARD_USART_BASE, 1);
}


//------------------------------------------------------------------------------
//     function sends ASCII text header to screen
//------------------------------------------------------------------------------
void show_menu(void) {
  
  // Console Output via USART to user HyperTerminal
  
  putchar(12); // clear screen

  printf("\n\r*************************************************\n\r");
  printf("   Holt Integrated Circuits HI-6130_40 EBI Project   \n\r");
  printf("        Compiled: %s %s        \n\r", __DATE__, __TIME__);
  printf("*************************************************\n\n\r");
  #if(BC_ena) 
    printf("   BC On   "); 
  #else 
    printf("   BC Off  ");
  #endif
  #if(SMT_ena) 
    printf("SMT On    ");
  #elif(IMT_ena) 
    printf("IMT On    ");
  #else 
    printf("MT Off   ");
  #endif
  #if(RT1_ena) 
    printf("RT1 On   ");
  #else 
    printf("RT1 Off  ");
  #endif
  #if(RT2_ena) 
    printf("RT2 On");
  #else 
    printf("RT2 Off");
  #endif
    printf("\n\n\r");
  #if(BC_ena) 
    printf(" Press '1' to step BC and list results...\n\r");
    printf(" Press '2' to list BC configuration...\n\r");
    printf(" Press '3' to list BC condition codes & GP flags...\n\r");
  #endif
  #if(SMT_ena || IMT_ena)
    printf(" Press '4' to list MT configuration...\n\r");
    printf(" Press '5' to list MT results, last msg...\n\r");
  #endif
    printf(" Press '6' to list HW interrupt status...\n\r");
  #if(BC_ena) 
    printf(" Press '7' to list BC interrupt status...\n\r");
  #endif
  #if(RT1_ena || RT2_ena)
    printf(" Press '8' to list RT interrupt status...\n\r");
  #endif
  #if(SMT_ena || IMT_ena)
    printf(" Press '9' to list MT interrupt status...\n\r");
  #endif
    
    printf(" NOTE: Options 6-9 clear the accessed Pending Interrupt Register!\n\r"); 
    print_line();
    printf("\n\r");
    print_menuprompt();

} // end of show_menu()


#if(BC_ena) 
//------------------------------------------------------------------------------
//	This function generates a BC external trigger pulse for the HI-613x. 
//	The pin has a pull-down with a push button pulling high. This function
//	provides an alternate, software-generated trigger. To avoid contention
//	with the button, the PA0 GPIO rests in high-z input mode. The PAOUT
//	register sets PA0 high, so this function simply pulses the pin in 
//	output mode to generate trigger.
//
//	After triggering, an in-line delay provides adequate time for 
//	worst case message completion, then console output displays 
//	formatted message results
//------------------------------------------------------------------------------
void bc_last_msg_console(void) {

  unsigned short int i, j, k, addr, wordcount=8, mcsb[10], data[32];
  char rtrt=0,bcast=0,mcode=0,tx=0,nr=0,mtype=0; //,mcd=0;

  // formfeed 
  putchar(12); 	
  printf("\n\rResults From Last Message Issued by BC\n\r");
  print_line();
  printf("Message Type: ");

	// read block address for the last message 
	addr = pH6130->BC_LAST_MSG_BLOCK_ADDR_REG;
	// read BC Control Word 
	mcsb[0] = *((unsigned int *)(0x60000000 + (addr<<1)));
	// read Command Word 
	addr++;
	mcsb[1] = *((unsigned int *)(0x60000000 + (addr<<1)));
        
  if((mcsb[1] & 0xF800) == 0xF800) {
	printf("Broadcast ");
	bcast = 1;
  }

  // RT-RT message? 
  if(mcsb[0] & 1) {
	rtrt=1;
	printf("RT-RT Message, ");
	wordcount = 10;
  }

  // read rest of control-status block 
	for (i=2,addr++; i<wordcount; i++,addr++) {
		mcsb[i] = *((unsigned int *)(0x60000000 + (addr<<1)));
	}

  
  if(!rtrt) {
	if(mcsb[1] & (1<<10)) {
	    printf("Tx ");
            tx = 1;
        }
	else printf("Rx ");
  }

//mtype = mcsb[0] & 7;
  // check CW subaddress field to detect mode code 
  j = mcsb[1] & 0x03E0;
  if((j == 0) || (j == 0x03E0)) {
      // mode code 
      mcode = 1;
      i = 0x1F & mcsb[1];
      if(i > 15) {
          wordcount = 1;
          addr = mcsb[2];
			data[0] = *((unsigned int *)(0x60000000 + (addr<<1)));

    //    mcd=1;
          printf("Mode Code %02d With Data 0x%.2X%.2X\n\n\r", i, (char)(data[0]>>8), (char)data[0]);
      }
      else {
          wordcount = 0;
          if(mcsb[1] & (1<<10)) printf("Mode Code %02d w/o Data\n\n\r", i);
          // undef Rx MC0-15 
          else printf("Mode Code %02d w/o Data, Undefined\n\n\r", i);
      }
  }
  
  else {
      printf("Subaddress Command, ");
      wordcount = mcsb[1] & 0x1F;
      if (!wordcount) wordcount = 32;
      printf("%d data word",wordcount);
      if(wordcount!=1) printf("s");
      printf("\n\n\r"); 
  }                                     

  if (rtrt) {
    // validity check RT-RT 
    if(mcode) printf("RT-RT problem: RxCW mode code.\n\r");
    // CW1 = Rx? 
    if (tx) printf("RT-RT problem: RxCW is transmit.\n\r");
    // CW2 = Tx? 
    if(!(mcsb[8] & (1<<10))) printf("RT-RT problem: TxCW is receive.\n\r");
    // check CW2 mode code 
    j = mcsb[8] & 0x03E0;
    if((j == 0) || (j == 0x03E0)) printf("RT-RT problem: TxCW mode code.\n\r");
    // rule-out CW1 address = CW2 address 
    i = mcsb[1] & 0xF800;
    j = mcsb[8] & 0xF800;
    if(i == j) printf("RT-RT problem: RxCW & TxCW have same RT addr.\n\r");
    // rule-out different word counts 
    i = mcsb[8] & 0x001F;
    if(i != wordcount) printf("RT-RT problem: RxCW & TxCW word counts differ.\n\r");
    printf("\n\r");

    i = mcsb[1];
    printf("CW1: 0x%.2X%.2X", (char)(i>>8), (char)i);
    printf(" = %02d-%1d-%02d-%02d     ", i>>11, (0x0400 & i)>>10, (0x03E0 & i)>>5, (0x001F & i));
    if(bcast) printf("RSW not applicable\n\r");
    else {
        i = mcsb[9];
        printf("RxSW: 0x%.2X%.2X", (char)(i>>8), (char)i);
        printf(" = RT%02d ", (char)(i>>11));
        i &= 0x7FF;
        if(!i) printf("CS\n\r");
        else {
            if(i & (1<<10)) printf("ME ");
            if(i & (1<<9)) printf("INST ");
            if(i & (1<<8)) printf("SRQ ");
            if(i & (7<<5)) printf("RSV ");
            if(i & 16) printf("BCR ");
            if(i & 8) printf("BSY ");
            if(i & 4) printf("SSF ");
            if(i & 2) printf("DBCA ");
            if(i & 1) printf("TF ");
            printf("\n\r");
        }
    }
    i = mcsb[8];
    printf("CW2: 0x%.2X%.2X", (char)(i>>8), (char)i);
    printf(" = %02d-%1d-%02d-%02d     ", (char)(i>>11), (char)((0x0400 & i)>>10), (char)((0x03E0 & i)>>5), (char)((0x001F & i)));
    i = mcsb[7];
    printf("TxSW: 0x%.2X%.2X", (char)(i>>8), (char)i);
    printf(" = RT%02d ", (char)(i>>11));
    i &= 0x7FF;
    if(!i) printf("CS\n\n\r");
    else {
        if(i & (1<<10)) printf("ME ");
        if(i & (1<<9)) printf("INST ");
        if(i & (1<<8)) printf("SRQ ");
        if(i & (7<<5)) printf("RSV ");
        if(i & 16) printf("BCR ");
        if(i & 8) printf("BSY ");
        if(i & 4) printf("SSF ");
        if(i & 2) printf("DBCA ");
        if(i & 1) printf("TF ");
        printf("\n\n\r");
    }
    
    // read Condition Code & General Purpose Flag register

	j = pH6130->BC_CCODE_AND_GPF_REG;

  }       // end rtrt
        
        else { 	// not rtrt

	    // display Command Word  
	    i = mcsb[1];
	    printf("CW: 0x%.2X%.2X", (char)(i>>8), (char)i);
	    printf(" = %02d-%1d-%02d-%02d     ", (char)(i>>11), (char)((0x0400 & i)>>10), (char)((0x03E0 & i)>>5), (char)((0x001F & i)));
            
            // read Condition Code & General Purpose Flag register
            

		j = pH6130->BC_CCODE_AND_GPF_REG;

                
	    // check broadcast 
	    if (bcast) printf("SW not applicable\n\n\r");

	    else {
		// not broadcast, check if No Response timeout 
		if(j & (1<<8)) {
		    // normal subaddress msg without RT response, excl rt-rt                   
		    printf("SW not received\n\n\r");
		    nr = 1;
		}
		else {
		    // normal subaddress msg with RT response, excl rt-rt 
		    i = mcsb[7];
		    // decode received RT status 
		    printf("SW: 0x%.2X%.2X", (char)(i>>8), (char)i);
		    printf(" = RT%02d ",i>>11);
		    i &= 0x7FF;
		    if(!i) printf("CS\n\n\r");
		    else {
			if(i & (1<<10)) printf("ME ");
			if(i & (1<<9)) printf("INST ");
			if(i & (1<<8)) printf("SRQ ");
			if(i & (7<<5)) printf("RSV ");
			if(i & 16) printf("BCR ");
			if(i & 8) printf("BSY ");
			if(i & 4) printf("SSF ");
			if(i & 2) printf("DBCA ");
			if(i & 1) printf("TF ");
		        printf("\n\n\r");	
                    }
                }
            }
	}	// end "not rtrt"

	// BC Control Word for message 
	i = mcsb[0];
	printf("BC Control Word: 0x%.2X%.2X  \n\r", (char)(i>>8), (char)i);
	if(i & (1<<15)) printf("MC17TTL ");
	if(i & (1<<14)) printf("MEmask ");
	if(i & (1<<13)) printf("SRQmask ");
	if(i & (1<<12)) printf("BSYmask ");
	if(i & (1<<11)) printf("SSFmask ");
	if(i & (1<<10)) printf("TFmask ");
	if(i & (1<<9)) printf("RSVmask ");
	if(i & (1<<8)) printf("RetryEna ");
	if(i & (1<<7)) printf("UseBusA ");
	else           printf("UseBusB ");
	if(i & (1<<6)) printf("SelfTest ");
	if(i & (1<<5)) printf("maskBCR ");
	if(i & (1<<4)) printf("EOMirq ");
	if((i & 7) == 0) printf("NonBcstSA ");
	if((i & 7) == 1) printf("RTRT ");
	if((i & 7) == 2) printf("BcstSA ");
	if((i & 7) == 3) printf("BcstRTRT ");
	if((i & 7) == 4) printf("MC ");
	if((i & 7) == 5) printf("RTRT_MC?? ");
	if((i & 7) == 6) printf("BcstMC ");
	if((i & 7) == 7) printf("BcstRTRT_MC?? ");
	printf("\n\r");

	k = (i & 2) >> 1;
	// Block Control broadcast format bit 1 should agree w/ CW1 
	if(k != bcast) printf("The Cmd Word and Control Word bit 1 disagree on broadcast format.\n\r");
	k = (i & 4) >> 2;
	// Block Control mode code format bit 2 should agree w/ CW1 
	if(k != mcode) printf("The Cmd Word and Control Word bit 2 disagree on mode code format.\n\r");
	printf("\n\r");


	// Block Status Word 
	i = mcsb[5];
	printf("Block Status Word: 0x%.2X%.2X \n\r", (char)(i>>8), (char)i);
	if(i & (1<<14)) printf("SOM  ");
	if(i & (1<<15)) printf("EOM  ");
	if(i & (1<<13)) printf("BusB  ");
	else printf("Bus A  ");
	if(i & (1<<12)) printf("ErrOcc ");
	if(i & (1<<11)) printf("SSet  ");
	if(i & (1<<9)) printf("NoResp  ");
	if(i & (1<<8)) printf("LpBk  ");
	if(i & (1<<6)) printf("2retry  ");
	else if(i & (1<<5)) printf("1retry  ");
	if(i & (1<<4)) printf("GDB  ");
	if(i & (1<<3)) printf("WAG  ");
	if(i & (1<<2)) printf("WdCt  ");
	if(i & (1<<1)) printf("SyncErr  ");
	if(i & (1<<0)) printf("InvWd  ");
	if(i & (1<<10)) printf("FmtErr  ");
	if(i & (1<<7)) printf("MSSet  ");
	printf("\n\n\r");

	// j = Condition Code Register value
	printf("Condition Code Register: 0x%.2X%.2X\n\r", (char)(j>>8), (char)j);
	if(j==0x0000) printf("BC Stopped: No Condition Codes or Gen Purpose Flags Are Set.");
	else if(j==0x8000) printf("BC Running: No Condition Codes or Gen Purpose Flags Are Set.");
	else if(j & (1<<15)) printf("BC Is Running.\n\r");
	else printf("BC Not Running.\n\r");
	if(j & (1<<14)) printf("2Retries  ");
	if(j & (1<<13)) printf("1Retry  ");
	if(j & (1<<12)) printf("BadMsg  ");
	if(j & (1<<10)) printf("GdDataBlk  ");
	if(j & (1<<9)) printf("FmtErr  ");
	if(j & (1<<8)) printf("NoResp  ");
	if(j & (1<<7)) printf("GPF7  ");
	if(j & (1<<6)) printf("GPF6  ");
	if(j & (1<<5)) printf("GPF5  ");
	if(j & (1<<4)) printf("GPF4  ");
	if(j & (1<<3)) printf("GPF3  ");
	if(j & (1<<2)) printf("GPF2  ");
	if(j & (1<<1)) printf("LT-GPF1  ");
	if(j & (1<<0)) printf("EQ-GPF0  ");
	if(j & (1<<11)) printf("MSSet  ");
	printf("\n\n\r");

	// if not a mode command, read data block then display it
	if(!(mtype & 4)) {

	    // rule out tx command with no response 
	    if(tx & nr & !rtrt) printf("No RT response, no received data\n\n\r");

	    else {
	        // store data words as array 
	        addr = mcsb[2];
			for (i=0; i<wordcount; i++,addr++) {
				data[i] = *((unsigned int *)(0x60000000 + (addr<<1)));
                        }
                        
	        // print the Data Buffer start address 
	        printf("Data Addr: 0x%.2X%.2X,      ",(char)(j>>8),(char)j);

			j = mcsb[2] << 1;
			printf("Bus Addr: 0x6000%.2X%.2X\n\rData:",(char)(j>>8),(char)j);

			printf("\n\r");

	        // print data words, 8 words per line 
	        for(i=0; (i<8)&&(i<wordcount); i++) {
			j = data[i];
			printf("0x%.2X%.2X ",(char)(j>>8),(char)j);
                }
	        printf("\n\r");
	        if(wordcount>7) {
	    	    for(i=8; (i<16)&&(i<wordcount); i++) {
					j = data[i];
					printf("0x%.2X%.2X ",(char)(j>>8),(char)j);
                }
				printf("\n\r");
            }
	        if(wordcount>15) {
	    	    for(i=16; (i<24)&&(i<wordcount); i++) {
					j = data[i];
					printf("0x%.2X%.2X ",(char)(j>>8),(char)j);
                }
				printf("\n\r");
            }
	        if(wordcount>23) {
				for(i=24; (i<32)&&(i<wordcount); i++) {
					j = data[i];
	                printf("0x%.2X%.2X ",(char)(j>>8),(char)j);
                }
				printf("\n\r");
            }
        }	// end else "store data words"
    }	// end "not a mode command"

	printf("\n\r");
	print_line();
	print_menuprompt();

}	// end bc_last_msg_console()
#endif //(BC_ena) 



#if (BC_ena) 
//--------------------------------------------------------------
//  this function lists the BC Config reg 0x0032
//	to the console via UART
//--------------------------------------------------------------
void list_bc_config (void) {

	unsigned short int i, j;


	i = pH6130->BC_CONFIG_REG;

                
	// formfeed 
	putchar(12); 	
	printf("\n\rBC Configuration Register: 0x%.2X%.2X\n\r",(char)(i>>8),(char)i);
	print_line();
	
	j = (i & 0xC000) >> 14;
	if(j == 1) printf("  20");
	else if(j == 2) printf("  58");
	else if(j == 3) printf(" 138");
	else printf("  15");
	printf("us Timeout (bus dead time)   ");

	if(i & (1<<13)) printf("Check Control Word Format Bits");
	else printf("Don't Check Control Word Format Bits");
	printf("\n\n\r");

	if(i & (1<<11)) {
		j = (i & (3<<9)) >> 9;
		if(j==0) printf(" 2 Retries, Same Bus, Same Bus   "); 
		else if(j==2) printf("  2 Retries, Alt Bus, Same Bus   "); 
		else if(j==1) printf("  2 Retries, Same Bus, Alt Bus   "); 
		else printf("   2 Retries, Alt Bus, Alt Bus   "); 
	}
	else if(i & (1<<12)) {
		if(i & (1<<10)) printf("  1 Retry if Msg Fail, Alt Bus   ");
		else printf(" 1 Retry if Msg Fail, Same Bus   ");
	}
	else printf("    No Retries for Msg Failure   ");

	if(i & (1<<8)) printf("Retry if RT status bit high");
	else printf("No Retry if RT status bit high");
	printf("\n\n\r");

	if(i & (1<<7)) printf(    " RT status ME w/o data = Valid   ");
	else printf(" RT status ME w/o data = Inval   ");
	if(i & (1<<6)) printf("RT status BSY w/o data = Valid");
	else printf("RT status BSY w/o data = Inval");
	printf("\n\n\r");

	if(i & (1<<5)) printf("            Msg Gap Timer = On   ");
	else printf("           Msg Gap Timer = Off   ");
	if(i & (1<<4)) printf("Watchdog Frame Timer = On");
	else printf("Watchdog Frame Timer = Off");
	printf("\n\n\r");

	if(i & (1<<3)) printf("MC17 send Time Tag low 16 bits   ");
	else printf("MC17 send data in msg data blk   ");
	if(i & (1<<2)) printf("MC17 data word always even");
	else printf("MC17 data may be even or odd");
	printf("\n\n\r");

	if(i & (1<<1)) printf("  Enable 4us Minimum Gap Check   ");
	else printf("   No 4us Minimum Gap Checking   ");
	if(i & (1<<0)) printf("BCR Mask Enabled, BCRME = 1");
	else printf("BCR Mask Disabled, BCRME = 0");
	printf("\n\n\r");


		i = pH6130->TTAG_CONFIG_REG;
           
	printf("Timetag Config Register 0x%.2X%.2X  ",(char)(i>>8),(char)i);

	if(i & (1<<3)) printf("32-bit timebase with ");
	else printf("16-bit timebase with ");

	i &= 7;
	if(i == 0) printf("clock disabled! ");
	else if(i == 1) printf("ext clock ");
	else if(i == 2) printf("2us clock ");
	else if(i == 3) printf("4us clock ");
	else if(i == 4) printf("8us clock ");
	else if(i == 5) printf("16us clock ");
	else if(i == 6) printf("32us clock ");
	else printf("64us clock ");
	printf("\n\n\r");

	print_line();
  	print_menuprompt();

}	// end list_bc_config()
#endif //(BC_ena) 


#if (BC_ena) 
//-------------------------------------------------------------------------
//    brief	this function lists the BC's Cond Code / GP Flag reg
//		to the console via UART
//-------------------------------------------------------------------------
void list_bc_ccgpf_reg(void) {

	unsigned short int i;


	i = pH6130->BC_CCODE_AND_GPF_REG;

	// formfeed
	putchar(12); 	
	printf("\n\rBC Condition Code & GP Flag Register: 0x%.2X%.2X\n\r", (char)(i>>8),(char)i);
	print_line();

	if(i==0x0000) printf("BC Stopped: No Condition Codes or Gen Purpose Flags Are Set.\n\r");
	else if(i==0x8000) printf("BC Running: No Condition Codes or Gen Purpose Flags Are Set.\n\r");
	else if(i & (1<<15)) printf("BC Is Running.\n\n\r");
	else printf("BC Not Running.\n\n\r");
	if(i & (1<<14)) printf("2Retries  ");
	if(i & (1<<13)) printf("1Retry  ");
	if(i & (1<<12)) printf("BadMsg  ");
	if(i & (1<<10)) printf("GdDataBlk  ");
	if(i & (1<<9)) printf("FmtErr  ");
	if(i & (1<<8)) printf("NoResp  ");
	if(i & (1<<7)) printf("GPF7  ");
	if(i & (1<<6)) printf("GPF6  ");
	if(i & (1<<5)) printf("GPF5  ");
	if(i & (1<<4)) printf("GPF4  ");
	if(i & (1<<3)) printf("GPF3  ");
	if(i & (1<<2)) printf("GPF2  ");
	if(i & (1<<1)) printf("LT-GPF1  ");
	if(i & (1<<0)) printf("EQ-GPF0  ");
	if(i & (1<<11)) printf("MSSet  ");
	printf("\n\r");
        
	print_line();
   	print_menuprompt();

}
#endif //(BC_ena) 



#if (BC_ena) 
//-------------------------------------------------------------
//      this function lists bus controller interrupt 
//	configuration to the console via UART. if there
//	are pending BC interrupts, these too are displayed.
//	Function returns the value read from the BC pending
//	interrupt register, since reading the reg clears it.
//-------------------------------------------------------------
unsigned short int list_bc_ints_console (void) {

	unsigned short int i, j, k=0;

	// formfeed 
	putchar(12); 	

	#if(!BC_ena) 
	printf("Bus Controller Is Not Enabled!\n\n\r"); 
	#endif


		i = pH6130->BC_INT_ENABLE_REG;
		j = pH6130->BC_INT_OUTPUT_ENABLE_REG;
		k = pH6130->BC_PENDING_INT_REG;

                
	printf("\n\r Bus Controller Ints   Enabled?   Pin Output?   Pending?\n\r");
	print_line();

	printf("BC Frame Timer Expired");
	if(!(i & (1<<15))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<15)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<15)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("Selected Msg Int,EOM=1");
	if(!(i & (1<<14))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<14)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<14)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("GenPurp Queue Rollover");
	if(!(i & (1<<13))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<13)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<13)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("   Message Was Retried");
	if(!(i & (1<<12))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<12)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<12)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("Call Stack Pointer Err");
	if(!(i & (1<<11))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<11)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<11)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("  Illegal Op Code Trap");
	if(!(i & (1<<10))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<10)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<10)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("Unexpected RT Stat Bit");
	if(!(i & (1<<9))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<9)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<9)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("User-Def Int Req Bit 3");
	if(!(i & (1<<8))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<8)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<8)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("User-Def Int Req Bit 2");
	if(!(i & (1<<7))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<7)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<7)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("User-Def Int Req Bit 1");
	if(!(i & (1<<6))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<6)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<6)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("User-Def Int Req Bit 0");
	if(!(i & (1<<5))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<5)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<5)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("RT Status is Msg Error");
	if(!(i & (1<<4))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<4)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<4)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("     BC End-of-Message");
	if(!(i & (1<<3))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<3)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<3)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("     Reserved Bits 2-0");
	print_dddn();
	print_line();
	printf("Note: Reading the BC Pending Interrupt Register clears it!\n\r");
	print_menuprompt();

	// return value read from the BCW Pending Interrupt Register, now cleared 
	return k;

}	// end list_bc_ints_console()


#endif //(BC_ena) 

#if(SMT_ena || IMT_ena)

//-----------------------------------------------------------------------
//    this function lists the MT Config reg 0x0029
//    to the console via UART
//-----------------------------------------------------------------------
void list_mt_config (void) {

	unsigned short int i, j;
	char smt = 0;


	i = pH6130->MT_CONFIG_REG;
               
	// IRIG monitor (imt) or Simple monitor (smt)? 
	if(i & 1) smt = 1;

	// formfeed 
	putchar(12); 	
	if(smt) printf("\n\rS");
	else    printf("\n\rI");
	printf("MT monitor mode selected.\n\n\r");

	printf("MT Configuration Register: 0x%.2X%.2X\n\r", (char)(i>>8),(char)i);
	print_line();

	j = (i & 0xC000) >> 14;
	if(j == 1) printf("  20us Timeout");
	else if(j == 2) printf("  58us Timeout");
	else if(j == 3) printf(" 138us Timeout");
	else printf("  15us Timeout");
	printf(" (bus dead time)   ");
	
	if(i & (1<<12)) printf("2us Gap Check ON");
	else printf("2us Gap Check OFF");
	printf("\n\n\r");

	j = (i & 0x0300) >> 8;
	if(j==0) printf("  Time Tag last word, last bit   "); 
	else if(j==1) printf("Time Tag first word, first bit   "); 
	else if(j==2) printf(" Time Tag first word, last bit   "); 
	else printf("           Time Tagging is OFF   "); 

	j = (i & 0x0060) >> 5;
	printf("Start Rec: First Cmd ");
	if(j==0) printf("Word");
	else if(j==1) printf("or Data Word");
	else if(j==2) printf("Sync + 2 bits");
	else printf("or Data Sync + 2 bits");
	printf("\n\n\r");

	printf("   Invalid Data Word");	
	if(i & (1<<4)) printf("s recorded   ");
	else printf          (" ends mssg   ");

	printf("Extended Status Flags ");
	if(i & (1<<2)) printf("ON");
	else printf("OFF");
	printf("\n\n\r");

	if(smt) {
		printf(" SMT using ");
		if(i & 2) printf("48");
		else printf("16");
		printf("-bit Time Tag ");
	}
	else {  // imt
		printf("IMT Pkt Timer starts ");
		if(i & (1<<7)) printf("at PktEnd   ");
		else printf("at Cmd Wd   ");

		j = (i & 0x0C00) >> 10;
		printf("IMT Hdr Data Type IRIG-106-");
		if(j==0) printf("09  "); 
		else if(j==1) printf("04  "); 
		else if(j==2) printf("05  "); 
		else printf("07  "); 
		printf("\n\n\r");
						 
		printf (" IMT Auto Header & Trailer ");
		if(i & 8) printf ("OFF   ");
		else printf      (" ON   ");
	
		printf ("IMT Data Packet Checksum ");
		if(i & 2) printf ("ON");
		else printf("OFF"); 
	} 
	printf("\n\n\r");
	print_line();
	print_menuprompt();

}	// end list_mt_config()


//-----------------------------------------------------------------------
//    this function lists the MT msg results, last message
//    to the console via UART
//-----------------------------------------------------------------------

void mt_last_msg_console(void) {

    
        extern const H6130 pH6130; // HI-6130 register struct base address    

	unsigned short int h,i,j,m,addr,dbp,ndw;
	char k,bswo=3,smt=0,xmf=0,ttag16=0,rtrt=1;

	// formfeed 
	putchar(12); 	

	if(SMT_ena||IMT_ena) {

		printf("\n\rLast Message Recorded by ");

		// fetch Bus Monitor config word 

		i = pH6130->MT_CONFIG_REG;

                        
		// extended message status flags enabled? 
		if(i & 2) xmf = 1;
		// Simple monitor (smt)? 
		if(i & 1) {
			printf("SMT:    ");
			smt = 1;
			//--------------------------------------------------------------
				// for smt, msg block start addr for last msg is in reg 0x31 
				addr = pH6130->MT_LAST_MSG_STACK_ADDR_REG;
				// j = address list offset 
				j = pH6130->MT_ADDR_LIST_POINTER;

				if(!(i&2)) {
					// smt with 16-bit ttag 
					ttag16 = 1;
					// bswo = block status word offset in cmd-stack msg block 
					bswo = 0;
					// data block pointer 
					dbp = *((unsigned int *)(0x60000000 + ((addr+2)<<1)));
				}
				else {
					// smt with 48-bit ttag
					// bswo = block status word offset in cmd-stack msg block 
					bswo = 3;
					// data block pointer 
					dbp = *((unsigned int *)(0x60000000 + ((addr+6)<<1)));
				}

				// first msg only, last block start addr = stack start addr 
				if(addr == *((unsigned int *)(0x60000000 + (j<<1)))) {
					// for first msg, the number of words stored in the
					// data stack = next data store addr - stack start addr 
					ndw = *((unsigned int *)(0x60000000 + ((j+5)<<1))) \
						- *((unsigned int *)(0x60000000 + ((j+4)<<1)));
				}
				else {
					h = *((unsigned int *)(0x60000000 + ((j+1)<<1)));
					// h = offset for next store addr in cmd stack block:
					// examine cmd word, detect both cases for broadcast mode codes 
					// w/o data, for these, no words are stored in the data stack
					m = *((unsigned int *)(0x60000000 + ((h-1)<<1)));
					if     (( m & 0xFFE0) >> 5 == 0x7E0) ndw = 0;
					else if(( m & 0xFFE0) >> 5 == 0x7FF) ndw = 0;
					else {
						// at least 1 word stored in data stack for last msg 
						// use next d-stack store address to determine number of d-stack words  
						h = *((unsigned int *)(0x60000000 + ((j+5)<<1)));
						ndw = h - dbp;
					}
				}
			//--------------------------------------------------------------

			//--------------------------------------------------------------
		}	// end smt 


		else if(i & 8) {
			// IMT with auto header/trailer OFF 
			printf("IMT:   ");
			bswo = 4;
			// the last msg block addr is in register 0x31 

				addr =pH6130->MT_LAST_MSG_STACK_ADDR_REG;

                                
			// the stored data starts at the 8th word  
			dbp = addr+7;
			// # of msg bytes in word 6

				ndw = *((unsigned int *)(0x60000000 + ((addr+6)<<1)));

			// convert bytes to words
			ndw = ndw/2;
			
		}
		else  {  
			// IMT with auto header/trailer ON 
			printf("IMT:   ");
			bswo = 4;
			// last msg block addr is stored in 5th word of MT addr list 

				i = 4 + pH6130->MT_ADDR_LIST_POINTER;
				addr = *((unsigned int *)(0x60000000 + (i<<1)));
				// the stored data starts at the 8th word  
				dbp = addr+7;
				// # of msg bytes in word 6, convert to words (div-by-2)
				ndw = (*((unsigned int *)(0x60000000 + (addr+6)<<1)))/2;

		}

		// read Block Status Word 

		i = *((unsigned int *)(0x60000000 + ((addr+bswo)<<1)));

                
		printf("Block Status Word = 0x%.2X%.2X...\n\n\r", (char)(i>>8),(char)i);
		if(!smt) printf("* means IMT BSW reserved bit:\n\n\r");
		printf("           * Start of Msg ");
		if(!xmf || !smt) printf("n/a    ");
		else if(i & (1<<14)) printf("= 1    ");
		else printf("= 0    ");
		printf("Bus Used = ");
		if(i & (1<<13)) printf("B\n\n\r");
		else printf("A\n\n\r");

		printf("             * End of Msg ");
		if(!xmf || !smt) printf("n/a    ");
		else if(i & (1<<15)) printf("= 1    ");
		else printf("= 0    ");
		printf("Error Occurred = ");
		if(i & (1<<12)) printf("1\n\n\r");
		else printf("0\n\n\r");

		printf("        * Good Data Block ");
		if(!xmf) printf("n/a    ");
		else if(i & (1<<8)) printf("= 1    ");
		else printf("= 0    ");
		printf("Response Timeout = ");
		if(i & (1<<9)) printf("1\n\n\r");
		else printf("0\n\n\r");

		printf("* SMT Data Stack Rollover ");
		if(!xmf) printf("n/a    ");
		else if(i & (1<<7)) printf("= 1    ");
		else printf("= 0    ");
		printf("Illegal Gap Error = ");
		if(i & (1<<10)) printf("1\n\n\r");
		else printf("0\n\n\r");

		printf(" * Cmd Word Content Error ");
		if(!xmf) printf("n/a    ");
		else if(i & 1) printf("= 1    ");
		else printf("= 0    ");
		printf("Word Count Error = ");
		if(i & (1<<5)) printf("1\n\n\r");
		else printf("0\n\n\r");

		printf("* RT-RT Gap,Sync,Addr Err ");
		if(!xmf) printf("n/a    ");
		else if(i & 4) printf("= 1    ");
		else printf("= 0    ");

		if(!(i & (1<<11))) {
			rtrt = 0;
			printf("Non-");
		}
		printf("RT-RT Cmd: ");
		// fetch & printf CW1 
		if(!smt)        m = 7; 
		else if(ttag16) m = 3; 
		else            m = 7;

			j = *((unsigned int *)(0x60000000 + ((addr+m)<<1)));

		printf("0x%.2X%.2X", (char)(j>>8),(char)j);

		if(rtrt) {
			// fetch & printf CW2 

				if(!smt) j = *((unsigned int *)(0x60000000 + ((addr+8)<<1)));
				else j = *((unsigned int *)(0x60000000 + (dbp<<1)));

                                
			printf("0x%.2X%.2X", (char)(j>>8),(char)j);
		}

		printf("\n\n\r * RT-RT Cmd Word 2 Error ");
		if(!xmf) printf("n/a    ");
		else if(i & 2) printf("= 1    ");
		else printf("= 0    ");

		printf("Timetag = ");
		if(smt & ttag16) {	// 16-bit ttag

			i = *((unsigned int *)(0x60000000 + ((addr+1)<<1)));

			printf("0x%.2X%.2X\n\n\r", (char)(i>>8),(char)i);
		}
		else {			// 48-bit ttag

				h = *((unsigned int *)(0x60000000 + ((addr+2)<<1)));
				i = *((unsigned int *)(0x60000000 + ((addr+1)<<1)));
				m = *((unsigned int *)(0x60000000 + (addr<<1)));

			printf("0x%.2X%.2X", (char)(h>>8),(char)h);
			printf("%.2X%.2X", (char)(i>>8),(char)i);
			printf("%.2X%.2X\n\n\r", (char)(m>>8),(char)m);
		}
		
		if(smt) {
			if(!ndw) printf("No Words Stored in Data Stack    (Broadcast Mode Code w/o Data)\n\n\r");
			else {
				if(ndw<9) printf(" ");
				printf("Msg Words 2-%d at Addr 0x%.2X%.2X   ", ndw+1, (char)(dbp>>8),(char)dbp);
			}
		}

		else {
			if(ndw<10) printf("  %d",ndw);
			else printf(" %d",ndw);
			printf(" Msg Words at Addr: ");
			printf("0x%.2X%.2X   ", (char)(dbp>>8),(char)dbp);
		}
		if(ndw) {

				dbp = dbp<<1;
				printf("Bus Debug Addr: 0x6000%.2X%.2X", (char)(dbp>>8),(char)(dbp));

			printf("\n\n\r");
		}

		// only for imt, or smt using 48-bit ttag option
		// (smt using 16-bit ttag option does not store gap time word):
		// get gap time word, the upper and lower bytes 
		// express both TxRT and RxRT gap times for RT-RT messages... 
		if(!smt || (smt & !ttag16)) {
			if(!smt) m = 5;
			else if(!ttag16) m = 4;

			j = *((unsigned int *)(0x60000000 + ((addr+m)<<1)));

                                
			// lower byte gives gap time for non-RT-RT msgs,
			// and gives the TxRT gap for RT-RT msgs 
			k = 0;
			i = j&0xFF;
			// expressed in 0.1us increments, parse byte into 
			// whole number k and tenths digit i for display 
			while (i >= 10) {
				i -= 10;
				k += 1;
			}
			if(k<10) printf(" ");
			if(rtrt) printf("       Tx");
			//else printf("         ");
			printf("RT Response = %d.%dus    ", k,i);
			if(!k && !i) printf("(Broadcast?)");
			if(!rtrt) printf("\n\r");

			// upper byte is non-zero for RT-RT only,
			// gives the RxRT gap if not broadcast msg 
			else {
				k = 0;
				i = (j>>8) & 0xFF;
				while (i >= 10) {
					i -= 10;
					k += 1;
				}
				printf("RxRT Response = %d.%dus ", k,i);
				if(!k && !i) printf("(Broadcast?)");
				printf("\n\r");
			}
		}
	}	
	print_line();
	print_menuprompt();

}	// end mt_last_msg_console()


//-----------------------------------------------------------------------
//	this function lists bus monitor interrupt 
//	configuration to the console via UART. if there
//	are pending MT interrupts, these too are displayed.
//	Function returns the value read from the MT pending
//	interrupt register, since reading the reg clears it.
//-----------------------------------------------------------------------
unsigned short int list_mt_ints_console(void) {

   
        extern const H6130 pH6130; // HI-6130 register struct base address    

	unsigned short i, j, k=0;
	char smt = 0;


		i = pH6130->MT_CONFIG_REG;
		if(i & 1) smt = 1;
		i = pH6130->MT_INT_ENABLE_REG;
		j = pH6130->MT_INT_OUTPUT_ENABLE_REG;
		k = pH6130->MT_PENDING_INT_REG;

                

	// formfeed 
	putchar(12); 	
	// IRIG monitor (imt) or Simple monitor (smt)? 
        printf("\n\r");
	if(smt) printf("SMT mode is selected by MT Config Register bit 0.\n\n\r");
	else    printf("IMT mode is selected by MT Config Register bit 0.\n\n\r");
	printf("   Bus Monitor Ints    Enabled?   Pin Output?   Pending?\n\r");
	print_line();

	if(!smt) {
		// IRIG-106 mode, disregard bits 7,4,2,1,0 
		printf("End of Pkt,Buffer Full");
		if(!(i & FULL_EOP)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & FULL_EOP) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & FULL_EOP) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("Offset before FULL_EOP");
		if(!(i & FULL_OFS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & FULL_OFS) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & FULL_OFS) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("Pkt Max 1553 Msg Words");
		if(!(i & MAXWORDS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MAXWORDS) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MAXWORDS) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("Pkt Max 1553 Msg Count");
		if(!(i & MAXMSGS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MAXMSGS) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MAXMSGS) printf("1\n\r");
			else printf("0\n\r"); 
		}
	
		printf(" Max Gap Time Exceeded");
		if(!(i & MAXGAP)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MAXGAP) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MAXGAP) printf("1\n\r");
			else printf("0\n\r"); 
		}
	
		printf("Max Pkt Recording Time");
		if(!(i & MAXTIME)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MAXTIME) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MAXTIME) printf("1\n\r");
			else printf("0\n\r"); 
		}
	
		printf("      Host Packet Stop");
		if(!(i & HPKTSTOP)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & HPKTSTOP) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & HPKTSTOP) printf("1\n\r");
			else printf("0\n\r"); 
		}
				 
		printf("Stack End Addr Written");
		if(!(i & STKROVR)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & STKROVR) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & STKROVR) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf(" Hit Cmd Stack Address");
		if(!(i & STKADRSS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & STKADRSS) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & STKADRSS) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf(" IRIG-106 Packet Ready");
		if(!(i & PKTREADY)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & PKTREADY) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & PKTREADY) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("Monitor End-of-Message");
		if(!(i & (1<<3))) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & (1<<3)) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & (1<<3)) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("\nIMT Skips Bits 4,2,1,0");
		print_dddn();

	}		// end if(!smt)

	else {
		// Simple Monitor mode, disregard bits 15-9 and 2-0 

		printf("SMT Cmd Stack Rollover");
		if(!(i & STKROVR)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & STKROVR) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & STKROVR) printf("1\n\r");
			else printf("0\n\r"); 
		}
	
		printf("SMT DataStack Rollover");
		if(!(i & DSTKROVR)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & DSTKROVR) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & STKROVR) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("SMT  Cmd Stack Address");
		if(!(i & STKADRSS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & STKADRSS) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & STKADRSS) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("SMT Data Stack Address");
		if(!(i & DSTKADRSS)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & DSTKADRSS) print_b1sp();
			else print_b0sp(); 
				// pending? 
			if(k & DSTKADRSS) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("RT Status is Msg Error");
		if(!(i & MSG_ERR)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MSG_ERR) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MSG_ERR) printf("1\n\r");
			else printf("0\n\r"); 
		}

		printf("     MT End-of-Message");
		if(!(i & MT_EOM)) {	
			// int disabled 
			print_null();
		}
		else {
			// int enabled 
			print_sp1sp();
			// pin output? 
			if(j & MT_EOM) print_b1sp();
			else print_b0sp(); 
			// pending? 
			if(k & MT_EOM) printf("1\n\r");
			else printf("0\n\r"); 
		}
	}		// end else .... SMT mode
          
	print_line();
	printf("Note: Reading the MT Pending Interrupt Register clears it!\n\r");

	print_menuprompt();

	// return value read from the MT Pending Interrupt Register, now cleared 
	return k;
 
}	// end list_mt_ints_console()


#endif  // (SMT_ena || IMT_ena)


//-------------------------------------------------------------------------
//  this function lists hardware interrupt configuration
//	to the console via UART. if there are pending
//	hardware interrupts, these too are displayed.
//	Function returns the value read from the HW pending
//	interrupt register, since reading the reg clears it.
//-------------------------------------------------------------------------
unsigned short int list_hw_ints_console (void) {

	unsigned short i, j, k;
        

        extern const H6130 pH6130; // HI-6130 register struct base address  

        
	// formfeed 
	putchar(12); 	
	printf("\n\r Hardware Interrupts   Enabled?   Pin Output?   Pending?\n\r");
	print_line();

		i = pH6130->HDW_INT_ENABLE_REG;
		j = pH6130->HDW_INT_OUTPUT_ENABLE_REG;
		k = pH6130->HDW_PENDING_INT_REG;
		printf("HI-6131 Host SPI Error    n/a         n/a         n/a\n\r");

                
	printf("EE Chksum/Corr RAM Err");
	if(!(i & (1<<14))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<14)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<14)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("RAM Init/UncorrRAM Err");
	if(!(i & (1<<13))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<13)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<13)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("Loopback Fail on Bus A");
	if(!(i & (1<<12))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<12)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<12)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("Loopback Fail on Bus B");
	if(!(i & (1<<11))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<11)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<11)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("MT Timetag Ct Rollover");
	if(!(i & (1<<10))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<10)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<10)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("BC Timetag Ct Rollover");
	if(!(i & (1<<9))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<9)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<9)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("RT2 Timetag Valu Match");
	if(!(i & (1<<8))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<8)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<8)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("RT1 Timetag Valu Match");
	if(!(i & (1<<7))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<7)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<7)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("MT Timetag Value Match");
	if(!(i & (1<<6))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<6)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<6)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("BC Timetag Value Match");
	if(!(i & (1<<5))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<5)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<5)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("  RT2 Addr Parity Fail");
	if(!(i & (1<<4))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<4)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<4)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf("  RT1 Addr Parity Fail");
	if(!(i & (1<<3))) {	
		// int disabled 
		print_null();
	}
	else {
		// int enabled 
		print_sp1sp();
		// pin output? 
		if(j & (1<<3)) print_b1sp();
		else print_b0sp(); 
		// pending? 
		if(k & (1<<3)) printf("1\n\r");
		else printf("0\n\r"); 
	}

	printf(" Check Pending RT Ints");
	if(!(i & (1<<2))) print_dd0n(); // int disabled 
	else print_dd1n();				// int enabled 

	printf(" Check Pending MT Ints");
	if(!(i & (1<<1))) print_dd0n(); // int disabled 
	else print_dd1n();				// int enabled 

	printf(" Check Pending BC Ints");
	if(!(i & (1<<0))) print_dd0n(); // int disabled 
	else print_dd1n();				// int enabled 

	print_line();
	printf("Note: Reading the HW Pending Interrupt Register clears it!\n\r");
	printf("Press 'M' for menu, or press any valid menu key.   ");

	// return value read from the HW Pending Interrupt Register, now cleared 
	return k;

}	// end list_hw_ints_console()


//---------------------------------------------------------------
//  this function lists remote terminal interrupt 
//	configuration to the console via UART. if there
//	are pending RT interrupts, these too are displayed.
//	Function returns the value read from the RT pending
//	interrupt register, since reading the reg clears it.
//---------------------------------------------------------------
unsigned short int list_rt_ints_console (void) {
  
        unsigned short k=0;

	#if(!(RT1_ena||RT2_ena)) 
	printf("\n\rNeither Remote Terminal Is Enabled!\n\n\r"); 
	#else // (RT1_ena||RT2_ena)
        
            unsigned short i,j;
    

            extern const H6130 pH6130; // HI-6130 register struct base address  

            
            // formfeed 
            putchar(12); 	
    

                    i = pH6130->RT_INT_ENABLE_REG;
                    j = pH6130->RT_INT_OUTPUT_ENABLE_REG;
                    k = pH6130->RT_PENDING_INT_REG;

                    
            printf("\n\r Remote Terminal Ints  Enabled?   Pin Output?   Pending?\n\r");
            print_line();
    
            printf("RT2  Reset RT mode cmd");
            if(!(i & (1<<15))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<15)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<15)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT2  Index Equals Zero");
            if(!(i & (1<<14))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<14)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<14)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT2    Illegal Command");
            if(!(i & (1<<13))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<13)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<13)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT2 Broadcast Msg Rcvd");
            if(!(i & (1<<12))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<12)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<12)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT2 MessageErrorStatus");
            if(!(i & (1<<11))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<11)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<11)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT2  Int When Accessed");
            if(!(i & (1<<10))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<10)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<10)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("        Reserved Bit 9");
            print_dddn();
    
            printf("RT1  Reset RT mode cmd");
            if(!(i & (1<<8))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<8)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<8)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT1  Index Equals Zero");
            if(!(i & (1<<7))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<7)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<7)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT1    Illegal Command");
            if(!(i & (1<<6))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<6)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<6)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT1 Broadcast Msg Rcvd");
            if(!(i & (1<<5))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<5)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<5)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT1 MessageErrorStatus");
            if(!(i & (1<<4))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<4)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<4)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("RT1  Int When Accessed");
            if(!(i & (1<<3))) {	
                    // int disabled 
                    print_null();
            }
            else {
                    // int enabled 
                    print_sp1sp();
                    // pin output? 
                    if(j & (1<<3)) print_b1sp();
                    else print_b0sp(); 
                    // pending? 
                    if(k & (1<<3)) printf("1\n\r");
                    else printf("0\n\r"); 
            }
    
            printf("     Reserved Bits 2-0");
            print_dddn();
            print_line();
            printf("Note: Reading the RT Pending Interrupt Register clears it!\n\r");
            
	#endif // (RT1_ena||RT2_ena)
	print_menuprompt();

	// return value read from the RT Pending Interrupt Register, now cleared 
	return k;

}	// end list_rt_ints_console()




//---------------------------------------------------------------------------
//   brief	this function checks for keyboard input and
//		decodes it, acts on it, when it occurs
//---------------------------------------------------------------------------
void chk_key_input(void) {

    char key;

    
    // poll USART Channel Status Reg for Rx Ready 
    if(USART_IsRxReady(BOARD_USART_BASE)) {
        // got key press at computer keyboard,
        // read USART Receive Holding register
        key = USART_GetChar(BOARD_USART_BASE);
      
        //-----------------------------------
        
        if (watch) {
            // Memory Watch mode enabled

          
        } // end if(watch)
        
        else { // Memory Watch mode disabled, normal menu mode...
          
            switch(key) {
                
                case 'm':
                case 'M':
                    watch = 0;
                    show_menu();
                break;
                
                // the W keys are only recognized if HI-6131

                case 'w':
                case 'W':
                    
                // these keys are only recognized in Memory Watch mode
                case 'r':
                case 'R':
                case 'u':
                case 'U':
                case 'd':
                case 'D':
                case 'a':
                case 'A':
                    // formfeed
                    putchar(12); 	
                    printf("\n\r Invalid Choice. Press 'M' for menu, or press any valid menu key. >> ");
                break;
                      
                 #if(BC_ena) 
                      case '1':	
                          // Generate a BC external trigger pulse for the HI-613x
                          bc_trigger();
                          // message completion delay, 1.5ms
                          Delay_us(1500);
                          // console output reg and RAM info for last completed msg
                          bc_last_msg_console();
                      break;
              
                      case '2': 		
                          list_bc_config();
                      break;
                  
                      case '3': 
                          // output Cond Code / GP Flag reg to console via UART
                          list_bc_ccgpf_reg();
                      break;
                      
                      case '7':
                          // display bc interrupt status
                          list_bc_ints_console();
                      break;
                      
                  #endif // (BC_ena) 
                  
                  #if(SMT_ena || IMT_ena)
                      case '4': 		
                          list_mt_config();
                      break;
      
                      case '5': 		
                          mt_last_msg_console();
                      break;
                  #endif // (SMT_ena || IMT_ena)
                  
                  case '6':
                      // display hw interrupt status
                      list_hw_ints_console();
                  break;
                  
                  #if(RT1_ena || RT2_ena)
                      case '8':
                          // display rt interrupt status
                          list_rt_ints_console();
                      break;
                  #endif // (RT1_ena || RT2_ena)
                  
                  #if(SMT_ena || IMT_ena)
                      case '9':
                          // display mt interrupt status
                          list_mt_ints_console();
                      break;
                  #endif // (SMT_ena || IMT_ena)
                  
                  
                  default:
                      // ignore unrecognized keys
                      // formfeed
                      putchar(12); 	
                      printf("\n\r Invalid Choice. Press 'M' for menu, or press any valid menu key. >> ");
                  break;
                  
            } // end switch(key)
            
            // prevent warning: waddr as declared but never referenced
            waddr = waddr;
                                          
        }  // end else !watch
        
    }  // end if(USART_IsRxReady(BOARD_USART_BASE))
                    
} // end chk_key_input()
                    
                                                                                                 
                                       
//----------------------------------------------------------------------------
//    used by above function, this function converts a char 0-15 into 0x0-0xF
//----------------------------------------------------------------------------
unsigned char ascii2int(char ch) {
    unsigned char hex;
    if((ch >='0') && (ch<='9')) hex = ch - '0';
    else if ((ch >='A') && (ch<='F')) hex = ch - 'A' + 10;
    else if ((ch >='a') && (ch<='f')) hex = ch - 'a' + 10;
    else hex = 0;
    return hex;
}
                                       
                                       
                                       
                                       
                                       
//-----------------------------------------------------------------
//    these functions give printf for text strings used
//    many times. reduces ROM needed for text storage
//-----------------------------------------------------------------
void print_null(void) {
	printf("    0           -           -\n\r");
}


void print_sp1sp(void) {
	printf("    1           ");
}


void print_b1sp(void) {
	printf("1           ");
}


void print_b0sp(void) {
	printf("0           ");
}


void print_dddn(void) {
	printf("    -           -           -\n\r");
}


void print_dd0n(void) {
	printf("    -           -           0\n\r");
}


void print_dd1n(void) {
	printf("    -           -           1\n\r");
}

void print_menuprompt(void) {
	printf("Press 'M' for menu, or press any valid menu key. >> ");
}

void print_line(void) {
	printf("===========================================================\n\r");
}


