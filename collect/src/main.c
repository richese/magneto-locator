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
// main.c
//
//
//
//------------------------------------------------------------------------------

//
// Standard C library includes.
//
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

//
// Peripheral/system driver includes.
//
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

//
// Project includes.
//
#include "buttons.h"
#include "io.h"
#include "intdef.h"
#include "magneto.h"
#include "rgb_led.h"
#include "timer.h"
#include "uart.h"


static void error_loop(i32 errno);


int main(void)
{
	//
	// Set main clock to 50MHz.
	//
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
					   SYSCTL_OSC_MAIN);

	//
	// Initialize peripherals.
	//
	buttons_init();
	rgb_init();
	timer_init();
	uart_init();

	//
	// Constant definition.
	//
	static const int kMagCount = 3;
	static const u32 kMagBases[] = {I2C0_BASE, I2C1_BASE, I2C2_BASE};
	static const time_t kSendPeriod = 1000; // 10Hz

	//
	// Variable definition.
	//
	int i = 0;
	uint8_t button_change = 0;
	uint8_t button_state = 0;

	int active = 0;
	time_t time_next = 0;
	struct mag_data data[3];

	//
	// Initialize communication with compass modules.
	//
	for (i = 0; i < kMagCount; ++i) {
		if (magneto_init(kMagBases[i]) != 0)
			error_loop(i);
	}

	//
	// Dimm led light intensity.
	//
	rgb_set_intensity(0x01);

	//
	// Enter main loop.
	//
	while (1) {
		//
		// Poll buttons state.
		//
		buttons_poll(&button_change, &button_state);

		//
		// Toggle active/inactive state if left button was pressed.
		//
		if (BUTTON_PRESSED(LEFT_BUTTON, button_state, button_change))
			active = active ? 0 : 1;

		//
		// Indicate active state by turning on red led.
		//
		if (active)
			rgb_set_led(LED_RED, LED_ON);
		else
			rgb_set_led(LED_RED, LED_OFF);

		//
		// Periodicaly read magnetometer data ten times per second.
		//
		if (time_next <= timer_get_time()) {
			time_next += kSendPeriod;

			//
			// Read measured data from magnetometers.
			//
			for (i = 0; i < kMagCount; ++i)
				magneto_read_data(kMagBases[i], &data[i]);

			//
			// Send measured data if in active mode.
			//
			if (active) {
				for (i = 0; i < kMagCount; ++i)
					Printf("%i %i %i ", data[i].x,
					                    data[i].y,
					                    data[i].z);
				Putchar('\r');
				Putchar('\n');
			}
		}
	}
}

static void error_loop(i32 errno)
{
	time_t time = timer_get_time();

	//
	// Turn on blue led to indicate failure.
	//
	rgb_set_intensity(INTENSITY_QUARTER);
	rgb_set_led(LED_BLUE, LED_ON);

	//
	// Enter infinite loop.
	//
	while (1) {
		//
		// Report error number every 5 seconds.
		//
		if (time <= timer_get_time()) {
			time += 50000;
			Printf("Error occured: %l\r\n", errno);
		}
	}
}
