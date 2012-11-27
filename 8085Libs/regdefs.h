/*
* HI-6130 Register Bit Definitions
* regdefs.h
*
* Phillip Johnston
* 8 October 2012
*/

#ifndef _HI6130_REGDEFS_H
#define _HI6130_REGDEFS_H

//------------------------------------------------------------------------------
//   Macro Definitions for HI-613x Registers
//------------------------------------------------------------------------------
//
// Macros for HI-613x Master Configuration Register - Read/Write
//
#define TXINHA 1<<15 // inhibit transmitter A
#define TXINHB 1<<14 // inhibit transmitter B
#define BCSTRT 1<<13 // BC start
#define BCENA 1<<12 // BC enable
#define ENA_MAP1 0x0<<10 // HI-6131 SPI only: enable memory addr pointer 1
#define ENA_MAP2 0x1<<10 // HI-6131 SPI only: enable memory addr pointer 2
#define ENA_MAP3 0x2<<10 // HI-6131 SPI only: enable memory addr pointer 3
#define ENA_MAP4 0x3<<10 // HI-6131 SPI only: enable memory addr pointer 4
#define SSR8 1<<9 // HI-6130 bus only: single-strobe 8-bit bus read
#define MTENA 1<<8 // MT / IMT enable 
#define RT2ENA 1<<7 // RT2 enable 
#define RT1ENA 1<<6 // RT1 enable 
#define RT2STEX 1<<5 // RT2 start execution 
#define RT1STEX 1<<4 // RT1 start execution 
#define BSDTXO 1<<3 // mode code 4,20 bus shutdown kills transmit only (receive still works)
#define PULSE_INT 0<<2 // use neg-pulse interrupts, no acknowledge needed
#define LEVEL_INT 1<<2 // interrupts are continuous low level until acknowledged
#define IMTA 1<<1 // if MT enabled, ACTIVE output will be high when
// MT is monitoring a message, otherwise ACTIVE only
// indicates activity for BC, RT1 and RT2 

//
// Macros for HI-613x Master Status & Reset Register 
//
#define READY 1<<15 // READY goes high after auto-initialization or built-in test completion
#define AUTOEN 1<<13 // enables auto-initialization after /Master Reset.
#define MTRESET 1<<12 // resets the bus monitor if enabled, then self-resets
#define RT2RESET 1<<11 // resets remote terminal 2 if enabled, then self-resets
#define RT1RESET 1<<10 // resets remote terminal 1 if enabled, then self-resets
#define BCMIP 1<<9 // BC messsage in process
#define BCACTIVE 1<<8 // when BC is enabled & running, high during msg processing or programmed delays
#define MTMIP 1<<7 // MT messsage in process
#define IMTPIP 1<<6 // IRIG-106 MT packet in process
#define RT2MIP 1<<5 // RT2 messsage in process
#define RT1MIP 1<<4 // RT1 messsage in process
#define RT2INH 1<<3 // RT2 has a bus inhibited by "bus shutdown" mode code 4 or 20
#define RT1INH 1<<2 // RT1 has a bus inhibited by "bus shutdown" mode code 4 or 20
#define EECKE 1<<1 // during auto-initialization, a checksum error occurred
#define RAMIF 1<<0 // during auto-initialization, RAM/EEPROM mismatch error


