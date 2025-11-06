/***************************************************************************//**
 * @file lora_rm126x.h
 * @brief LoRa RM126x driver wrapper for LR11868MHz Click driver.
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

#ifndef LORA_RM126X_H
#define LORA_RM126X_H

#include "sl_status.h"
#include "sl_iostream.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup lora_rm126x - LoRa RM126x Driver
 * @brief
 *  Silicon Labs LoRa RM126x driver wrapper for LR11868MHz Click driver.
 *  This driver provides Silicon Labs API compatibility for the LoRa RM126x
 *  AT command interface module.
 *
 *    @n @section lora_rm126x_example Example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 *      #include "app_log.h"
 *      #include "lora_rm126x.h"
 *      #include "sl_iostream_init_usart_instances.h"
 *      #include <string.h>
 *      #include <stdint.h>
 *
 *      static uint8_t rx_buffer[256];
 *      static uint16_t rx_len;
 *
 *      void app_init(void)
 *      {
 *        app_log("LoRa RM126x Driver Init\r\n");
 *        sl_status_t status = lora_rm126x_init(sl_iostream_uart_mikroe_handle);
 *        app_log("lora_rm126x_init = 0x%lx\r\n", status);
 *
 *        // Reset the module
 *        lora_rm126x_reset_device();
 *        app_log("Device reset completed\r\n");
 *
 *        // Send AT command
 *        lora_rm126x_cmd_run((uint8_t *)"AT", NULL);
 *        sl_sleeptimer_delay_millisecond(100);
 *
 *        // Read response
 *        status = lora_rm126x_generic_read(rx_buffer, sizeof(rx_buffer),
 *   &rx_len);
 *        if (status == SL_STATUS_OK) {
 *          app_log("Response: %.*s\r\n", rx_len, rx_buffer);
 *        }
 *      }
 *
 *      @endcode
 *
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    LoRa RM126x configuration structure.
 * @details
 *    This structure contains configuration parameters for initializing the
 *    LoRa RM126x driver.
 ******************************************************************************/
typedef struct {
  void *uart_handle;          /**< UART handle for communication */
  bool uart_blocking;         /**< UART blocking mode */
  uint32_t baud_rate;         /**< UART baud rate */
} lora_rm126x_config_t;

/***************************************************************************//**
 * @brief
 *    LoRa RM126x control commands.
 * @details
 *    Predefined control commands for the LoRa RM126x module.
 ******************************************************************************/
#define LORA_RM126X_CMD_AT                                  "AT"
#define LORA_RM126X_CMD_FACTORY_RESET                       "AT&F"
#define LORA_RM126X_CMD_SAVE_SETTINGS                       "AT&W"
#define LORA_RM126X_CMD_SOFT_RESET                          "ATZ"
#define LORA_RM126X_CMD_GET_INFO                            "ATI"
#define LORA_RM126X_CMD_PARAM_ACCESS_STR                    "AT%S"
#define LORA_RM126X_CMD_PARAM_ACCESS_NUM                    "ATS"
#define LORA_RM126X_CMD_I2C_READ                            "AT+I2R"
#define LORA_RM126X_CMD_I2C_WRITE                           "AT+I2W"
#define LORA_RM126X_CMD_NET_DROP                            "AT+DROP"
#define LORA_RM126X_CMD_NET_JOIN                            "AT+JOIN"
#define LORA_RM126X_CMD_NET_CHECK_LINK                      "AT+LINKC"
#define LORA_RM126X_CMD_NET_SEND_DATA                       "AT+SEND"
#define LORA_RM126X_CMD_NET_GET_TIME                        "AT+TIME"
#define LORA_RM126X_CMD_GROUP_CREATE                        "AT+MCGA"
#define LORA_RM126X_CMD_GROUP_REMOVE                        "AT+MCGR"
#define LORA_RM126X_CMD_GROUP_START                         "AT+MCGS"
#define LORA_RM126X_CMD_GROUP_END                           "AT+MCGE"
#define LORA_RM126X_CMD_GROUP_LIST                          "AT+MCGL"
#define LORA_RM126X_CMD_P2P_END_SESSION                     "AT+P2PE"
#define LORA_RM126X_CMD_P2P_START_SESSION                   "AT+P2PS"
#define LORA_RM126X_CMD_P2P_SEND_DATA                       "AT+P2PT"
#define LORA_RM126X_CMD_SIO_CONFIG                          "AT+SIOC"
#define LORA_RM126X_CMD_SIO_READ                            "AT+SIOR"
#define LORA_RM126X_CMD_SIO_WRITE                           "AT+SIOW"
#define LORA_RM126X_CMD_SPI_READ                            "AT+SPR"
#define LORA_RM126X_CMD_SPI_WRITE                           "AT+SPW"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x get info (ATI) command parameters.
 * @details
 *    Specified setting for get info (ATI) command parameters of LoRa RM126x
 *   module.
 ******************************************************************************/

