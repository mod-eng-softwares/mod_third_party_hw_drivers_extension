/***************************************************************************//**
 * @file ble_efx.c
 * @brief BLE functionality for BTHome v2.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#include <string.h>
#include "sl_bt_api.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Initializes function for driver.
 ******************************************************************************/
sl_status_t ble_init(void)
{
  sl_status_t sc;

  // Create an advertising set.
  sc = sl_bt_advertiser_create_set(&advertising_set_handle);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  // Generate data for advertising
  sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                             sl_bt_advertiser_general_discoverable);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  // Set advertising interval to 100ms.
  sc = sl_bt_advertiser_set_timing(
    advertising_set_handle,   // advertising set handle
    160,   // min. adv. interval (milliseconds * 1.6)
    160,   // max. adv. interval (milliseconds * 1.6)
    0,     // adv. duration
    0);    // max. num. adv. events
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Start advertising.
 ******************************************************************************/
sl_status_t ble_advertise_start(void)
{
  return sl_bt_legacy_advertiser_start(advertising_set_handle,
                                       sl_bt_legacy_advertiser_non_connectable);
}

/***************************************************************************//**
 *  Stop advertising.
 ******************************************************************************/
sl_status_t ble_advertise_stop(void)
{
  return sl_bt_advertiser_stop(advertising_set_handle);
}

/***************************************************************************//**
 * Set user-defined advertising data packet.
 ******************************************************************************/
sl_status_t ble_advertise_set_data(uint8_t* data, uint8_t data_len)
{
  return sl_bt_legacy_advertiser_set_data(advertising_set_handle,
                                          sl_bt_advertiser_advertising_data_packet,
                                          data_len, data);
}

/***************************************************************************//**
 * Get the Bluetooth address.
 ******************************************************************************/
sl_status_t ble_get_address(uint8_t* addr)
{
  sl_status_t sc;
  bd_addr address;
  uint8_t address_type;

  // Extract unique ID from BT Address.
  sc = sl_bt_system_get_identity_address(&address, &address_type);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  memcpy(addr, address.addr, 6);

  return SL_STATUS_OK;
}
