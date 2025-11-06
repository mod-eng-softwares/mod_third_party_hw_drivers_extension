/***************************************************************************//**
 * @file
 * @brief SPIDRV Config
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
 ******************************************************************************/

#ifndef SPI_USART_EFX_CONFIG_H
#define SPI_USART_EFX_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> SPIDRV settings

// <o SPI_USART_EFX_BITRATE> SPI bitrate
// <i> Default: 1000000
#define SPI_USART_EFX_BITRATE           1000000

// <o SPI_USART_EFX_CS_CONTROL> SPI master chip select (CS) control scheme.
// <0=> CS controlled by the SPI driver
// <1=> CS controlled by the application
#define SPI_USART_EFX_CS_CONTROL         0
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <usart signal=TX,RX,CLK,(CS)> SPI_USART_EFX
// $[USART_SPI_USART_EFX]
 #define SPI_USART_EFX_PERIPHERAL        USART0
 #define SPI_USART_EFX_PERIPHERAL_NO     0

 #define SPI_USART_EFX_TX_PORT           SL_GPIO_PORT_C
 #define SPI_USART_EFX_TX_PIN            3

 #define SPI_USART_EFX_RX_PORT           SL_GPIO_PORT_C
 #define SPI_USART_EFX_RX_PIN            2

 #define SPI_USART_EFX_CLK_PORT          SL_GPIO_PORT_C
 #define SPI_USART_EFX_CLK_PIN           1

 #define SPI_USART_EFX_CS_PORT           SL_GPIO_PORT_C
 #define SPI_USART_EFX_CS_PIN            0
// [USART_SPI_USART_EFX]$
// <<< sl:end pin_tool >>>

#endif // SPI_USART_EFX_CONFIG_H