//
// Macros for HI-613x Time Tag Counter Configuration Register
//
#define MTTAG_CLR 1<<14 // reset MT time tag count to zero
#define MTTAG_LOAD 2<<14 // load MT time tag count from MT Time Tag Utility reg(s)
#define MTTAG_CAP 3<<14 // capture MT time tag count to MT Time Tag Utility reg(s)
#define BTTAG_CLR 1<<12 // reset BC time tag count to zero
#define BTTAG_LOAD 2<<12 // load BC time tag count from BC Time Tag Utility reg(s)
#define R2TTAG_CLR 1<<10 // reset RT2 time tag count to zero
#define R2TTAG_LOAD 2<<10 // load RT2 time tag count from RT2 Time Tag Utility reg
#define R1TTAG_CLR 1<<8 // reset RT1 time tag count to zero
#define R1TTAG_LOAD 2<<8 // load RT1 time tag count from RT1 Time Tag Utility reg
#define MTTAG_OFF 0 // MT time tag clock is disabled, no counting
#define MTTAG_PIN 1<<4 // MT time tag clock is MTTCLK input pin
#define MTTAG_2U 2<<4 // MT time tag counter uses internally generated 2us clock
#define MTTAG_4U 3<<4 // MT time tag counter uses internally generated 4us clock
#define MTTAG_8U 4<<4 // MT time tag counter uses internally generated 8us clock
#define MTTAG_16U 5<<4 // MT time tag counter uses internally generated 16us clock
#define MTTAG_32U 6<<4 // MT time tag counter uses internally generated 32us clock
#define MTTAG_64U 7<<4 // MT time tag counter uses internally generated 64us clock
#define MTTAG_100N 8<<4 // MT time tag counter uses internally generated 100ns clock
#define BTTAG32 1<<3 // BC time tag counter is 32 bits
#define BTTAG16 0<<3 // BC time tag counter is 16 bits
#define TTAG_PIN 1<<4 // BC & RT time tag clock is TTCLK input pin
#define TTAG_2U 2<<0 // BC & RT time tag counter uses internally generated 2us clock
#define TTAG_4U 3<<0 // BC & RT time tag counter uses internally generated 4us clock
#define TTAG_8U 4<<0 // BC & RT time tag counter uses internally generated 8us clock
#define TTAG_16U 5<<0 // BC & RT time tag counter uses internally generated 16us clock
#define TTAG_32U 6<<0 // BC & RT time tag counter uses internally generated 32us clock
#define TTAG_64U 7<<0 // BC & RT time tag counter uses internally generated 64us clock

// NOTE: for SMT simple bus monitor, 16- or 48-bit time tag is selected by MT Configuration reg bit 1 !!

//
// Macros for HI-613x Hardware Interrupt Registers: 
// Interrupt Enable Reg, Interrupt Output Enable Reg & Pending Interrupt Reg 
//
#define HSPI_INT 1<<15 // HI-6131 only: host SPI gave wrong # of SCK clocks
#define EECKE_INT 1<<14 // EEPROM checksum error
#define RAMIF_INT 1<<13 // RAM initialization fail: mismatches EEPROM
#define LBFA 1<<12 // bus A loopback failure
#define LBFB 1<<11 // bus B loopback failure 
#define MTTTRO 1<<10 // MT timetag counter rollover from full count to zero
#define BCTTRO 1<<9 // BC timetag counter rollover from full count to zero
#define RT2TTM 1<<8 // RT2 timetag count matches value in RT2 Timetag Utility reg
#define RT1TTM 1<<7 // RT1 timetag count matches value in RT1 Timetag Utility reg
#define MTTTM 1<<6 // MT timetag count matches value in MT Timetag Utility reg(s)
#define BCTTM 1<<5 // BC timetag count matches value in BC Timetag Utility reg(s)
#define RT2APF 1<<4 // RT2 address parity error (in RT2 Operational Status reg)
#define RT1APF 1<<3 // RT1 address parity error (in RT1 Operational Status reg)

// next 3 only apply to Hardware Pending Interrupt Register
#define RTIP 1<<2 // RT interrupt pending
#define MTIP 1<<1 // MT interrupt pending
#define BCIP 1<<0 // BC interrupt pending


//-------------------------------------------------------------------------------------------

//
// Macros for HI-613x Bus Controller Configuration Register
//
#define BCTO_15U 0<<14 // "no response timeout" has 15us bus dead time
#define BCTO_20U 1<<14 // "no response timeout" has 20us bus dead time
#define BCTO_58U 2<<14 // "no response timeout" has 18us bus dead time
#define BCTO_138U 3<<14 // "no response timeout" has 138us bus dead time
#define CHKCWFMT 1<<13 // enable msg format error checking (see data sheet)
#define BCRE 1<<12 // BC retry enable
#define BC2RE 1<<11 // BC second retry enable
#define BCR1A 1<<10 // BC first retry use alternate bus
#define BCR2A 1<<9 // BC second retry use alternate bus
#define RESBS 1<<8 // BC retry if RT status bit set
#define MENDV 1<<7 // tx msg is valid if RT gives Message Error status without sending data
#define BSYNDV 1<<6 // tx msg is valid if RT gives Busy status without sending data
#define BCGTE 1<<5 // BC gap timer enable
#define BCWDTE 1<<4 // BC watchdog timer enable
#define TTSYNEN 1<<3 // for mode code 17, transmit (low) 16-bits of BC TTAG count as data
#define ETTSYN 1<<2 // enhanced time tag sync for mode code 17 (see data sheet)
#define BCGCE 1<<1 // BC gap check enable
#define BCRME 1<<0 // BC Broadcast mask enable


