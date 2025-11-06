/***************************************************************************//**
 * @file mikroe_nand_flash_gd5f2g.c
 * @brief SPI nand flash driver
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
#include "drv_spi_master.h"
#include "drv_digital_out.h"
#include "mikroe_gd5f2g_config.h"
#include "mikroe_gd5f2g.h"

static const struct nand_config nand_config = {
  .id = {0xc8, 0x52},
  .onfi_table_page_addr = 0x04
};
static digital_out_t hld;
static digital_out_t wp;

static spi_master_t spi;
static spi_master_config_t spi_config;

sl_status_t mikroe_gd5f2g_init(struct nand_spi *nand_spi)
{
  digital_out_init(&hld, hal_gpio_pin_name(GD5F2G_HLD_PORT, GD5F2G_HLD_PIN));
  digital_out_high(&hld);

  digital_out_init(&wp, hal_gpio_pin_name(GD5F2G_WP_PORT, GD5F2G_WP_PIN));
  digital_out_high(&wp);

  spi_master_configure_default(&spi_config);
  spi_master_open(&spi, &spi_config);
  return nand_init(nand_spi,
                   &spi,
                   &nand_config);
}
