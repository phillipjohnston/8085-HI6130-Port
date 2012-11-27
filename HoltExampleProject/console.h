/*---------------------------------------------------------------------------- 
 *            H O L T   I N T E G R A T E D   C I R C U I T S 
 *---------------------------------------------------------------------------- 
 * The software is delivered "AS IS" without warranty or condition of any
 * kind, either expressed, implied or statutory. This includes without
 * limitation any warranty or condition with respect to merchantability or
 * fitness for any particular purpose, or against the infringements of
 * intellectual property rights of others.
 *----------------------------------------------------------------------------
 
 File Name              : console.h
 Object                 : Function Protoypes for reference design,
                          HI-613x Evaluation Board based on the Atmel Cortex M-3
                                                 
 ----------------------------------------------------------------------------*/



//------------------------------------------------------------------------------
//     function prototypes
//------------------------------------------------------------------------------

// console functions used by all terminal modes
void ConfigureUsart1(void);
void show_menu(void);
void chk_key_input(void);
unsigned short int list_hw_ints_console(void);

// console functions used by bc mode
void bc_last_msg_console(void);
void list_bc_config (void);
void list_bc_ccgpf_reg(void);
unsigned short int list_bc_ints_console(void);

// console functions used by rt1 and/or rt2
unsigned short int list_rt_ints_console(void);

// console functions used by smt or imt modes
void list_mt_config(void);
void mt_last_msg_console(void);
unsigned short int list_mt_ints_console(void);

unsigned char ascii2int(char ch);

// primitive console functions that "printf"
// redundant char strings to reduce program size
void print_null(void);
void print_sp1sp(void);
void print_b1sp(void);
void print_b0sp(void);
void print_dddn(void);
void print_dd0n(void);
void print_dd1n(void);
void print_menuprompt(void);
void print_line(void);

