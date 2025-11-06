/***************************************************************************//**
 * @file   portserial_si91x.c
 * @brief  Porting Serial for Si91x devices
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "mb.h"
#include "mbport.h"
#include "mb_port_config.h"

#include "sl_si91x_usart.h"

static sl_usart_handle_t usart_handle;
static sl_si91x_usart_control_config_t _usart_config;
static BOOL tx_enabled = false;
static volatile BOOL rx_enabled = false;
static CHAR rx_byte;

void usart_callback_event(uint32_t event);

static usart_parity_typedef_t uart_parity_mapping(eMBParity parity)
{
  usart_parity_typedef_t val;

  switch (parity) {
    default:
    case MB_PAR_NONE:
      val = SL_USART_NO_PARITY;
      break;
    case MB_PAR_EVEN:
      val = SL_USART_EVEN_PARITY;
      break;
    case MB_PAR_ODD:
      val = SL_USART_ODD_PARITY;
      break;
  }
  return val;
}

static usart_stopbit_typedef_t uart_stopbit_mapping(UCHAR stop_bits)
{
  if (stop_bits == 2) {
    return SL_USART_STOP_BITS_2;
  }

  return SL_USART_STOP_BITS_1;
}

static usart_databits_typedef_t uart_databits_mapping(UCHAR data_bits)
{
  usart_databits_typedef_t val;

  switch (data_bits) {
    case 5:
      val = SL_USART_DATA_BITS_5;
      break;
    case 6:
      val = SL_USART_DATA_BITS_6;
      break;
    case 7:
      val = SL_USART_DATA_BITS_7;
      break;
    default:
    case 8:
      val = SL_USART_DATA_BITS_8;
      break;
  }
  return val;
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity, UCHAR ucStopBits)
{
  sl_status_t status;

  _usart_config.baudrate = ulBaudRate;
  _usart_config.parity = uart_parity_mapping(eParity);
  _usart_config.stopbits = uart_stopbit_mapping(ucStopBits);
  _usart_config.databits = uart_databits_mapping(ucDataBits);
  _usart_config.mode = SL_USART_MODE_ASYNCHRONOUS;
  _usart_config.hwflowcontrol = SL_USART_FLOW_CONTROL_NONE;
  _usart_config.usart_module = ucPORT;

  // Initialize the UART
  status = sl_si91x_usart_init((usart_peripheral_t)_usart_config.usart_module,
                               &usart_handle);
  if (status != SL_STATUS_OK) {
    return FALSE;
  }
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(usart_handle,
                                                    &_usart_config);
  if (status != SL_STATUS_OK) {
    return FALSE;
  }
  // Register user callback function
  status =
    sl_si91x_usart_multiple_instance_register_event_callback(
      (usart_peripheral_t)_usart_config.usart_module,
      usart_callback_event);
  if (status != SL_STATUS_OK) {
    return FALSE;
  }

  return TRUE;
}

void vMBPortSerialEnable(BOOL rxEnable, BOOL txEnable)
{
  // Configure receive interrupt
  rx_enabled = rxEnable;
  tx_enabled = txEnable;
  if (tx_enabled) {
    pxMBFrameCBTransmitterEmpty();
  }
  if (rx_enabled) {
    sl_si91x_usart_receive_data(usart_handle, &rx_byte, 1); // start to get incomming data
  }
}

BOOL xMBPortSerialPutByte(CHAR byte)
{
  sl_si91x_usart_send_data(usart_handle, &byte, 1);
  return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *byte)
{
  *byte = rx_byte;
  sl_si91x_usart_receive_data(usart_handle, &rx_byte, 1);
  return TRUE;
}

/*******************************************************************************
 * Callback function triggered on data Transfer and reception
 ******************************************************************************/
void usart_callback_event(uint32_t event)
{
  switch (event) {
    case SL_USART_EVENT_SEND_COMPLETE:
      if (tx_enabled) {
        pxMBFrameCBTransmitterEmpty();
      }
      break;
    case SL_USART_EVENT_RECEIVE_COMPLETE:
      if (rx_enabled) {
        pxMBFrameCBByteReceived();
      }
      break;
  }
}
