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
// magneto.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_MAGNETO_H_
#define _COLLECT_MAGNETO_H_

#include "intdef.h"


//!
//!
//!
struct mag_data {
	i16 x;
    i16 y;
    i16 z;
};


//!
//! Initialize magneto module connected to I²C bus.
//!
//! \param base Valid I²C peripheral module base.
//!
u32 magneto_init(u32 base);

//!
//! Read value of one magnetometer register.
//!
//! \param base Valid I²C peripheral module base.
//! \param reg Register number.
//!
u32  magneto_read_register(u32 base, u8 reg);

//!
//! Write value to magnetometer register.
//!
//! \param base Valid I²C peripheral module base.
//! \param reg Register number.
//! \param data Data to write to register.
//!
void magneto_write_register(u32 base, u8 reg, u8 data);

//!
//! Reads axis data from magnetometer and saves them to gived structure.
//!
//! \param base Valid I²C peripheral module base.
//! \param data Pointer.
//!
void magneto_read_data(u32 base, struct mag_data *data);

//!
//! Prints current content of magnetometer configuration registers.
//!
//! \param base Valid I²C peripheral module base.
//!
void magneto_print_config(u32 base);


#endif // _COLLECT_MAGNETO_H_