// Macros for HI-613x Bus Controller Interrupt Registers
// (Int Enable Reg, Int Output Enable Reg & Pending Int Reg)
//
#define BCWDT 1<<15 // BC frame timer expired
#define SELMSG 1<<14 // completion of a 1553 msg with EOM set in msg block Control Word
#define BCGPQ 1<<13 // BC general pupose queue rollover
#define BCRETRY 1<<12 // occurence of a retried 1553 message
#define CSTKERR 1<<11 // BC call stack pointer error
#define BCTRAP 1<<10 // BC trap interrupt, illegal op code 
#define STATSET 1<<9 // BC status set (see data sheet)
#define BCIRQMASK 15<<5 // logical AND mask for 4-bit interrupt request bits
#define BCMERR 1<<4 // message error status received from RT
#define BCEOM 1<<3 // end-of-message interrupt 


//-------------------------------------------------------------------------------------------

//
// Macros for HI-613x Remote Terminal RT1 & RT2 Configuration Registers
//
#define RTTO_15U 0<<14 // RT-RT receive "no response timeout" has 15us bus dead time
#define RTTO_20U 1<<14 // RT-RT receive "no response timeout" has 20us bus dead time
#define RTTO_58U 2<<14 // RT-RT receive "no response timeout" has 18us bus dead time
#define RTTO_138U 3<<14 // RT-RT receive "no response timeout" has 138us bus dead time
#define RTINH_A 1<<13 // RT inhibit Bus A as defined by BSDTXO bit in register 0
#define RTINH_B 1<<12 // RT inhibit Bus B as defined by BSDTXO bit in register 0
#define BCASTINV 1<<11 // when high, this bit renders broadcast commands invalid for this RT
#define DBCENA 1<<10 // dynamic bus control enable for this RT
#define UMCINV 1<<9 // undefined mode commands invalid
#define NOTICE2 1<<8 // 1553 Notice 2 storage of broadcast command data
#define SMCP 1<<7 // simplified mode command processing
#define TRXDB 1<<6 // temporary receive buffer enable
#define ALTBITW 1<<5 // use alternate BIT word for mode code 19
#define HOST_SHUTDN 1<<4 // host does bus shutdown for mode codes 4,20
#define AUTO_SHUTDN 0<<4 // automatic bus shutdown for mode codes 4,20
#define HOSTSYNC 0<<2 // host performs synchronize for mode code 17
#define ASYNCDB0 1<<2 // device performs mode code 17 sync, loading data to timetag if data bit 0 = 0
#define ASYNCDB1 2<<2 // device performs mode code 17 sync, loading data to timetag if data bit 0 = 1
#define AUTO_SYNC 3<<2 // device performs mode code 17 sync, unconditionally loading data to timetag
#define MC16OPT 1<<1 // mode code command 16 option (see data sheet)
#define HOST_MC8_RESET 1 // host reset for mode code command 8 (see data sheet)
#define AUTO_MC8_RESET 0 // automatic soft reset for mode code command 8 (see data sheet)

//
// Macros for HI-613x Remote Terminal RT1 & RT2 Operational Status Registers
//
#define RTAP 1<<10 // RT address parity bit
#define LOCK 1<<9 // Terminal address bits lock (write prevented)
#define MCND 1<<5 // current (last) command was a mode command, no data (transmit MC0-MC15)
#define MCRD 1<<4 // current (last) command was a mode command, w/ data (receive MC16-MC31)
#define MCTD 1<<3 // current (last) command was a mode command, w/ data (transmit MC16-MC31)
#define RTAPF 1<<2 // interrupt for RT address parity error

