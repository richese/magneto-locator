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
// magneto.c
//
//
//
//------------------------------------------------------------------------------
#include "magneto.h"

#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "HMC5883.h"
#include "io.h"


static u32 _magneto_error(u32 base)
{
	u32 err;

	do {
		err = MAP_I2CMasterErr(base);
		if (err != I2C_MASTER_ERR_NONE) {
			Printf("Error on I2C1 module. Code: 0x%x\r\n", err);
			return err;
		}
	} while (MAP_I2CMasterBusy(base));

	return 0;
}

u32 magneto_init(u32 base)
{
	switch (base) {
	case I2C0_BASE:
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

		MAP_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
		MAP_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
		MAP_GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
		MAP_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
		break;

	case I2C1_BASE:
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

		MAP_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
		MAP_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
		MAP_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
		MAP_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
		break;

	case I2C2_BASE:
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
		if (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
			MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);

		MAP_GPIOPinConfigure(GPIO_PE4_I2C2SCL);
		MAP_GPIOPinConfigure(GPIO_PE5_I2C2SDA);
		MAP_GPIOPinTypeI2CSCL(GPIO_PORTE_BASE, GPIO_PIN_4);
		MAP_GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_5);
		break;

	default:
		Puts("Invalid I2C base.\r\n");
		return 1;
	}

	/* Configure Master module */
	MAP_I2CMasterInitExpClk(base, MAP_SysCtlClockGet(), true);

	/* Check identification registers */
	if (magneto_read_register(base, HMC_IDENTIF_REG_A) != HMC_IRA_VALUE ||
	    magneto_read_register(base, HMC_IDENTIF_REG_B) != HMC_IRB_VALUE ||
	    magneto_read_register(base, HMC_IDENTIF_REG_C) != HMC_IRC_VALUE) {
		return 1;
	}

	// normal, 30Hz, 8avg
	magneto_write_register(base, HMC_CONF_REG_A, (HMC_MS_0|HMC_DO_5|HMC_MA_3) );
	// default gain
	magneto_write_register(base, HMC_CONF_REG_B, (HMC_GN_0) );
	// continuous measurement
	magneto_write_register(base, HMC_MODE_REG,   (HMC_MD_0) );

	return 0;
}

u32 magneto_read_register(u32 base, u8 reg)
{
	MAP_I2CMasterSlaveAddrSet(base, HMC_DEVICE_ADDR, false);

	MAP_I2CMasterDataPut(base, reg);
	MAP_I2CMasterControl(base, I2C_MASTER_CMD_SINGLE_SEND);
	if (_magneto_error(base))
		return -1;

	MAP_I2CMasterSlaveAddrSet(base, HMC_DEVICE_ADDR, true);
	MAP_I2CMasterControl(base, I2C_MASTER_CMD_SINGLE_RECEIVE);
	if (_magneto_error(base))
		return -1;

	return MAP_I2CMasterDataGet(base);
}

void magneto_write_register(u32 base, u8 reg, u8 data)
{
	MAP_I2CMasterSlaveAddrSet(base, HMC_DEVICE_ADDR, false);

	MAP_I2CMasterDataPut(base, reg);
	MAP_I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_START);
	if (_magneto_error(base)) {
		MAP_I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
		return;
	}

	MAP_I2CMasterDataPut(base, data);
	MAP_I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_FINISH);
	if (_magneto_error(base)) {
		MAP_I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
		return;
	}
}

void magneto_read_data(u32 base, struct mag_data *data)
{
	u32 i;
	u8 tmp[6];

	for (i=0; i<6; ++i)
		tmp[i] = magneto_read_register(base, HMC_OUT_X_MSB + i);

	data->x = (tmp[0]<<8)|tmp[1];
	data->z = (tmp[2]<<8)|tmp[3];
	data->y = (tmp[4]<<8)|tmp[5];
}

void magneto_print_config(u32 base)
{
	Printf("# HMC5883 compass configuration:\r\n");
	Printf("# Control register A: 0x%x\r\n",
			magneto_read_register(base, HMC_CONF_REG_A));
	Printf("# Control register B: 0x%x\r\n",
			magneto_read_register(base, HMC_CONF_REG_B));
	Printf("# Mode register:      0x%x\r\n",
			magneto_read_register(base, HMC_MODE_REG));
	Printf("# Status register:    0x%x\r\n\n",
			magneto_read_register(base, HMC_STATUS_REG));
}
