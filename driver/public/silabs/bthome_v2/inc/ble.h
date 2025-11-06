/***************************************************************************//**
 * @file ble.h
 * @brief BLE functionality for BTHome v2.
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
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#ifndef BLE_H_
#define BLE_H_

/***************************************************************************//**
 * @addtogroup bthome_v2
 * @{
 *
 * @brief
 *  The implementation of BLE for the BTHome driver.
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "sl_status.h"

/***************************************************************************//**
 * @brief
 *    Initializes function.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t ble_init(void);

/***************************************************************************//**
 * @brief
 *    Start advertising.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t ble_advertise_start(void);

/***************************************************************************//**
 * @brief
 *    Stop advertising.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t ble_advertise_stop(void);

/***************************************************************************//**
 * @brief
 *    Set user-defined advertising data packet.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t ble_advertise_set_data(uint8_t* data, uint8_t data_len);
/***************************************************************************//**
 * @brief
 *    Get the Bluetooth identity address.
 *
 * @param[out] addr
 *    Bluetooth 6 bytes address.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t ble_get_address(uint8_t* addr);

#endif /* BLE_H_ */
