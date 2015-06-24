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
// buttons.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_BUTTONS_H_
#define _COLLECT_BUTTONS_H_

#include "intdef.h"

#include "driverlib/gpio.h"


#define LEFT_BUTTON             (GPIO_PIN_4)
#define RIGHT_BUTTON            (GPIO_PIN_0)

#define BUTTONS_MASK            (LEFT_BUTTON | RIGHT_BUTTON)

#define BUTTON_PRESSED(button, buttons, changed) \
        (((button) & (changed)) && ((button) & (buttons)))

#define BUTTON_RELEASED(button, buttons, changed) \
        (((button) & (changed)) && !((button) & (buttons)))


void buttons_init(void);

u8 buttons_poll(u8 *delta, u8 *state);


#endif // _COLLECT_BUTTONS_H_
