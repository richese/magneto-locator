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
// io.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_IO_H_
#define _COLLECT_IO_H_

#include "uart.h"

//!
//! Print one character.
//!
//! \param c
//!
inline void Putchar(char c) { uart_write(c); }

//!
//! Print string.
//!
//! \param s Pointer to null terminated string.
//!
void Puts(char* s);

//!
//! Print formatted output.
//!
//! Availible formatters:
//!  * %c - char
//!  * %s - null terminated string
//!  * %i - i16
//!  * %u - u16
//!  * %l - i32
//!  * %n - u32
//!  * %x - u16
//!
void Printf(char* format, ...);


#endif // _COLLECT_IO_H_
