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
// uart.c
//
//
//
//------------------------------------------------------------------------------
#include "uart.h"

#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"

#include "intdef.h"


void uart_init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/*
	MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
	MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
	MAP_GPIOPinConfigure(GPIO_PA0);
	MAP_GPIOPinConfigure(GPIO_PA1);
	*/

	MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	MAP_UARTConfigSetExpClk(UART0_BASE, MAP_SysCtlClockGet(), UART_BAUD_RATE,
	                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                        UART_CONFIG_PAR_NONE));
}

void uart_write(char c)
{
	MAP_UARTCharPut(UART0_BASE, c);
}

char uart_read(void)
{
	if (MAP_UARTCharsAvail(UART0_BASE))
		return MAP_UARTCharGet(UART0_BASE)&0xff;

	return -1;
}