#define LORA_RM126X_PARAM_DEVICE_TYPE                       "0"
#define LORA_RM126X_PARAM_APP_FW_VERSION                    "3"
#define LORA_RM126X_PARAM_APP_STATE                         "42"
#define LORA_RM126X_PARAM_MIN_BAUD_RATE                     "1002"
#define LORA_RM126X_PARAM_MAX_BAUD_RATE                     "1003"
#define LORA_RM126X_PARAM_RESET_REASON                      "2000"
#define LORA_RM126X_PARAM_RESET_CAUSE                       "2001"
#define LORA_RM126X_PARAM_TX_POWER                          "2008"
#define LORA_RM126X_PARAM_RADIO_ACTIVITY                    "2016"
#define LORA_RM126X_PARAM_BANDWIDTH                         "3000"
#define LORA_RM126X_PARAM_CONNECTION_STATUS                 "3001"
#define LORA_RM126X_PARAM_DEVICE_ADDRESS                    "3002"
#define LORA_RM126X_PARAM_BOOTLOADER_VERSION                "3003"
#define LORA_RM126X_PARAM_MODULATION_TYPE                   "3004"
#define LORA_RM126X_PARAM_MODULE_TYPE                       "3005"
#define LORA_RM126X_PARAM_PACKETS_RXED                      "3006"
#define LORA_RM126X_PARAM_PACKETS_TXED                      "3007"
#define LORA_RM126X_PARAM_RSSI                              "3008"
#define LORA_RM126X_PARAM_SNR                               "3009"
#define LORA_RM126X_PARAM_SPREADING_FACTOR                  "3010"
#define LORA_RM126X_PARAM_P2P_BEACON_TOA                    "4000"
#define LORA_RM126X_PARAM_P2P_BEACON_SETTLE_TIME            "4001"
#define LORA_RM126X_PARAM_P2P_SLOT_TOA                      "4002"
#define LORA_RM126X_PARAM_P2P_SLOT_PACKET_SIZE              "4003"
#define LORA_RM126X_PARAM_P2P_SLOT_DELAY                    "4004"
#define LORA_RM126X_PARAM_P2P_MIN_WINDOW_LENGTH             "4005"
#define LORA_RM126X_PARAM_P2P_MAX_SLOTS_PER_WINDOW          "4006"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x param access string (AT%S) command parameters ID.
 * @details
 *    Predefined setting for param access string (AT%S) command parameters ID
 ******************************************************************************/
#define LORA_RM126X_PARAM_ID_DEVICE_NAME                    "0"
#define LORA_RM126X_PARAM_ID_APP_KEY                        "500"
#define LORA_RM126X_PARAM_ID_DEV_EUI                        "501"
#define LORA_RM126X_PARAM_ID_JOIN_EUI                       "502"
#define LORA_RM126X_PARAM_ID_ABP_P2P_ADDRESS                "635"
#define LORA_RM126X_PARAM_ID_ABP_P2P_NET_KEY                "636"
#define LORA_RM126X_PARAM_ID_ABP_P2P_APP_KEY                "637"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x param access numeric (ATS) command parameters ID.
 * @details
 *     Specified setting for param access numeric (ATS) command parameters ID
 ******************************************************************************/
