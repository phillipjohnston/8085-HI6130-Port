/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \unit
/// This file contains several function re-implementations to make the standard
/// printf() method output on USART0.
///
/// The re-implemented functions are fputc(), fputs() and putchar().
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <usart/usart.h>

#include <stdio.h>
#include "board_6130.h"

#ifndef NOFPUT
//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Implementation of fputc using USART0 as the standard output. Required
/// for printf().
/// Returns the character written if successful, or -1 if the output stream is
/// not stdout or stderr.
/// \param c  Character to write.
/// \param pStream  Output stream.
//------------------------------------------------------------------------------
signed int fputc(signed int c, FILE *pStream)
{
    if ((pStream == stdout) || (pStream == stderr)) {
        USART_Write(BOARD_USART_BASE, c, 0);
        return c;
    }
    else {

        return EOF;
    }
}

//------------------------------------------------------------------------------
/// Implementation of fputs using USART0 as the standard output. Required
/// for printf().
/// Returns the number of characters written if successful, or -1 if the output
/// stream is not stdout or stderr.
/// \param pStr  String to write.
/// \param pStream  Output stream.
//------------------------------------------------------------------------------
signed int fputs(const char *pStr, FILE *pStream)
{
    signed int num = 0;

    while (*pStr != 0) {

        if (fputc(*pStr, pStream) == -1) {

            return -1;
        }
        num++;
        pStr++;
    }

    return num;
}

#undef putchar

//------------------------------------------------------------------------------
/// Outputs a character on USART0. Returns the character itself.
/// \param c  Character to output.
//------------------------------------------------------------------------------
signed int putchar(signed int c)
{
    return fputc(c, stdout);
}

#else

//------------------------------------------------------------------------------
/// Re-implement DBGU_PutChar function to outputs a character through the 
/// USART0 peripheral
/// \param c  Character to send.
//------------------------------------------------------------------------------

void DBGU_PutChar(unsigned char ch)
{ 
    USART_Write(BOARD_USART_BASE, ch, 0);
}

#endif
