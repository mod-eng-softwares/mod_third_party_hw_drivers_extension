/***************************************************************************//**
 * @file   portserial_efx.c
 * @brief  Porting Serial for EFx devices
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

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#define USART_CONCAT2(x, y)                      x ## y
#define USART_CONCAT_DEV(periph_nbr) \
  USART_CONCAT2(USART, periph_nbr)
#define USART_CONCAT_CLK(periph_nbr) \
  USART_CONCAT2(cmuClock_USART, periph_nbr)
#define USART_CONCAT3(x, y, z)                   x ## y ## z
#define USART_CONCAT_TX_IRQ_NUMBER(periph_nbr) \
  USART_CONCAT3(USART, periph_nbr, _TX_IRQn)
#define USART_CONCAT_RX_IRQ_NUMBER(periph_nbr) \
  USART_CONCAT3(USART, periph_nbr, _RX_IRQn)
#define USART_CONCAT_TX_IRQ_HANDLER(periph_nbr) \
  USART_CONCAT3(USART, periph_nbr, _TX_IRQHandler)
#define USART_CONCAT_RX_IRQ_HANDLER(periph_nbr) \
  USART_CONCAT3(USART, periph_nbr, _RX_IRQHandler)

#define USART_DEV            USART_CONCAT_DEV(MBUSART_USART_NO)
#define USART_CLK            USART_CONCAT_CLK(MBUSART_USART_NO)
#define USART_TX_IRQ         USART_CONCAT_TX_IRQ_NUMBER(MBUSART_USART_NO)
#define USART_RX_IRQ         USART_CONCAT_RX_IRQ_NUMBER(MBUSART_USART_NO)
#define USART_TX_IRQHandler  USART_CONCAT_TX_IRQ_HANDLER(MBUSART_USART_NO)
#define USART_RX_IRQHandler  USART_CONCAT_RX_IRQ_HANDLER(MBUSART_USART_NO)

static USART_Databits_TypeDef data_bits_mapping(UCHAR data_len)
{
  USART_Databits_TypeDef len;
  switch (data_len) {
    case 4:
      len = usartDatabits4;
      break;
    case 5:
      len = usartDatabits5;
      break;
    case 6:
      len = usartDatabits6;
      break;
    case 7:
      len = usartDatabits7;
      break;
    case 8:
      len = usartDatabits8;
      break;
    case 9:
      len = usartDatabits9;
      break;
    case 10:
      len = usartDatabits10;
      break;
    case 11:
      len = usartDatabits11;
      break;
    case 12:
      len = usartDatabits12;
      break;
    case 13:
      len = usartDatabits13;
      break;
    case 14:
      len = usartDatabits14;
      break;
    case 15:
      len = usartDatabits15;
      break;
    case 16:
      len = usartDatabits16;
      break;
    default:
      len = usartDatabits8;
      break;
  }
  return len;
}

static USART_Parity_TypeDef parity_mapping(eMBParity parity)
{
  if (parity == MB_PAR_EVEN) {
    return usartEvenParity;
  } else if (parity == MB_PAR_ODD) {
    return usartOddParity;
  }

  return usartNoParity;
}

static USART_Stopbits_TypeDef stop_bits_mapping(UCHAR stopbits)
{
  if (stopbits == 2) {
    return usartStopbits2;
  }

  return usartStopbits1;
}

BOOL xMBPortSerialInit(UCHAR ucPORT,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity,
                       UCHAR ucStopBits)
{
  (void)ucPORT;
  // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  // Enable clock to GPIO
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Configure TX and RX GPIOs
  GPIO_PinModeSet(MBUSART_TX_PORT, MBUSART_TX_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(MBUSART_RX_PORT, MBUSART_RX_PIN, gpioModeInputPull, 1);

  // Enable clock to USARTx
  CMU_ClockEnable(USART_CLK, true);

  init.baudrate = ulBaudRate;
  init.databits = data_bits_mapping(ucDataBits);
  init.parity = parity_mapping(eParity);
  init.stopbits = stop_bits_mapping(ucStopBits);
  init.enable = usartDisable;

  // Enable RX and TX signals now that they have been routed
  GPIO->USARTROUTE[MBUSART_USART_NO].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN
                                               | GPIO_USART_ROUTEEN_TXPEN;
  // Route USARTx TX and RX to the board controller TX and RX pins
  GPIO->USARTROUTE[MBUSART_USART_NO].TXROUTE =
    (MBUSART_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | (MBUSART_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[MBUSART_USART_NO].RXROUTE =
    (MBUSART_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | (MBUSART_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

  // Configure USART for basic async operation
  USART_InitAsync(USART_DEV, &init);

  USART_IntDisable(USART_DEV, USART_IEN_RXDATAV);
  USART_IntDisable(USART_DEV, USART_IEN_TXBL);
  NVIC_DisableIRQ(USART_TX_IRQ);
  NVIC_DisableIRQ(USART_RX_IRQ);

  // Finally enable USART
  USART_Enable(USART_DEV, usartEnable);

  return TRUE;
}

void vMBPortSerialEnable(BOOL rxEnable, BOOL txEnable)
{
  // Disable interrupts during configuration
  NVIC_DisableIRQ(USART_TX_IRQ);
  NVIC_DisableIRQ(USART_RX_IRQ);

  // Configure receive interrupt
  if (rxEnable) {
    USART_IntEnable(USART_DEV, USART_IEN_RXDATAV);
    NVIC_EnableIRQ(USART_RX_IRQ);
  } else {
    USART_IntDisable(USART_DEV, USART_IEN_RXDATAV);
  }
  // Configure transmit interrupt
  if (txEnable) {
    USART_IntEnable(USART_DEV, USART_IEN_TXBL);
    NVIC_EnableIRQ(USART_TX_IRQ);
  } else {
    USART_IntDisable(USART_DEV, USART_IEN_TXBL);
  }
}

BOOL xMBPortSerialPutByte(CHAR byte)
{
  USART_DEV->TXDATA = (uint32_t)byte;
  return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *byte)
{
  *byte = (uint8_t)USART_DEV->RXDATA;
  return TRUE;
}

void USART_RX_IRQHandler(void)
{
  pxMBFrameCBByteReceived();
}

/**************************************************************************//**
 * @brief
 *    The USARTx transmit interrupt outputs characters.
 *****************************************************************************/
void USART_TX_IRQHandler(void)
{
  pxMBFrameCBTransmitterEmpty();
}

/* ----------------------- End of file --------------------------------------*/
