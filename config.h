/*
* HI-6130 Operational Configuration Options
* 6130_config.h
*
* Phillip Johnston
* 6 October 2012
*/

/*******************************
* Configuration Definitions *
*******************************/
//#define USE_TEST_REGS
//#define SMCP_OK 
//For description of this, look in 6130_initialization.h

/*********************************
* Operational Mode Options *
*********************************/ 
//#define I_AM_BC
//#define I_AM_BM
//#define I_AM_RT

#ifdef I_AM_BC
#define BC_ena 1 // Bus Controller
#endif

#ifdef I_AM_RT
#define RT1_ena 1 // Remote Terminal 1
//#define RT2_ena 1 // Remote Terminal 2
#endif

#ifdef I_AM_BM
#define SMT_ena 1 // Simple Monitor Terminal   *   DO NOT set SMT_ena = IMT_ena = 1.  *
//#define IMT_ena 0 // IRIG-106 Monitor Terminal *   One or both of these 2 must be 0.  *
#endif