//
// Macros for HI-613x Remote Terminal RT1 & RT2 descriptor table Control Words
//
#define IXEQZ 0x8000 // interrupt when index equals zero
#define IWA 0x4000 // interrupt whenever accessed
#define IBRD 0x2000 // interrupt when broadcast command
#define MKBUSY 0x1000 // make busy
#define DBAC 0x0800 // data block accessed
#define DPA 0x0400 // data pointer A flag
#define BCAST 0x0200 // broadcast flag
#define PPON 0x0100 // ping-pong enable status
#define STOPP 0x0008 // stop ping-pong request
#define PINGPONG 0x0004 // ping-pong mode select
#define CIR2_2MSG 0x0032 // circular mode 2 select, 2 messages
#define CIR2_4MSG 0x0042 // circular mode 2 select, 4 messages 
#define CIR2_8MSG 0x0052 // circular mode 2 select, 8 messages 
#define CIR2_16MSG 0x0062 // circular mode 2 select, 16 messages 
#define CIR2_32MSG 0x0072 // circular mode 2 select, 32 messages 
#define CIR2_64MSG 0x0082 // circular mode 2 select, 64 messages 
#define CIR2_128MSG 0x0092 // circular mode 2 select, 128 messages 
#define CIR2_256MSG 0x00A2 // circular mode 2 select, 256 messages 
#define CIR2_512MSG 0x00B2 // circular mode 2 select, 512 messages 
#define CIRC1 0x0001 // circular mode 1 select
#define INDEX 0x0000 // index mode select

//
// Macros for HI-613x Remote Terminal RT1 & RT2 1553 Status Word Bits Registers
//
#define TXANDCLR 1<<15 // transmit once then clear 
#define INST 1<<9 // instrumentation
#define SVCREQ 1<<8 // service request
#define BUSY 1<<3 // busy
#define SUBSYSTEM 1<<2 // subsystem flag
#define TERMFLAG 1<<0 // terminal flag bit


//
// Macros for HI-613x Remote Terminal Interrupt Registers
// (Int Enable Reg, Int Output Enable Reg & Pending Int Reg)
//
#define RT2_MC8 1<<15 // mode code command 8, "reset RT"
#define RT2_IXEQZ 1<<14 // interrupt when index equals zero
#define RT2_ILCMD 1<<13 // interrupt for illegalized command
#define RT2_IBR 1<<12 // interrupt when broadcast command
#define RT2_MERR 1<<11 // interrupt for message error
#define RT2_IWA 1<<10 // interrupt whenever accessed

#define RT1_MC8 1<<8 // mode code command 8, "reset RT"
#define RT1_IXEQZ 1<<7 // interrupt when index equals zero
#define RT1_ILCMD 1<<6 // interrupt for illegalized command
#define RT1_IBR 1<<5 // interrupt when broadcast command
#define RT1_MERR 1<<4 // interrupt for message error
#define RT1_IWA 1<<3 // interrupt whenever accessed

//
// Macros for HI-613x Remote Terminal RT1 & RT2 BIT (Built-In Test) Word Registers
//
#define TXSDA 1<<15 // Bus A transmitter shut down by MC4 or MC21
#define TXSDB 1<<14 // Bus B transmitter shut down by MC4 or MC21
#define RXSDA 1<<13 // Bus A receiver shut down by MC4 or MC21
#define RXSDB 1<<12 // Bus B receiver shut down by MC4 or MC21
#define BLBFA 1<<5 // BIST Bus A loopback failure
#define BLBFB 1<<4 // BIST Bus B loopback failure
#define BMTF 1<<3 // BIST memory test failure
#define RTAPE 1<<2 // RT address parity error 
#define EEPROMFAIL 1<<1 // EEPROM load fail flag
#define TFLAGINH 1<<0 // Terminal Flag (status) Bit inhibited by MC6


//-------------------------------------------------------------------------------------------

//
// Macros for HI-613x Bus Monitor Configuration Register
// 
// These 3 options apply only to Simple Monitor 
#define SELECT_SMT 1 // select mode: Simple Monitor enabled
#define SMT_TTAG48 1<<1 // SMT ONLY: 48-bit time tagging
#define SMT_TTAG16 0<<1 // SMT ONLY: 16-bit time tagging

