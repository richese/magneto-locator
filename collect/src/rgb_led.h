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
// rgb_led.h
//
//
//
//------------------------------------------------------------------------------
#ifndef _COLLECT_RGB_LED_H_
#define _COLLECT_RGB_LED_H_

#include "intdef.h"

//!
//! Type used to store color information.
//! '0x00rrggbb'
//!
typedef u32 color_t;

//
// Some sample colors.
//
#define COLOR_OFF               (0x00000000)
#define COLOR_RED               (0x00FF0000)
#define COLOR_GREEN             (0x0000FF00)
#define COLOR_BLUE              (0x000000FF)
#define COLOR_WHITE             (0x00FFFFFF)
#define COLOR_DARK_GREEN        (0x00006400)
#define COLOR_CYAN              (0x0000FFFF)
#define COLOR_GOLD              (0x00FFD700)
#define COLOR_MAGENTA           (0x00FF00FF)
#define COLOR_ORANGE            (0x00FFa500)
#define COLOR_SKY_BLUE          (0x0087CEEB)
#define COLOR_YELLOW            (0x00FFFF00)

//!
//! Type used to store data for one componnent of color.
//!
typedef u8  component_t;

//
// Single led selectors.
//
#define LED_RED                 (0x01)
#define LED_GREEN               (0x02)
#define LED_BLUE                (0x04)

//!
//! Type for light intensity shared for all leds.
//!
typedef u8  intensity_t;

//
// Sample intensity levels.
//
#define INTENSITY_FULL          (0xFF)
#define INTENSITY_HALF          (0x7F)
#define INTENSITY_QUARTER       (0x3F)
#define INTENSITY_OFF           (0x00)
#define LED_OFF                 (0x00)
#define LED_ON                  (0xFF)

//
// PWM period.
//
#define RGB_PWM_PERIOD          (781250)


//!
//! Initialize led driver.
//!
void rgb_init(void);

//!
//! Set one led to given intensity.
//!
//! \param led One or more led selectors.
//! \param intensity Desired intensity. Value in range from 0x00 to 0xFF.
//!
void rgb_set_led(u8 led, intensity_t intensity);

//!
//! Set all leds to aproximate given color.
//!
//! \param color
//!
void rgb_set_color(color_t color);

//!
//! Set intensity level for all leds.
//!
//! \param intensity Value in range from 0x00 to 0xFF.
//!
void rgb_set_intensity(intensity_t intensity);

//!
//! Convert color given by it's red, green, blue commonents.
//!
color_t rgb_to_color(component_t r, component_t g, component_t b);


#endif // _COLLECT_RGB_LED_H_
