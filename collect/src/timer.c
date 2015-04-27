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
// timer.c
//
//
//
//------------------------------------------------------------------------------
#include "timer.h"

#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"


volatile time_t _system_time_ = 0;


void timer_init(void)
{
	_system_time_ = 0;

	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, 5000);
	MAP_IntEnable(INT_TIMER0A_TM4C123);
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
}

time_t timer_get_time(void)
{
	return _system_time_;
}

void timer_sleep(u32 ms)
{
	volatile time_t time_end = 10*(time_t)ms + timer_get_time();

	while (time_end > timer_get_time())
		__asm("nop");
}

void Timer0A_Handler(void)
{
	_system_time_ += 1;

	MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}
