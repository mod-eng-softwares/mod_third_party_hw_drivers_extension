/***************************************************************************//**
 * @file epaper_display.h
 * @brief E-Paper Display Header File
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#ifndef EPAPER_DISPLAY
#define EPAPER_DISPLAY

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @name Update mode
#define UPDATE_NONE   0x00 ///< No update
#define UPDATE_NORMAL 0x01 ///< Normal  , default
#define UPDATE_FAST   0x02 ///< Fast update

/***************************************************************************//**
 * @brief  EPD Initialization function.
 ******************************************************************************/
void epd_init(void);

/***************************************************************************//**
 * @brief
 *    EPD Update Display function.
 *
 * @param[in] next_frame: data is the new image data that you want to display
 * @param[in] previous_frame: The data definition of DRAM2 is different between
 *   "Normal update" and "Fast update"
 *       1. Normal update: previousFrame image is dummy data
 *       2. Fast update: previousFrame image is the OLD image data
 * @param[in] frame_size: Size of frame to update
 ******************************************************************************/
void epd_update_display(uint8_t *next_frame,
                        uint8_t *previous_frame,
                        uint32_t frame_size);

/***************************************************************************//**
 * @brief
 *    EPD Update mode function.
 *
 * @param[in] mode update mode UPDATE_NORMAL or UPDATE_FAST
 ******************************************************************************/
void epd_set_update_mode(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif // EPAPER_DISPLAY