#define LORA_RM126X_PARAM_ID_START_FLAGS                    "100"
#define LORA_RM126X_PARAM_ID_SIO_NET                        "117"
#define LORA_RM126X_PARAM_ID_UART_IDLE_TIME                 "213"
#define LORA_RM126X_PARAM_ID_CORE_VOLTAGE                   "217"
#define LORA_RM126X_PARAM_ID_UART_BAUD_RATE                 "302"
#define LORA_RM126X_PARAM_ID_ADR_ENABLED                    "600"
#define LORA_RM126X_PARAM_ID_BEACON_FREQ                    "601"
#define LORA_RM126X_PARAM_ID_ACTIVATION_MODE                "602"
#define LORA_RM126X_PARAM_ID_DEVICE_CLASS                   "603"
#define LORA_RM126X_PARAM_ID_CONFIRMED_PACKETS              "604"
#define LORA_RM126X_PARAM_ID_CONFIRMED_RETRY_COUNT          "605"
#define LORA_RM126X_PARAM_ID_JOIN_DELAY                     "606"
#define LORA_RM126X_PARAM_ID_JOIN_JITTER                    "607"
#define LORA_RM126X_PARAM_ID_PING_SLOT_PERIODICITY          "609"
#define LORA_RM126X_PARAM_ID_REGION                         "611"
#define LORA_RM126X_PARAM_ID_REQUEST_COUNT                  "612"
#define LORA_RM126X_PARAM_ID_RX1_DELAY                      "613"
#define LORA_RM126X_PARAM_ID_SUB_BAND                       "617"
#define LORA_RM126X_PARAM_ID_APP_PORT                       "629"
#define LORA_RM126X_PARAM_ID_CLASS_C_SCAN_TIME              "633"
#define LORA_RM126X_PARAM_ID_P2P_DEVICE_ADDRESS             "700"
#define LORA_RM126X_PARAM_ID_P2P_NET_SIZE                   "701"
#define LORA_RM126X_PARAM_ID_P2P_WINDOW_LENGTH              "702"
#define LORA_RM126X_PARAM_ID_P2P_DATA_RATE                  "703"
#define LORA_RM126X_PARAM_ID_P2P_LISTEN_DURATION            "704"
#define LORA_RM126X_PARAM_ID_P2P_LISTEN_INTERVAL            "705"
#define LORA_RM126X_PARAM_ID_P2P_BEACON_DATA_RATE           "706"
#define LORA_RM126X_PARAM_ID_P2P_PACKET_SIZE                "707"
#define LORA_RM126X_PARAM_ID_P2P_TX_POWER                   "708"
#define LORA_RM126X_PARAM_ID_PUBLIC_MODE                    "709"
#define LORA_RM126X_PARAM_ID_SLOTS_PER_WINDOW               "710"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x response strings.
 * @details
 *    Expected response strings from the module.
 ******************************************************************************/
#define LORA_RM126X_RSP_OK                                  "OK"
#define LORA_RM126X_RSP_ERROR                               "ERROR"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x device events settings.
 * @details
 *    Device events settings.
 ******************************************************************************/
#define LORA_RM126X_EVT_CLASS                               "CLASS "
#define LORA_RM126X_EVT_JOIN                                "JOIN ["
#define LORA_RM126X_EVT_LINK_CHECK                          "LC S:"
#define LORA_RM126X_EVT_RX_LNS                              "RX: W:"
#define LORA_RM126X_EVT_RX_P2P                              "RX: [P2P] S:"
#define LORA_RM126X_EVT_TIME                                "TIME ["
#define LORA_RM126X_EVT_TX                                  "TX ["
#define LORA_RM126X_EVT_WAKE                                "WAKE"

