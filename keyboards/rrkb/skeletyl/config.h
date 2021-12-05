/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
* Changes made to the original kb to accomodate my hand soldering
* RR - 19/11/2021
*/

#pragma once
#include "config_common.h"
#define VENDOR_ID 0xA8F8
#define PRODUCT_ID 0x1830
#define DEVICE_VER 0x0001
#define MANUFACTURER Bastard Keyboards
#define PRODUCT Skeletyl

#define MATRIX_ROWS 6
#define MATRIX_COLS 6
#define RGBLIGHT_LIMIT_VAL 180
#define MATRIX_ROW_PINS { F7, B1, B3 }
#define MATRIX_COL_PINS { B5, B4, E6, D7, C6, B2 }

#define DIODE_DIRECTION COL2ROW //Left side works with this setting
// #define DIODE_DIRECTION ROW2COL

#define RGB_DI_PIN B6
#define RGBLED_NUM 8
#define RGBLED_SPLIT { 4, 4 }
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LAYERS
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB

#define DEBOUNCE 5

//#define USE_I2C
#define SOFT_SERIAL_PIN D1
#define USE_SERIAL
#define SELECT_SOFT_SERIAL_SPEED 0
//#define SERIAL_USE_MULTI_TRANSACTION

//#define USB_POLLING_INTERVAL_MS 1
//define SPLIT_USB_DETECT
#define SPLIT_HAND_PIN D4
#define MASTER_LEFT
//#define MASTER_RIGHT
//#define EE_HANDS
#define SPLIT_CONNECTION_CHECK_TIMEOUT 500
//#define SPLIT_MAX_CONNECTION_ERRORS 10
//#define FORCED_SYNC_THROTTLE_MS 100

//#define DEBUG_MATRIX_SCAN_RATE

#define ONESHOT_TAP_TOGGLE 1  /* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TIMEOUT 500  /* Time (in ms) before the one shot key is released */
#define TAPPING_TERM 175
#define TAPPING_TOGGLE 1

#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

//#define AUTO_SHIFT_TIMEOUT 200
//#define NO_AUTO_SHIFT_SPECIAL
//#define TAPPING_FORCE_HOLD

// RGB matrix support
#ifdef RGB_MATRIX_ENABLE
#    define SPLIT_TRANSPORT_MIRROR
#    define DRIVER_LED_TOTAL 36 // Number of LEDs
#    define RGB_MATRIX_SPLIT { 18, 18 }
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 50
#    define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_DISABLE_WHEN_USB_SUSPENDED
#    define RGB_MATRIX_KEYPRESSES
#endif
