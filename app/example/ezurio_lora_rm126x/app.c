/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "lora_rm126x.h"
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"
#include "app_log.h"

#include "sl_simple_button.h"
#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"

#include "sl_udelay.h"

/***************************************************************************//**
 * LoRa Network Mode
 ******************************************************************************/
#warning "Select Node type: Central or Sensor"
// Select Node type: Central or Sensor
// 1 - Sensor Node
// 0 - Central Node
#define LORA_Node_Type 1   // Central Node

/***************************************************************************//**
 * LoRa RM126x P2P Network parameters.
 ******************************************************************************/
// LoRa RM126x P2P Node ID
// 0 - central
// 1 - sensor
#if LORA_Node_Type
#define LORA_RM126X_P2P_NODE_ID_TX   "0"
#define LORA_RM126X_P2P_NODE_ID      "1"
#else
#define LORA_RM126X_P2P_NODE_ID_TX   "1"
#define LORA_RM126X_P2P_NODE_ID      "0"
#endif
// The default network size will be 2
#define LORA_RM126X_P2P_NETWORK_SIZE "2"
// The default data rate will be DR2
#define LORA_RM126X_P2P_DATA_RATE    "2"
// The default transmission power will be 1dBm
#define LORA_RM126X_P2P_TX_POWER     "1"

/***************************************************************************//**
 * Application Static variables.
 ******************************************************************************/
static char rx_buffer[256] = { 0 };
static size_t rx_len = 0;
static uint8_t packet_counter = 0;
static uint8_t startTx = false;

/***************************************************************************//**
 * LoRa RM126x P2P Initialization definition.
 ******************************************************************************/
void lora_p2p_init(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log_iostream_set(sl_iostream_vcom_handle);

  app_log("----------------------------\r\n");
  app_log("LoRa RM126X P2P DEMO example\r\n");
  app_log("----------------------------\r\n");
  app_log("> LoRa RM126x Driver Init\r\n");
  sl_status_t status = lora_rm126x_init(sl_iostream_ezurio_handle);
  app_log("lora_rm126x_init = 0x%lx\r\n", status);

  // Reset the module
  lora_rm126x_reset_device();
  app_log("> Device reset completed\r\n");

  app_log("> Test communication with the chip\r\n");
  lora_rm126x_generic_flush();
  // Send AT command
  status = lora_rm126x_cmd_run("AT", NULL);
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_cmd_run() error = 0x%lx\r\n", status);
  }
  sl_udelay_wait(100000); // 100 ms

  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  lora_p2p_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t status;
  char msg_hex[101] = { 0 };
  char source[10] = { 0 };
  char rssi[10] = { 0 };
  char snr[10] = { 0 };
  char *start_ptr = NULL;
  char *end_ptr = NULL;
  char tmp_buffer[50] = { 0 };

  if (startTx) {
    // Send P2P data on button press
    app_log("Button pressed, packet counter: %d\r\n", packet_counter);
    snprintf(tmp_buffer,
             sizeof(tmp_buffer),
             "%s, \"0%x\"",
             LORA_RM126X_P2P_NODE_ID_TX,
             packet_counter);
    lora_rm126x_cmd_run(LORA_RM126X_CMD_P2P_SEND_DATA, tmp_buffer);
    packet_counter++;
    if (packet_counter > 99) {
      packet_counter = 0;
    }
    startTx = false;
  }

  // Read response
  status = lora_rm126x_generic_read(rx_buffer, sizeof(rx_buffer), &rx_len);
  if (status == SL_STATUS_OK) {
    start_ptr = strstr(rx_buffer, LORA_RM126X_EVT_RX_P2P);
    end_ptr = NULL;
    // If P2P event is received
    if (start_ptr) {
      // Extract sender address
      start_ptr = start_ptr + strlen(LORA_RM126X_EVT_RX_P2P);
      end_ptr = strstr(start_ptr, ",");
      memcpy(source, start_ptr, end_ptr - start_ptr);

      // Extract RSSI
      start_ptr = strstr(end_ptr, ":") + 1;
      end_ptr = strstr(start_ptr, ",");
      memcpy(rssi, start_ptr, end_ptr - start_ptr);

      // Extract SNR
      start_ptr = strstr(end_ptr, ":") + 1;
      end_ptr = strstr(start_ptr, ",");
      memcpy(snr, start_ptr, end_ptr - start_ptr);

      // Extract message
      start_ptr = end_ptr + 2;
      end_ptr = strstr(start_ptr, "#");
      memcpy(msg_hex, start_ptr, end_ptr - start_ptr);
      // Print received data
      app_log("Received data: Source: %s, RSSI: %s, SNR: %s, Data: %s\r\n",
              source,
              rssi,
              snr,
              msg_hex);
      // Toggle LED
      sl_led_toggle(&sl_led_led0);
    }
    // In any other case
    else {
      app_log("Response: %.*s\r\n", rx_len - 1, rx_buffer);
    }
    rx_len = 0;
    rx_buffer[0] = '\0';
  }
}

/***************************************************************************//**
 * Button state change handler.
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    startTx = true;
  }
}

/***************************************************************************//**
 * LoRa RM126x P2P Initialization declaration.
 ******************************************************************************/
