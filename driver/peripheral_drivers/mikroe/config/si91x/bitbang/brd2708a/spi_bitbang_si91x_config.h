/***************************************************************************//**
 * @file spi_bitbang_si91x_config
 * @brief Mikroe SPI bitbang for Si91x
 * @version 1.2.0
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

#ifndef SPI_BITBANG_SI91X_CONFIG_H
#define SPI_BITBANG_SI91X_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> SPIDRV settings

// <o SPI_USART_EFX_BITRATE> SPI bitrate
// <i> Default: 1000000
#define SPI_BITBANG_SI91X_BITRATE                1000000

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> SPI_BITBANG_SI91X_CLK
// $[GPIO_SPI_BITBANG_SI91X_CLK]
#define SPI_BITBANG_SI91X_CLK_PORT                    HP
#define SPI_BITBANG_SI91X_CLK_PIN                     25
// [GPIO_SPI_BITBANG_SI91X_CLK]$

// <gpio> SPI_BITBANG_SI91X_MISO
// $[GPIO_SPI_BITBANG_SI91X_MISO]
#define SPI_BITBANG_SI91X_MISO_PORT                   HP
#define SPI_BITBANG_SI91X_MISO_PIN                    26
// [GPIO_SPI_BITBANG_SI91X_MISO]$

// <gpio> SPI_BITBANG_SI91X_MOSI
// $[GPIO_SPI_BITBANG_SI91X_MOSI]
#define SPI_BITBANG_SI91X_MOSI_PORT                   HP
#define SPI_BITBANG_SI91X_MOSI_PIN                    27
// [GPIO_SPI_BITBANG_SI91X_MOSI]$

// <gpio> SPI_BITBANG_SI91X_CS
// $[GPIO_SPI_BITBANG_SI91X_CS]
#define SPI_BITBANG_SI91X_CS_PORT                     HP
#define SPI_BITBANG_SI91X_CS_PIN                      28
// [GPIO_SPI_BITBANG_SI91X_CS]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SPI_BITBANG_SI91X_CONFIG_H
