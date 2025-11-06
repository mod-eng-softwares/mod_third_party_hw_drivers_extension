/***************************************************************************//**
 * @file nand_spi.h
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

#ifndef _NAND_SPI_H_
#define _NAND_SPI_H_

#include <stdio.h>
#include "sl_status.h"
#include "drv_spi_master.h"

#define SPI_NAND_ID_LEN 2

/* Status register bits */
#define SPI_NAND_WIP_BIT             (1 << 0) /* Write in progress */

/* Get/Set Feature Address Definition */
#define SPI_NAND_FEA_ADDR_BLOCK_PROT 0xA0
#define SPI_NAND_FEA_ADDR_CONF_B0    0xB0
#define SPI_NAND_FEA_ADDR_STATUS     0xC0
#define SPI_NAND_FEA_ADDR_RECOVERY   0x70

/* Secure OTP Register Bits  0xB0 */
#define SPINAND_SECURE_BIT_CONT   0x04 /* continuous read enable */
#define SPINAND_SECURE_BIT_ECC_EN 0x10 /* On-die ECC enable */
#define SPINAND_SECURE_BIT_OTP_EN 0x40

/* Block Protection Register Bits 0xA0 */
#define SPINAND_BLOCK_PROT_BIT_BP_MASK 0x38

/* Flash opcodes */
#define SPI_NAND_CMD_RDID 0x9F /* Read ID */

#define SPI_NAND_CMD_PAGE_READ  0x13 /* Read data from array to cache */
#define SPI_NAND_CMD_READ_CACHE 0x03 /* Read data from cache*/

#define SPI_NAND_CMD_WREN         0x06 /* Write enable */
#define SPI_NAND_CMD_PP_LOAD      0x02 /* Load data to cache*/
#define SPI_NAND_CMD_PROGRAM_EXEC 0x10 /* Execute program */
#define SPI_NAND_CMD_BE           0xD8 /* Block erase */

#define SPI_NAND_CMD_GET_FEATURE   0x0F
#define SPI_NAND_CMD_SET_FEATURE   0x1F
#define SPI_NAND_CMD_RESET         0xFF
#define SPI_NAND_CMD_ECC_STAT_READ 0x7C

#define SPI_NAND_BLOCK_OFFSET 0x40000
#define SPI_NAND_PAGE_OFFSET  0x1000
#define SPI_NAND_BLOCK_MASK   0x3FFFF
#define SPI_NAND_PAGE_MASK    0xFFF

#define ONFI_SIG_0         0
#define ONFI_SIG_1         1
#define ONFI_SIG_2         2
#define ONFI_SIG_3         3
#define ONFI_PAGE_SIZE_80  80
#define ONFI_PAGE_SIZE_81  81
#define ONFI_PAGE_SIZE_82  82
#define ONFI_OOB_SIZE_84   84
#define ONFI_OOB_SIZE_85   85
#define ONFI_PAGE_NUM_92   92
#define ONFI_PAGE_NUM_93   93
#define ONFI_BLK_NUM_96    96
#define ONFI_BLK_NUM_97    97
#define ONFI_ECC_NUM_112   112
#define ONFI_BE_TIME_135   135
#define ONFI_BE_TIME_136   136
#define ONFI_CONT_READ_168 168

/* Build-time data associated with the device. */
struct nand_config {
  uint8_t id[2];
  uint8_t onfi_table_page_addr;
};

struct nand_spi {
  const struct nand_config *config;
  spi_master_t *spi;
  struct nand_data *data;
};

sl_status_t nand_erase(struct nand_spi *nand_spi,
                       off_t addr, size_t size);

sl_status_t nand_erase_block(struct nand_spi *nand_spi,
                             uint16_t block);

sl_status_t nand_write(struct nand_spi *nand_spi,
                       off_t addr,
                       void *src, size_t size);

sl_status_t nand_write_page(struct nand_spi *nand_spi,
                            uint16_t page,
                            off_t offset,
                            void *dest, size_t size);

sl_status_t nand_write_page_oob(struct nand_spi *nand_spi,
                               uint16_t page,
                               void *dest, size_t size);

sl_status_t nand_read_page(struct nand_spi *nand_spi,
                           uint16_t page,
                           off_t offset,
                           void *dest, size_t size);

sl_status_t nand_read_page_oob(struct nand_spi *nand_spi,
                               uint16_t page,
                               void *dest, size_t size);

sl_status_t nand_read(struct nand_spi *nand_spi,
                      off_t addr,
                      void *dest, size_t size);

sl_status_t nand_init(struct nand_spi *nand_spi,
                      spi_master_t *spi,
                      const struct nand_config *config);

uint32_t nand_get_block_size(struct nand_spi *nand_spi);

uint16_t nand_get_page_size(struct nand_spi *nand_spi);

uint8_t nand_get_oob_size(struct nand_spi *nand_spi);

uint16_t nand_get_page_num(struct nand_spi *nand_spi);

uint16_t nand_get_block_num(struct nand_spi *nand_spi);

sl_status_t nand_enable_ecc(struct nand_spi *nand_spi, bool enable);

#endif /*_NAND_SPI_H_*/