/***************************************************************************//**
 * @brief
 *    LoRa RM126x initialization function.
 *
 * @param[in] uart_handle
 *    UART handle for the communication interface (can be sl_iostream handle)
 *
 * @return
 *    SL_STATUS_OK: Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_INITIALIZATION: Initialization failed.
 *    SL_STATUS_ALREADY_INITIALIZED: Driver is already initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_init(sl_iostream_t *uart_handle);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x deinitialization function.
 *
 * @return
 *    SL_STATUS_OK: Successful deinitialization.
 ******************************************************************************/
sl_status_t lora_rm126x_deinit(void);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic write function.
 *
 * @param[in] data_buf
 *    Data buffer to write
 * @param[in] len
 *    Number of bytes to write
 *
 * @return
 *    SL_STATUS_OK: Successful write operation.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_FAIL: Write operation failed.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_write(char *data_buf, size_t len);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic read function.
 *
 * @param[out] data_buf
 *    Buffer to store read data
 * @param[in] max_len
 *    Maximum number of bytes to read
 * @param[out] actual_len
 *    Actual number of bytes read
 *
 * @return
 *    SL_STATUS_OK: Successful read operation.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_EMPTY: No data available.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_read(char *data_buf,
                                     size_t max_len,
                                     size_t *actual_len);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic flush function.
 * @return
 *    SL_STATUS_OK: Successful flush operation.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_flush(void);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x reset device function.
 *
 * @return
 *    None
 ******************************************************************************/
void lora_rm126x_reset_device(void);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command run function.
 *
 * @param[in] cmd
 *    Command string to send
 * @param[in] param
 *    Parameter string (NULL if no parameters needed)
 *
 * @return
 *    SL_STATUS_OK: Successful command execution.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_FAIL: Command execution failed.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_run(char *cmd, char *param);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command set function.
 *
 * @param[in] cmd
 *    Command string
 * @param[in] param_id
 *    Parameter ID string
 * @param[in] value
 *    Value string to set
 *
 * @return
 *    SL_STATUS_OK: Successful command execution.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_FAIL: Command execution failed.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_set(char *cmd, char *param_id, char *value);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command get function.
 *    Queries the specified parameter, with <command> <parameter_id>?
 * @param[in] cmd
 *    Command string
 * @param[in] param_id
 *    Parameter ID string
 *
 * @return
 *    SL_STATUS_OK: Successful command execution.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_FAIL: Command execution failed.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_get(char *cmd, char *param_id);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command help function.
 *    Queries the specified parameter, with <command> <parameter_id>=?
 * @param[in] cmd
 *    Command string
 * @param[in] param_id
 *    Parameter ID string
 *
 * @return
 *    SL_STATUS_OK: Successful command execution.
 *    SL_STATUS_INVALID_PARAMETER: Invalid parameter provided.
 *    SL_STATUS_FAIL: Command execution failed.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_help(char *cmd, char *param_id);

/***************************************************************************//**
 * @brief
 *    It is a blocking function that waits for an acknowledgment from the
 *   device.
 *    Do not use this function in an interrupt context.
 *
 * @return
 *    SL_STATUS_OK: ACK received.
 *    SL_STATUS_TIMEOUT: Timeout waiting for ACK.
 *    SL_STATUS_NOT_INITIALIZED: Driver is not initialized.
 ******************************************************************************/
sl_status_t lora_rm126x_wait_for_ACK(void);

/***************************************************************************//**
 * @brief
 *    Check if the LoRa RM126x driver is initialized.
 *
 * @return
 *    true if initialized, false otherwise
 ******************************************************************************/
bool lora_rm126x_is_initialized(void);

#ifdef __cplusplus
}
#endif

#endif // LORA_RM126X_H
