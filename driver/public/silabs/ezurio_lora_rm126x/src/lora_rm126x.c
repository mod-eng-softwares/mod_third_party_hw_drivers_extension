/***************************************************************************//**
 * @file lora_rm126x.c
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

#include "lora_rm126x.h"
#include "sl_status.h"
#include "lora_rm126x_config.h"
#include "sl_iostream.h"
#include "sl_gpio.h"
#include "sl_udelay.h"
#include <stdio.h>
#include <string.h>
#include "sl_sleeptimer.h"

// Static variables for the driver
static sl_iostream_t *lora_rm126x_stream = NULL;
static bool driver_initialized = false;

/***************************************************************************//**
 * @brief
 *    LoRa RM126x internal getchar function.
 * @return
 *    The received character or EOF on error.
 ******************************************************************************/
static int lora_rm126x_internal_getchar(void);

/***************************************************************************//**
 * @brief
 *    LoRa RM126x internal getchar function.
 ******************************************************************************/
int lora_rm126x_internal_getchar(void)
{
  char ch;
  sl_status_t status = sl_iostream_getchar(lora_rm126x_stream, &ch);
  if (status != SL_STATUS_OK) {
    return EOF;
  }

  return ch;
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x initialization function.
 ******************************************************************************/
sl_status_t lora_rm126x_init(sl_iostream_t *uart_handle)
{
  if (uart_handle == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (driver_initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  // Mark the driver as initialized
  driver_initialized = true;

  // Initialize the stream
  lora_rm126x_stream = uart_handle;

  // Init Reset Pin
  sl_gpio_set_pin_mode(LORA_RESET_GPIO, SL_GPIO_MODE_PUSH_PULL, 0);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x deinitialization function.
 ******************************************************************************/
sl_status_t lora_rm126x_deinit(void)
{
  if (!driver_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  driver_initialized = false;
  lora_rm126x_stream = NULL;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic write function.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_write(char *data_buf, size_t len)
{
  if (!driver_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  if (data_buf == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return sl_iostream_write(lora_rm126x_stream, data_buf, len);
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic read function.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_read(char *data_buf,
                                     size_t max_len,
                                     size_t *actual_len)
{
  sl_status_t status = SL_STATUS_OK;

  if (!driver_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  if ((data_buf == NULL) || (actual_len == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  int ch = 0;
  do
  {
    sl_udelay_wait(100);
    ch = lora_rm126x_internal_getchar();

    if ((ch == EOF) && (status != SL_STATUS_IN_PROGRESS)) {
      status = SL_STATUS_EMPTY;
    }
    // The new line character is identified as an response started.
    else if ((ch == '\n') && (status != SL_STATUS_IN_PROGRESS)) {
      status = SL_STATUS_IN_PROGRESS;
      *actual_len = 0;
    }
    // If the received character is a carriage return, response end
    else if ((ch == '\r') && (status == SL_STATUS_IN_PROGRESS)) {
      *actual_len += 1;
      status = SL_STATUS_OK;
      data_buf[*actual_len - 1] = '#';
    }
    // If the response coming, than store the character
    else if (status == SL_STATUS_IN_PROGRESS) {
      // Read the character into the buffer
      *actual_len += 1;
      if (*actual_len < max_len) {
        data_buf[*actual_len - 1] = ch;
      } else {
        status = SL_STATUS_FULL;
      }
    } else {
      // Handle unexpected character
      status = SL_STATUS_FAIL;
    }
  } while (status == SL_STATUS_IN_PROGRESS);

  return status;
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x generic flush function.
 ******************************************************************************/
sl_status_t lora_rm126x_generic_flush(void)
{
  if (!driver_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  uint8_t data_buf[256] = { 0 };
  size_t actual_len = 0;

  sl_status_t status =
    sl_iostream_read(lora_rm126x_stream, data_buf, sizeof(data_buf),
                     &actual_len);

  return status;
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x reset device function.
 ******************************************************************************/
void lora_rm126x_reset_device(void)
{
  if (driver_initialized) {
    sl_gpio_clear_pin(LORA_RESET_GPIO);
    sl_sleeptimer_delay_millisecond(100); // Wait for 100ms
    sl_gpio_set_pin(LORA_RESET_GPIO);
    sl_sleeptimer_delay_millisecond(3000); // Wait for 3s
  }
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command run function.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_run(char *cmd, char *param)
{
  char cmd_buffer[256] = { 0 };
  if (cmd != NULL) {
    if (param != NULL) {
      snprintf(cmd_buffer, sizeof(cmd_buffer), "%s %s\r", cmd, param);
    } else {
      snprintf(cmd_buffer, sizeof(cmd_buffer), "%s\r", cmd);
    }
    return lora_rm126x_generic_write(cmd_buffer, strlen(cmd_buffer));
  } else {
    // Handle error: cmd is NULL
    return SL_STATUS_INVALID_PARAMETER;
  }
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command set function.
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_set(char *cmd, char *param_id, char *value)
{
  char cmd_buffer[256] = { 0 };
  if ((cmd != NULL) && (param_id != NULL) && (value != NULL)) {
    lora_rm126x_generic_flush();
    snprintf(cmd_buffer, sizeof(cmd_buffer), "%s %s=%s\r", cmd, param_id,
             value);
    return lora_rm126x_generic_write(cmd_buffer, strlen(cmd_buffer));
  } else {
    // Handle error: cmd, param_id or value is NULL
    return SL_STATUS_INVALID_PARAMETER;
  }
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command get function.
 *    Queries the specified parameter, with <command> <parameter_id>?
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_get(char *cmd, char *param_id)
{
  char cmd_buffer[256] = { 0 };
  if ((cmd != NULL) && (param_id != NULL)) {
    lora_rm126x_generic_flush();
    snprintf(cmd_buffer, sizeof(cmd_buffer), "%s %s?\r", cmd, param_id);
    return lora_rm126x_generic_write(cmd_buffer, strlen(cmd_buffer));
  } else {
    // Handle error: cmd or param_id is NULL
    return SL_STATUS_INVALID_PARAMETER;
  }
}

/***************************************************************************//**
 * @brief
 *    LoRa RM126x command help function.
 *    Queries the specified parameter, with <command> <parameter_id>=?
 ******************************************************************************/
sl_status_t lora_rm126x_cmd_help(char *cmd, char *param_id)
{
  return lora_rm126x_cmd_set(cmd, param_id, "?");
}

/***************************************************************************//**
 * @brief
 *    It is a blocking function that waits for an acknowledgment from the
 *   device.
 *    Do not use this function in an interrupt context.
 ******************************************************************************/
sl_status_t lora_rm126x_wait_for_ACK(void)
{
  /* Packet structure: \n<data>\r */
  char ack[5] = { 'O', 'K', '\r', 0 };
  char error[7] = { 'E', 'R', 'R', 'O', 'R', ' ', 0 };
  char error_msg[3] = { '1', '0', 0 };
  uint8_t cnt = 0;

  sl_status_t status = SL_STATUS_IS_WAITING;

  int ch = 0;
  do
  {
    ch = lora_rm126x_internal_getchar();

    // Check if the received character is a newline
    if ((ch == '\n') && (status == SL_STATUS_IS_WAITING)) {
      // If a newline is received, reset the ACK and ERROR counters
      status = SL_STATUS_IN_PROGRESS;
    }
    // Check if the received character matches the expected ACK sequence
    else if ((ch == ack[cnt]) && (status == SL_STATUS_IN_PROGRESS)) {
      cnt++;
      status = SL_STATUS_TRANSMIT_BUSY;
    }
    // Check if the received character matches the expected ERROR sequence
    else if ((ch == error[cnt]) && (status == SL_STATUS_IN_PROGRESS)) {
      cnt++;
      status = SL_STATUS_BUS_ERROR;
    }
    // Check if the received character matches the expected ACK sequence
    // And we are in ACK reception
    else if ((ch == ack[cnt]) && (status == SL_STATUS_TRANSMIT_BUSY)) {
      cnt++;
      // Check if the entire ACK sequence has been received
      if (cnt > 2) {
        status = SL_STATUS_OK;
      }
    }
    // Check if the received character matches the expected ERROR sequence
    // And we are in ERROR reception
    else if ((ch == error[cnt]) && (status == SL_STATUS_BUS_ERROR)) {
      cnt++;
      // Check if the entire ERROR sequence has been received
      if (cnt > 4) {
        status = SL_STATUS_NOT_READY;
      }
    }
    // Read the error message
    else if (status == SL_STATUS_NOT_READY) {
      // Handle error case
      if (cnt > 5) {
        error_msg[cnt - 6] = ch;
      }
      cnt++;
      if (cnt > 8) {
        status = SL_STATUS_FAIL;
      }
    } else if ((ch != EOF) && (ch != '\r')) {
      // Handle unexpected character
      cnt = 0;
      status = SL_STATUS_FAIL;
    }
  } while ((status != SL_STATUS_OK) && (status != SL_STATUS_FAIL));

  // Wait for an acknowledgment from the device
  if (status == SL_STATUS_FAIL) {
    return atoi(error_msg);
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Check if the LoRa RM126x driver is initialized.
 ******************************************************************************/
bool lora_rm126x_is_initialized(void)
{
  return driver_initialized;
}
