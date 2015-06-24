//------------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Mandák <lukas.mandak@yandex.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
// uart.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_UART_H_
#define _COLLECT_UART_H_

#include "intdef.h"


//#define UART_BAUD_RATE 		(9600)
#define UART_BAUD_RATE 		(115200)


//!
//! Initialize UART module.
//!
//! Required for io.h functions to work correctly.
//!
void uart_init(void);

//!
//! Write one character to UART output buffer.
//!
//! \param c Character to write to output buffer.
//!
void uart_write(char c);

//!
//! Read one character from UART input buffer.
//!
//! \return Character at top of input buffer or -1 if no characters are present.
//!
char uart_read(void);


#endif // _COLLECT_UART_H_
