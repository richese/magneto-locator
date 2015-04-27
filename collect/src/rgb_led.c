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
// rgb_led.c
//
//
//
//------------------------------------------------------------------------------
#include "rgb_led.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"


static struct {
    color_t     c;
    intensity_t i;
} g_rgb_data;


void rgb_init(void)
{
	/* Set PWM clock to system clock */
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	/* Make sure thet all required peripherals are enabled */
	if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
		MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1))
		MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	/* Configure pins to PWM output function */
	MAP_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	MAP_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	MAP_GPIOPinConfigure(GPIO_PF3_M1PWM7);

	MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
	MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
	MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);

	/* Configure PWM module */
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN |
						PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN |
						PWM_GEN_MODE_GEN_SYNC_LOCAL);


	MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, RGB_PWM_PERIOD);
	MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, RGB_PWM_PERIOD);

	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0);
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0);
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0);

	//PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	MAP_PWMSyncTimeBase(PWM1_BASE, PWM_GEN_2_BIT|PWM_GEN_3_BIT);

	g_rgb_data.c = 0;
	g_rgb_data.i = 0;
}

void rgb_set_color(color_t color)
{
	u16 red_pulse   = ((color&0x00FF0000)>>16) * g_rgb_data.i * 12;
	u16 green_pulse = ((color&0x0000FF00)>>8)  * g_rgb_data.i * 12;
	u16 blue_pulse  =  (color&0x000000FF)      * g_rgb_data.i * 12;

	if (red_pulse) {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red_pulse);
	} else {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
	}

	if (blue_pulse) {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue_pulse);
	} else {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
	}

	if (green_pulse) {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green_pulse);
	} else {
		MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
	}

	MAP_PWMSyncUpdate(PWM1_BASE, PWM_GEN_2_BIT|PWM_GEN_3_BIT);

	g_rgb_data.c = color;
}

void rgb_set_led(u8 led, intensity_t intensity)
{
	if (led & LED_RED) {
		g_rgb_data.c &= ~(0xFF0000);
		g_rgb_data.c |= (intensity<<16);
	}
	if (led & LED_GREEN) {
		g_rgb_data.c &= ~(0x00FF00);
		g_rgb_data.c |= (intensity<<8);
	}
	if (led & LED_BLUE) {
		g_rgb_data.c &= ~(0x0000FF);
		g_rgb_data.c |= intensity;
	}

	rgb_set_color(g_rgb_data.c);
}

void rgb_set_intensity(intensity_t intensity)
{
	g_rgb_data.i = intensity;
	rgb_set_color(g_rgb_data.c);
}

color_t rgb_to_color(component_t r, component_t g, component_t b)
{
	return (color_t) (r<<16)|(g<<8)|b;
}

