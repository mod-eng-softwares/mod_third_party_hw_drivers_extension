/***************************************************************************//**
 * @file ble_si91x.c
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
#include <stdlib.h>
#include <sl_string.h>
#include "ble.h"

//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"
#include "sl_utility.h"

#include "ble_config.h"
#include "rsi_ble.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_common_config.h"
#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"
#include "rsi_common_apis.h"

#include "bthome_v2.h"
#include "mbedtls/ccm.h"

// Length of the device address
#define BD_ADDR_LEN          18

static const sl_wifi_device_configuration_t
  ble_config = {
    .boot_option = LOAD_NWP_FW,
    .mac_address = NULL,
    .band = SL_SI91X_WIFI_BAND_2_4GHZ,
    .region_code = US,
    .boot_config = {
      .oper_mode = SL_SI91X_CLIENT_MODE,
      .coex_mode = SL_SI91X_WLAN_BLE_MODE,
      .feature_bit_map = (SL_SI91X_FEAT_WPS_DISABLE
                         | SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE
                         | SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1),
      .tcp_ip_feature_bit_map = 0,
      .custom_feature_bit_map = 0,
      .ext_custom_feature_bit_map = SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE,
      .ext_tcp_ip_feature_bit_map = 0,
      .bt_feature_bit_map = (SL_SI91X_BT_RF_TYPE
                             | SL_SI91X_ENABLE_BLE_PROTOCOL),
      .ble_feature_bit_map =
        (SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
        | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC)
        | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID
        | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
        | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS)
        | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE),
      .ble_ext_feature_bit_map = 0,
      .config_feature_bit_map = SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP }};

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Initializes function for driver.
 ******************************************************************************/
sl_status_t ble_init(void)
{
  return sl_wifi_init(&ble_config, NULL, sl_wifi_default_event_handler);
}

/***************************************************************************//**
 *  Start advertising.
 ******************************************************************************/
sl_status_t ble_advertise_start(void)
{
  return rsi_ble_start_advertising();
}

/***************************************************************************//**
 *  Stop advertising.
 ******************************************************************************/
sl_status_t ble_advertise_stop(void)
{
  return rsi_ble_stop_advertising();
}

/***************************************************************************//**
 * Set user-defined advertising data packet.
 ******************************************************************************/
sl_status_t ble_advertise_set_data(uint8_t* data, uint8_t data_len)
{
  return rsi_ble_set_advertise_data(data, data_len);
}

/***************************************************************************//**
 * Get the Bluetooth address.
 ******************************************************************************/
sl_status_t ble_get_address(uint8_t* addr)
{
  uint8_t address[6] = { 0 };

  // Extract unique ID from BT Address.
  rsi_bt_get_local_device_address(address);
  memcpy(addr, address, 6);

  return SL_STATUS_OK;
}