void lora_p2p_init(void)
{
  sl_status_t status = SL_STATUS_OK;
  char tmp_buffer[16] = { 0 };

  // Factory reset
  app_log("> Factory reset\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_FACTORY_RESET, NULL);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Warm reset the device
  app_log("> Rebooting device\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_SOFT_RESET, NULL);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set device class to P2P mode
  app_log("> Setting device class to P2P mode\r\n");
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_DEVICE_CLASS,
                      "3");
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set Region to EU868
  app_log("> Setting Region to EU868\r\n");
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_REGION,
                      "1");
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Save settings to the non-volatile memory
  app_log("> Saving settings\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_SAVE_SETTINGS, NULL);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Device Address
  app_log("> Setting P2P Device Address - ID:%s\r\n", LORA_RM126X_P2P_NODE_ID);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_DEVICE_ADDRESS,
                      LORA_RM126X_P2P_NODE_ID);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Network Size
  app_log("> Setting P2P Network Size - Net size:%s\r\n",
          LORA_RM126X_P2P_NETWORK_SIZE);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_NET_SIZE,
                      LORA_RM126X_P2P_NETWORK_SIZE);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P DATA Rate
  app_log("> Setting P2P DATA Rate - Data rate:%s\r\n",
          LORA_RM126X_P2P_DATA_RATE);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_DATA_RATE,
                      LORA_RM126X_P2P_DATA_RATE);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Listen Duration - Class C listen time
  app_log("> Setting P2P Listen Duration - Class C listen time\r\n");
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_LISTEN_DURATION,
                      "0");
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Listen Frequency - Class C listening behavior
  app_log("> Setting P2P Listen Frequency - Class C listening behavior\r\n");
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_LISTEN_INTERVAL,
                      "0");
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Beacon data rate - Class C listening behavior
  app_log("> Setting P2P Beacon data rate - Class C listening behavior\r\n");
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_BEACON_DATA_RATE,
                      "2");
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Tx Power to 1dBm
  app_log("> Setting P2P Tx Power - %sdBm\r\n", LORA_RM126X_P2P_TX_POWER);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_TX_POWER,
                      LORA_RM126X_P2P_TX_POWER);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P allowed packet size
  app_log("> Setting P2P allowed packet size\r\n");
  lora_rm126x_cmd_run(LORA_RM126X_CMD_GET_INFO,
                      LORA_RM126X_PARAM_P2P_SLOT_PACKET_SIZE);

  int16_t packet_size = 0;
  while (1) {
    sl_udelay_wait(100); // 100 us
    status = lora_rm126x_generic_read(rx_buffer, sizeof(rx_buffer), &rx_len);
    if (status == SL_STATUS_OK) {
      app_log("Maximum Allowed packet size: %.*s\r\n", rx_len - 1, rx_buffer);
      packet_size = atoi(&rx_buffer[0]) - 5;
      rx_len = 0;
      rx_buffer[0] = '\0';
      break;
    }
  }
  snprintf(tmp_buffer, sizeof(tmp_buffer), "%d", packet_size);
  app_log("> Setting P2P allowed packet size - %s\r\n", tmp_buffer);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_PACKET_SIZE,
                      tmp_buffer);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Set P2P Windows Length
  app_log("> Setting P2P Windows Length\r\n");
  lora_rm126x_cmd_run(LORA_RM126X_CMD_GET_INFO,
                      LORA_RM126X_PARAM_P2P_MIN_WINDOW_LENGTH);
  int16_t window_length = 0;
  while (1) {
    sl_udelay_wait(100); // 100 us
    status = lora_rm126x_generic_read(rx_buffer, sizeof(rx_buffer), &rx_len);

    if (status == SL_STATUS_OK) {
      app_log("Minimum Allowed window length: %.*s\r\n", rx_len - 1, rx_buffer);
      window_length = atoi(&rx_buffer[0]) * 2;
      rx_len = 0;
      rx_buffer[0] = '\0';
      break;
    }
  }
  snprintf(tmp_buffer, sizeof(tmp_buffer), "%d", window_length);
  app_log("> Setting P2P Windows Length - %s\r\n", tmp_buffer);
  lora_rm126x_cmd_set(LORA_RM126X_CMD_PARAM_ACCESS_NUM,
                      LORA_RM126X_PARAM_ID_P2P_WINDOW_LENGTH,
                      tmp_buffer);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Save settings to the non-volatile memory
  app_log("> Saving settings\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_SAVE_SETTINGS, NULL);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Warm reset the device
  app_log("> Reboot\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_SOFT_RESET, NULL);
  sl_udelay_wait(200000); // 200 ms
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }

  // Start P2P session
  app_log("> Starting P2P session\r\n");
  lora_rm126x_generic_flush();
  lora_rm126x_cmd_run(LORA_RM126X_CMD_P2P_START_SESSION, NULL);
  // Wait for ACK
  status = lora_rm126x_wait_for_ACK();
  if (status != SL_STATUS_OK) {
    app_log("lora_rm126x_wait_for_ACK() error = 0x%lx\r\n", status);
  } else {
    app_log("LoRa RM126x ACK received\r\n");
  }
}
