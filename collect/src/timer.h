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
// timer.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_TIMER_H_
#define _COLLECT_TIMER_H_

#include "intdef.h"

#ifndef _TIME_T_
typedef unsigned long int time_t;
#endif

//!
//! Initialize timer module.
//!
void timer_init(void);

//!
//! Returns elapsed time since timer initialization.
//!
//! \return 1x10-4 s
//!
time_t timer_get_time(void);

//!
//! Loops for given period of time.
//!
//! \param ms Number of miliseconds to loop.
//!
void timer_sleep(u32 ms);



#endif // _COLLECT_TIMER_H_
