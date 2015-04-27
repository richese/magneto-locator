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
// buttons.c
//
//
//
//------------------------------------------------------------------------------
#include "buttons.h"

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"


static u8 buttons_state = BUTTONS_MASK;


void buttons_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	ROM_GPIODirModeSet(GPIO_PORTF_BASE, BUTTONS_MASK, GPIO_DIR_MODE_IN);
	MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, BUTTONS_MASK, GPIO_STRENGTH_2MA,
	                     GPIO_PIN_TYPE_STD_WPU);

	buttons_state = ROM_GPIOPinRead(GPIO_PORTF_BASE, BUTTONS_MASK);
}

u8 buttons_poll(u8 *delta, u8 *state)
{
	u32 current_state = (ROM_GPIOPinRead(GPIO_PORTF_BASE, BUTTONS_MASK));
	if (state) {
		*state = (u8) ~current_state;
	}

	u32 current_delta = current_state ^ buttons_state;

	static u8 switch_clock_a = 0;
	static u8 switch_clock_b = 0;
	switch_clock_a ^= switch_clock_b;
	switch_clock_b = ~switch_clock_b;
	switch_clock_a &= current_delta;
	switch_clock_b &= current_delta;

	buttons_state &= switch_clock_a | switch_clock_b;
	buttons_state |= (~(switch_clock_a | switch_clock_b)) & current_state;

	current_delta ^= (switch_clock_a | switch_clock_b);

	if (delta) {
		*delta = (u8) current_delta;
	}

	return (~buttons_state);
}