// These options apply only to IRIG-106 Monitor 
#define SELECT_IMT 0 // select mode: IRIG-106 Monitor enabled
#define IMT_HDR_OFF 1<<3 // IMT ONLY: disable packet header/trailer generation 
#define IMT_HDR_ON 0<<3 // IMT ONLY: enable packet header/trailer generation
#define IMT_CKSUM_ON 1<<1 // IMT ONLY: enable data packet checksum
#define IMT_CKSUM_OFF 0<<1 // IMT ONLY: disable data packet checksum
#define IMT_DTYPE9 0<<10 // IMT ONLY: packet header "data type" specifier
#define IMT_DTYPE4 1<<10 // IMT ONLY: 
#define IMT_DTYPE5 2<<10 // IMT ONLY: 
#define IMT_DTYPE7 3<<10 // IMT ONLY: 
#define PKTSTRT_CW 0<<7 // IMT ONLY: packet timer starts at cmd word detection
#define PKTSTRT_ENA 1<<7 // IMT ONLY: packet timer starts immediately when MT enabled
#define IMT_PKTSTOP 1<<13 // IMT ONLY: BC asserts this bit to end in-process packet 

// These options apply to BOTH Simple & IRIG-106 Monitor 
#define EXTD_STATUS 1<<2 // enable extended error and status flags, NOT FOR IMT!
#define IRIG_STATUS 0<<2 // enable IRIG-106 compatible error and status flags
#define MTTO_15U 0<<14 // "no response timeout" has 15us bus dead time
#define MTTO_20U 1<<14 // "no response timeout" has 20us bus dead time
#define MTTO_58U 2<<14 // "no response timeout" has 18us bus dead time
#define MTTO_138U 3<<14 // "no response timeout" has 138us bus dead time
#define GAPCHKON 1<<12 // enables 2us bus dead time check, RT response & intermessage
#define GAPCHKOFF 0<<12 // disables 2us bus dead time check, RT response & intermessage
#define TAG_LWLB 0<<8 // time tag at last word, last bit in msg  
#define TAG_FWFB 1<<8 // time tag at first word, first bit in msg  
#define TAG_FWLB 2<<8 // time tag at first word, last bit in msg  
#define TAG_OFF 3<<8 // time tag disabled, all stored tags = 0
#define REC_CW 0<<5 // full cmd word starts new msg recording
#define REC_CWDW 1<<5 // full cmd or data word starts new msg recording
#define REC_CS 2<<5 // cmd sync + 2 data bits starts new msg recording
#define REC_CSDS 3<<5 // cmd or data sync + 2 data bits starts new msg recording
#define STOR_INVWD 1<<4 // invalid data words are stored, recording continues
#define STOP_INVWD 0<<4 // invalid data word ends msg/fragment record, bad word not stored


//
// Macros for HI-613x Bus Monitor Interrupt Management Registers
// (Int Enable Register, Int Output Enable Register & Pending Int Register)
// 
// These interrupts apply to BOTH Simple & IRIG-106 Monitor 
#define STKROVR 1<<8 // IMT or SMT: (command) stack end-address written
#define STKADRSS 1<<6 // IMT or SMT: (cmd) stack address written equals word 3 in MT Addr List
#define MT_EOM 1<<3 // IMT or SMT: message completion

// These interrupts apply only to IRIG-106 Monitor 
#define FULL_EOP 1<<15 // IMT ONLY: stack full end-of-packet
#define MAXWORDS 1<<14 // IMT ONLY: max 1553 msg word count attained
#define MAXMSGS 1<<13 // IMT ONLY: max msg count attained
#define MAXGAP 1<<12 // IMT ONLY: max gap time exceeded
#define MAXTIME 1<<11 // IMT ONLY: max recording time exceeded
#define HPKTSTOP 1<<10 // IMT ONLY: host asserted packet stop, now stopped
#define PKTREADY 1<<9 // IMT ONLY: packet ready, other bit says why
#define FULL_OFS 1<<5 // IMT ONLY: offset from FULL_EOP interrupt
// plus STKROVR above // IMT or SMT: (command) stack end-address written
// plus STKADRSS above // IMT or SMT: stack fixed address level attained
// plus MT_EOM above // IMT or SMT: message completion

// These interrupts apply only to Simple Monitor 
#define DSTKROVR 1<<7 // SMT ONLY: data stack end-address written 
#define DSTKADRSS 1<<5 // SMT ONLY: data stack address written equals word 7 in MT Addr List
#define MSG_ERR 1<<4 // SMT ONLY: RT status word detected with Message Error
// plus STKROVR above // IMT or SMT: (command) stack end-address written
// plus STKADRSS above // IMT or SMT: stack fixed address level attained
// plus MT_EOM above // IMT or SMT: message completion


//
// Boolean States
//
#define False ((char)0)
#define True ((char)1)


#endif //_HI6130_REGDEFS_H
