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
// HMC5883.h
//
// I²C Communication Definitions For Honeywell HMC5883L
// 3-Axix Digital Compass IC
//
//------------------------------------------------------------------------------
#ifndef _HMC5883L_H_
#define _HMC5883L_H_

#ifdef __cplusplus
extern "C" {
#endif

/* HMC5883L I²C Device Adress */
#define HMC_DEVICE_ADDR         (0x1E) /* 7-bit address */
//#define HMC_DEVICE_READ_ADDR    (0x3D) /* 8-bit read address*/
//#define HMC_DEVICE_WRITE_ADDR   (0x3C) /* 8-bit write address*/

/* HMC5883L I²C Register Adress */
#define HMC_CONF_REG_A          (0x00) /* Configuration register A */
#define HMC_CONF_REG_B          (0x01) /* Configuration register B */
#define HMC_MODE_REG            (0x02) /* Mode register */
#define HMC_OUT_X_MSB           (0x03) /* Data Output X MSB Register */
#define HMC_OUT_X_LSB           (0x04) /* Data Output X LSB Register */
#define HMC_OUT_Z_MSB           (0x05) /* Data Output Z MSB Register */
#define HMC_OUT_Z_LSB           (0x06) /* Data Output Z LSB Register */
#define HMC_OUT_Y_MSB           (0x07) /* Data Output Y MSB Register */
#define HMC_OUT_Y_LSB           (0x08) /* Data Output Y LSB Register */
#define HMC_STATUS_REG          (0x09) /* Status register */
#define HMC_IDENTIF_REG_A       (0x0A) /* Identification register A */
#define HMC_IDENTIF_REG_B       (0x0B) /* Identification register B */
#define HMC_IDENTIF_REG_C       (0x0C) /* Identification register C */

/* Configuration Register A Control Bits */
#define HMC_MS0                 (0x01) /* Measurement Configuration Select Bit: 0 */
#define HMC_MS1                 (0x02) /* Measurement Configuration Select Bit: 1 */
#define HMC_DO0                 (0x04) /* Data Output Rate Select Bit: 0 */
#define HMC_DO1                 (0x08) /* Data Output Rate Select Bit: 1 */
#define HMC_DO2                 (0x10) /* Data Output Rate Select Bit: 2 */
#define HMC_MA0                 (0x20) /* Number Of Samples Averaged Select Bit: 0 */
#define HMC_MA1                 (0x40) /* Number Of Samples Averaged Select Bit: 1 */
//#define RESERVED              (0x80) /* Must be cleared for correct operation */

#define HMC_MS_0                (0x00) /* Normal Measurement Configuration */
#define HMC_MS_1                (0x01) /* Positive Bias Configuration */
#define HMC_MS_2                (0x02) /* Negative Bias Configuration */

#define HMC_DO_0                (0x00) /* Typical Data Output Rate: 0.75 Hz */
#define HMC_DO_1                (0x04) /* Typical Data Output Rate: 1.5 Hz */
#define HMC_DO_2                (0x08) /* Typical Data Output Rate: 3 Hz */
#define HMC_DO_3                (0x0C) /* Typical Data Output Rate: 7.5 Hz */
#define HMC_DO_4                (0x10) /* Typical Data Output Rate: 15 Hz (Default) */
#define HMC_DO_5                (0x14) /* Typical Data Output Rate: 30 Hz */
#define HMC_DO_6                (0x1C) /* Typical Data Output Rate: 75 Hz */

#define HMC_MA_0                (0x00) /* Number Of Samples Averaged: 1 */
#define HMC_MA_1                (0x20) /* Number Of Samples Averaged: 2 */
#define HMC_MA_2                (0x40) /* Number Of Samples Averaged: 4 */
#define HMC_MA_3                (0x60) /* Number Of Samples Averaged: 8 (Default) */

/* Configuration Register B Control Bits */
//#define RESERVED              (0x01) /* Must be cleared for correct operation */
//#define RESERVED              (0x02) /* Must be cleared for correct operation */
//#define RESERVED              (0x04) /* Must be cleared for correct operation */
//#define RESERVED              (0x08) /* Must be cleared for correct operation */
//#define RESERVED              (0x10) /* Must be cleared for correct operation */
#define HMC_GN0                 (0x20) /* Gain Configuration Selest Bit: 0 */
#define HMC_GN1                 (0x40) /* Gain Configuration Selest Bit: 1 */
#define HMC_GN2                 (0x80) /* Gain Configuration Selest Bit: 2 */

#define HMC_GN_0                (0x00) /* Gain (LSB/Gauss): 1370 */
#define HMC_GN_1                (0x20) /* Gain (LSB/Gauss): 1090 (Default) */
#define HMC_GN_2                (0x40) /* Gain (LSB/Gauss): 820 */
#define HMC_GN_3                (0x60) /* Gain (LSB/Gauss): 660 */
#define HMC_GN_4                (0x80) /* Gain (LSB/Gauss): 440 */
#define HMC_GN_5                (0xA0) /* Gain (LSB/Gauss): 390 */
#define HMC_GN_6                (0xC0) /* Gain (LSB/Gauss): 330 */
#define HMC_GN_7                (0xE0) /* Gain (LSB/Gauss): 230 */

/* Mode Register Control Bits */
#define HMC_MD0                 (0x01) /* Mode Select Bit: 0 */
#define HMC_MD1                 (0x02) /* Mode Select Bit: 1 */
//define RESERVED               (0x04) /* Must be cleared for correct operation */
//define RESERVED               (0x08) /* Must be cleared for correct operation */
//define RESERVED               (0x10) /* Must be cleared for correct operation */
//define RESERVED               (0x20) /* Must be cleared for correct operation */
//define RESERVED               (0x40) /* Must be cleared for correct operation */
//define RESERVED               (0x80) /* Must be cleared for correct operation */

#define HMC_MD_0                (0x00) /* Continuous-Measurement Mode */
#define HMC_MD_1                (0x01) /* Single-Measurement Mode (Default) */
#define HMC_MD_2                (0x02) /* Idle Mode */
#define HMC_MD_3                (0x03) /* Idle Mode */

/* Data Output X Registers A (MSB) and B (LSB) Bits */
#define HMC_DXRB0               (0x0001)
#define HMC_DXRB1               (0x0002)
#define HMC_DXRB2               (0x0004)
#define HMC_DXRB3               (0x0008)
#define HMC_DXRB4               (0x0010)
#define HMC_DXRB5               (0x0020)
#define HMC_DXRB6               (0x0040)
#define HMC_DXRB7               (0x0080)

#define HMC_DXRA0               (0x0100)
#define HMC_DXRA1               (0x0200)
#define HMC_DXRA2               (0x0400)
#define HMC_DXRA3               (0x0800)
#define HMC_DXRA4               (0x1000)
#define HMC_DXRA5               (0x2000)
#define HMC_DXRA6               (0x4000)
#define HMC_DXRA7               (0x8000)

/* Data Output Y Registers A (MSB) and B (LSB) Bits */
#define HMC_DYRB0               (0x0001)
#define HMC_DYRB1               (0x0002)
#define HMC_DYRB2               (0x0004)
#define HMC_DYRB3               (0x0008)
#define HMC_DYRB4               (0x0010)
#define HMC_DYRB5               (0x0020)
#define HMC_DYRB6               (0x0040)
#define HMC_DYRB7               (0x0080)

#define HMC_DYRA0               (0x0100)
#define HMC_DYRA1               (0x0200)
#define HMC_DYRA2               (0x0400)
#define HMC_DYRA3               (0x0800)
#define HMC_DYRA4               (0x1000)
#define HMC_DYRA5               (0x2000)
#define HMC_DYRA6               (0x4000)
#define HMC_DYRA7               (0x8000)

/* Data Output Z Registers A (MSB) and B (LSB) Bits */
#define HMC_DZRB0               (0x0001)
#define HMC_DZRB1               (0x0002)
#define HMC_DZRB2               (0x0004)
#define HMC_DZRB3               (0x0008)
#define HMC_DZRB4               (0x0010)
#define HMC_DZRB5               (0x0020)
#define HMC_DZRB6               (0x0040)
#define HMC_DZRB7               (0x0080)

#define HMC_DZRA0               (0x0100)
#define HMC_DZRA1               (0x0200)
#define HMC_DZRA2               (0x0400)
#define HMC_DZRA3               (0x0800)
#define HMC_DZRA4               (0x1000)
#define HMC_DZRA5               (0x2000)
#define HMC_DZRA6               (0x4000)
#define HMC_DZRA7               (0x8000)

/* Status Register Control Bits */
#define HMC_RDY                 (0x01) /* Ready Bit (Read-Only) */
#define HMC_LOCK                (0x02) /* Data Output Register Lock (Read-Only) */
//define RESERVED               (0x04) /* (Read-Only) */
//define RESERVED               (0x08) /* (Read-Only) */
//define RESERVED               (0x10) /* (Read-Only) */
//define RESERVED               (0x20) /* (Read-Only) */
//define RESERVED               (0x40) /* (Read-Only) */
//define RESERVED               (0x80) /* (Read-Only) */

/* Identification Register A Value */
#define HMC_IRA_VALUE           (0x48) /* Register value. ASCII value H (Read-Only) */

/* Identification Register B Value */
#define HMC_IRB_VALUE           (0x34) /* Register value. ASCII value 4 (Read-Only) */

/* Identification Register C Value */
#define HMC_IRC_VALUE           (0x33) /* Register value. ASCII value 3 (Read-Only) */


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _HMC5883L_H_ */
