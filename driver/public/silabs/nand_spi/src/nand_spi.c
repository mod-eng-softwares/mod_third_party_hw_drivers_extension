/***************************************************************************//**
 * @file nand_spi.c
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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sl_core.h"
#include "sl_sleeptimer.h"
#include "nand_spi.h"

#define uptime_tick() sl_sleeptimer_get_tick_count()
#define tick_to_ms(t) sl_sleeptimer_tick_to_ms(t)

#define BSWAP_32(x) ((uint32_t) ((((x) >> 24) & 0xff) | \
           (((x) >> 8) & 0xff00) | \
           (((x) & 0xff00) << 8) | \
           (((x) & 0xff) << 24)))
#define sys_cpu_to_be32(val) BSWAP_32(val)

/* Indicates that an access command includes bytes for the address.
 * If not provided the opcode is not followed by address bytes.
 */
#define NAND_ACCESS_ADDRESSED (1u << 0)

/* Indicates that addressed access uses a 8-bit address regardless of
 * nand_data::flag_8bit_addr.
 */
#define NAND_ACCESS_8BIT_ADDR (1u << 1)

/* Indicates that addressed access uses a 16-bit address regardless of
 * nand_data::flag_16bit_addr.
 */
#define NAND_ACCESS_16BIT_ADDR (1u << 2)

/* Indicates that addressed access uses a 24-bit address regardless of
 * nand_data::flag_32bit_addr.
 */
#define NAND_ACCESS_24BIT_ADDR (1u << 3)

/* Indicates that addressed access uses a 32-bit address regardless of
 * nand_data::flag_32bit_addr.
 */
#define NAND_ACCESS_32BIT_ADDR (1u << 4)

/* Indicates that an access command is performing a write.  If not
 * provided access is a read.
 */
#define NAND_ACCESS_WRITE      (1u << 5)

#define NAND_ACCESS_DUMMY      (1u << 6)



/**
 * struct nand_data - Structure for defining the SPI NAND access
 *
 */
struct nand_data {
	/* Number of bytes per page */
	uint16_t page_size;

	/* Number of oob bytes per page */
	uint8_t oob_size;

	/* Number of pages per block */
	uint16_t page_num;

	/* Number of bytes per block */
	uint32_t block_size;

	/* Number of blocks per chip */
	uint16_t block_num;

	/* Size of flash, in bytes */
	uint64_t flash_size;

	uint8_t page_shift;

	uint8_t block_shift;

	bool continuous_read;

	/* Byte 135-136: Maximum block erase time  */
	uint16_t prg_timeout;

	/* Byte 112: Number of bits ECC correctability */
	uint8_t ecc_bits;
};

/*
 * @brief Send an SPI command
 *
 * @param nand_spi Device struct
 * @param opcode The command to send
 * @param access flags that determine how the command is constructed.
 *        See NAND_ACCESS_*.
 * @param addr The address to send
 * @param data The buffer to store or read the value
 * @param length The size of the buffer
 * @return 0 on success, negative errno code otherwise
 */
static sl_status_t nand_access(struct nand_spi *nand_spi,
                               uint8_t opcode,
                               unsigned int access,
                               off_t addr,
                               void *data, size_t length)
{
  bool is_addressed = (access & NAND_ACCESS_ADDRESSED) != 0U;
  bool is_write = (access & NAND_ACCESS_WRITE) != 0U;
  uint8_t buf[5] = {0};
  uint8_t address_len = 0;
  struct spi_buf spi_buf[2] = {
    {
      .buf = buf,
      .len = 1,
    },
    {
      .buf = data,
      .len = length
    },
  };

  buf[0] = opcode;
  if (is_addressed) {
    union {
      uint32_t u32;
      uint8_t u8[4];
    } addr32 = {
      .u32 = sys_cpu_to_be32(addr),
    };

    if ((access & NAND_ACCESS_32BIT_ADDR) != 0U) {
      address_len = 4;
    } else if ((access & NAND_ACCESS_24BIT_ADDR) != 0U) {
      address_len = 3;
    } else if ((access & NAND_ACCESS_16BIT_ADDR) != 0U) {
      address_len = 2;
    } else if ((access & NAND_ACCESS_8BIT_ADDR) != 0U) {
      address_len = 1;
    }
    memcpy(&buf[1], &addr32.u8[4 - address_len], address_len);
    spi_buf[0].len += address_len;
  };

  if (access & NAND_ACCESS_DUMMY) {
    spi_buf[0].len += 1;
  }

  const struct spi_buf_set tx_set = {
    .buffers = spi_buf,
    .count = (length != 0) ? 2 : 1,
  };

  const struct spi_buf_set rx_set = {
    .buffers = spi_buf,
    .count = 2,
  };

  if (is_write) {
    if (SPI_MASTER_SUCCESS != spi_master_transceive(nand_spi->spi,
                                                    &tx_set,
                                                    NULL)) {
      return SL_STATUS_IO;
    }
  }

  if (SPI_MASTER_SUCCESS != spi_master_transceive(nand_spi->spi,
                                                  &tx_set,
                                                  &rx_set)) {
    return SL_STATUS_IO;
  }
  return SL_STATUS_OK;
}

#define nand_cmd_read(nand_spi, opcode, dest, length)    \
	nand_access(nand_spi, opcode, 0, 0, dest, length)
#define nand_cmd_write(nand_spi, opcode)                 \
  nand_access(nand_spi, opcode, NAND_ACCESS_WRITE, 0, NULL, 0)

/* Everything necessary to acquire owning access to the device.
 *
 * This means taking the lock .
 */
static void acquire_device(struct nand_spi *nand_spi)
{
  (void) nand_spi;
}

/* Everything necessary to release access to the device.
 *
 * This means releasing the lock.
 */
static void release_device(struct nand_spi *nand_spi)
{
  (void) nand_spi;
}

static sl_status_t nand_get_feature(struct nand_spi *nand_spi,
                                uint8_t feature_addr,
                                uint8_t *val)
{
	sl_status_t sc = nand_access(nand_spi, SPI_NAND_CMD_GET_FEATURE,
				  NAND_ACCESS_ADDRESSED | NAND_ACCESS_8BIT_ADDR, feature_addr, val,
				  sizeof(*val));

	return sc;
}

static sl_status_t nand_set_feature(struct nand_spi *nand_spi,
                                uint8_t feature_addr,
                                uint8_t val)
{
	sl_status_t sc = nand_access(nand_spi, SPI_NAND_CMD_SET_FEATURE,
				  NAND_ACCESS_WRITE | NAND_ACCESS_ADDRESSED | NAND_ACCESS_8BIT_ADDR,
				  feature_addr, &val, sizeof(val));

	return sc;
}

/**
 * @brief Wait until the flash is ready
 *
 * @note The device must be externally acquired before invoking this
 * function.
 *
 * This function should be invoked after every ERASE, PROGRAM, or
 * WRITE_STATUS operation before continuing.  This allows us to assume
 * that the device is ready to accept new commands at any other point
 * in the code.
 *
 * @param nand_spi The device structure
 * @return 0 on success, negative errno code otherwise
 */
static sl_status_t nand_wait_until_ready(struct nand_spi *nand_spi)
{
	sl_status_t sc = 0;
	uint8_t reg = 0;
	struct nand_data *data = nand_spi->data;

	uint32_t timeout_ms = data->prg_timeout;
	uint32_t start_time = uptime_tick();

	do {
		sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_STATUS, &reg);

		if (sc != SL_STATUS_OK) {
			return sc;
		}

		if ((reg & SPI_NAND_WIP_BIT) == 0U) {
			return sc;
		}
	} while (tick_to_ms(uptime_tick() - start_time) < timeout_ms);
	return SL_STATUS_TIMEOUT;
}

static sl_status_t nand_conti_read_enable(struct nand_spi *nand_spi, bool conti)
{
	sl_status_t sc;
	uint8_t secur_reg = 0;

	acquire_device(nand_spi);
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out;
	}
	if (conti) {
		secur_reg |= SPINAND_SECURE_BIT_CONT;
	} else {
		secur_reg &= ~SPINAND_SECURE_BIT_CONT;
	}
	sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out;
	}
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out;
	}
	if ((secur_reg & SPINAND_SECURE_BIT_CONT) == 0) {
		sc = SL_STATUS_FAIL;
	}

out:
	release_device(nand_spi);
	return sc;
}

static sl_status_t write_page(struct nand_spi *nand_spi,
                              uint16_t page_addr,
                              off_t offset,
                              void *src, size_t size)
{
  sl_status_t sc = 0;

  nand_cmd_write(nand_spi, SPI_NAND_CMD_WREN);

  sc = nand_access(nand_spi, SPI_NAND_CMD_PP_LOAD,
                   NAND_ACCESS_WRITE
                   | NAND_ACCESS_ADDRESSED
                   | NAND_ACCESS_16BIT_ADDR,
                   offset, (void *)src, size);
  if (sc != SL_STATUS_OK) {
      return sc;
  }

  sc = nand_access(nand_spi, SPI_NAND_CMD_PROGRAM_EXEC,
                   NAND_ACCESS_WRITE
                   | NAND_ACCESS_ADDRESSED
                   | NAND_ACCESS_24BIT_ADDR,
                   page_addr, NULL, 0);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return nand_wait_until_ready(nand_spi);
}

static sl_status_t read_page(struct nand_spi *nand_spi,
                             uint16_t page_addr,
                             off_t offset,
                             void *dest, size_t size)
{
  sl_status_t sc = 0;

  sc = nand_access(nand_spi, SPI_NAND_CMD_PAGE_READ,
                   NAND_ACCESS_ADDRESSED
                   | NAND_ACCESS_24BIT_ADDR,
                   page_addr, NULL, 0);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = nand_wait_until_ready(nand_spi);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return nand_access(nand_spi, SPI_NAND_CMD_READ_CACHE,
                     NAND_ACCESS_ADDRESSED
                     | NAND_ACCESS_16BIT_ADDR
                     | NAND_ACCESS_DUMMY,
                     offset, dest, size);
}

static sl_status_t read_cont(struct nand_spi *nand_spi,
                                  off_t addr,
                                  void *dest, size_t size)
{
	struct nand_data *data = nand_spi->data;
	sl_status_t sc = 0;

	acquire_device(nand_spi);
	sc = nand_access(nand_spi, SPI_NAND_CMD_PAGE_READ,
			      NAND_ACCESS_ADDRESSED | NAND_ACCESS_24BIT_ADDR,
			      addr >> data->page_shift, NULL, 0);
	if (sc != SL_STATUS_OK) {
		goto out;
	}

	sc = nand_wait_until_ready(nand_spi);
	if (sc != SL_STATUS_OK) {
		goto out;
	}

	sc = nand_access(nand_spi, SPI_NAND_CMD_READ_CACHE,
			      NAND_ACCESS_ADDRESSED | NAND_ACCESS_24BIT_ADDR,
			      ((addr >> data->page_shift) & SPI_NAND_PAGE_MASK), dest, size);
	if (sc != SL_STATUS_OK) {
		goto out;
	}

	sc = nand_conti_read_enable(nand_spi, false);

out:
	release_device(nand_spi);

	return sc;
}

static sl_status_t read_normal(struct nand_spi *nand_spi,
                               off_t addr,
                               void *dest, size_t size)
{
	struct nand_data *data = nand_spi->data;
	sl_status_t sc = 0;
	uint32_t offset = 0;
	uint32_t chunk = 0;

	acquire_device(nand_spi);
	while (size > 0) {
		/* Read on _page_size_bytes boundaries (Default 2048 bytes a page) */
		offset = addr % data->page_size;
		chunk = (offset + size < data->page_size) ? size : (data->page_size - offset);

		sc = read_page(nand_spi,
                   addr >> data->page_shift,
                   (addr & SPI_NAND_PAGE_MASK),
                   dest, chunk);
    if (sc != SL_STATUS_OK) {
      goto out;
    }

		dest = (uint8_t *)dest + chunk;

		addr = (addr + SPI_NAND_PAGE_OFFSET) & (~SPI_NAND_PAGE_MASK);
		size -= chunk;
	}

out:
	release_device(nand_spi);

	return sc;
}

sl_status_t nand_read_page(struct nand_spi *nand_spi,
                             uint16_t page,
                             off_t offset,
                             void *dest, size_t size)
{
  struct nand_data *data = nand_spi->data;

  if ((page >= (data->page_num * data->block_num))
      || (offset < 0)
      || ((offset + size) > data->page_size)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return read_page(nand_spi, page, offset, dest, size);
}

sl_status_t nand_read_page_oob(struct nand_spi *nand_spi,
                               uint16_t page,
                               void *dest, size_t size)
{
  struct nand_data *data = nand_spi->data;

  if ((page >= (data->page_num * data->block_num))
      || (size > data->oob_size)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return read_page(nand_spi, page, data->page_size, dest, size);
}

sl_status_t nand_read(struct nand_spi *nand_spi,
                          off_t addr,
                          void *dest, size_t size)
{
	sl_status_t sc = 0;
	struct nand_data *data = nand_spi->data;
	/* should be between 0 and flash size */
	if ((addr < 0) || ((addr + size) > data->flash_size)) {
		return SL_STATUS_INVALID_PARAMETER;
	}
	if (data->ecc_bits == 0) {
		if (data->continuous_read) {
			sc = read_cont(nand_spi, addr, dest, size);
		} else {
			sc = read_normal(nand_spi, addr, dest, size);
		}
	} else {
		return SL_STATUS_NOT_SUPPORTED;
	}
	return sc;
}

sl_status_t nand_write_page(struct nand_spi *nand_spi,
                            uint16_t page,
                            off_t offset,
                            void *dest, size_t size)
{
  struct nand_data *data = nand_spi->data;

  if ((page >= (data->page_num * data->block_num))
      || (offset < 0)
      || ((offset + size) > data->page_size)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return write_page(nand_spi, page, offset, dest, size);
}

sl_status_t nand_write_page_oob(struct nand_spi *nand_spi,
                               uint16_t page,
                               void *dest, size_t size)
{
  struct nand_data *data = nand_spi->data;

  if ((page >= (data->page_num * data->block_num))
      || (size > data->oob_size)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return write_page(nand_spi, page, data->page_size, dest, size);
}

sl_status_t nand_write(struct nand_spi *nand_spi,
                       off_t addr,
                       void *src, size_t size)
{
	struct nand_data *data = nand_spi->data;
	const size_t flash_size = data->flash_size;
	sl_status_t sc = 0;
	uint32_t offset = 0;
	uint32_t chunk = 0;

	if (data->ecc_bits != 0) {
    return SL_STATUS_NOT_SUPPORTED;
	}

	/* should be between 0 and flash size */
	if ((addr < 0) || ((addr + size) > flash_size)) {
		return SL_STATUS_INVALID_PARAMETER;
	}

	acquire_device(nand_spi);

	while (size > 0) {
		/* Don't write more than a page. */
		offset = addr % data->page_size;
		chunk = data->page_size - offset;

		sc = write_page(nand_spi,
                    addr >> data->page_shift,
                    addr & SPI_NAND_PAGE_MASK,
                    src, chunk);
    if (sc != SL_STATUS_OK) {
      goto out;
    }

		src = (uint8_t *)(src) + chunk;
		addr = (addr + SPI_NAND_PAGE_OFFSET) & (~SPI_NAND_PAGE_MASK);
		size -= chunk;
	}

out:
	release_device(nand_spi);
	return sc;
}

static sl_status_t erase_block(struct nand_spi *nand_spi,
                               uint16_t block)
{
  sl_status_t sc;

  nand_cmd_write(nand_spi, SPI_NAND_CMD_WREN);

  sc = nand_access(nand_spi, SPI_NAND_CMD_BE,
                   NAND_ACCESS_ADDRESSED
                   | NAND_ACCESS_24BIT_ADDR,
                   block, NULL, 0);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return nand_wait_until_ready(nand_spi);
}

sl_status_t nand_erase_block(struct nand_spi *nand_spi,
                             uint16_t block)
{
  struct nand_data *data = nand_spi->data;

  if (block > data->block_num) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return erase_block(nand_spi, block << 6);
}

sl_status_t nand_erase(struct nand_spi *nand_spi,
                       off_t addr, size_t size)
{
	struct nand_data *data = nand_spi->data;
	sl_status_t sc = 0;
	/* erase area must be subregion of device */
	if ((addr < 0) || ((size + addr) > data->flash_size)) {
		return SL_STATUS_INVALID_PARAMETER;
	}
	/* address must be block-aligned */
	if ((addr % data->block_size) != 0) {
		return SL_STATUS_INVALID_PARAMETER;
	}
	/* size must be a multiple of blocks */
	if ((size % data->block_size) != 0) {
		return SL_STATUS_INVALID_PARAMETER;
	}

	acquire_device(nand_spi);
	while ((size > 0) && (sc == 0)) {
		nand_cmd_write(nand_spi, SPI_NAND_CMD_WREN);

		sc = nand_access(nand_spi, SPI_NAND_CMD_BE,
				      NAND_ACCESS_ADDRESSED | NAND_ACCESS_24BIT_ADDR,
				      addr >> data->page_shift, NULL, 0);
		if (sc != SL_STATUS_OK) {
			goto out;
		}

    sc = nand_wait_until_ready(nand_spi);
    if (sc != SL_STATUS_OK) {
      goto out;
    }

		addr += SPI_NAND_BLOCK_OFFSET;
		size -= data->block_size;
	}

out:
	release_device(nand_spi);
	return sc;
}

static sl_status_t nand_check_id(struct nand_spi *nand_spi)
{
	const struct nand_config *cfg = nand_spi->config;
	uint8_t const *expected_id = cfg->id;
	uint8_t read_id[SPI_NAND_ID_LEN];

	acquire_device(nand_spi);
	sl_status_t sc = nand_access(nand_spi,
	                             SPI_NAND_CMD_RDID,
	                             NAND_ACCESS_DUMMY,
	                             0,
	                             read_id,
				                       SPI_NAND_ID_LEN);
	release_device(nand_spi);
	if (memcmp(expected_id, read_id, sizeof(read_id)) != 0) {
		return SL_STATUS_NOT_READY;
	}

	return sc;
}

static sl_status_t nand_read_otp_onfi(struct nand_spi *nand_spi)
{
	sl_status_t sc;
	uint8_t secur_reg = 0, onfi_table[256];
	struct nand_data *data = nand_spi->data;

	acquire_device(nand_spi);
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out0;
	}
	secur_reg |= SPINAND_SECURE_BIT_OTP_EN;
	sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out0;
	}
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out0;
	}
	if ((secur_reg & SPINAND_SECURE_BIT_OTP_EN) == 0) {
		sc = SL_STATUS_INVALID_STATE;
	}
out0:
	release_device(nand_spi);
	if (sc != SL_STATUS_OK) {
		return sc;
	}
	sc = read_page(nand_spi,
	               nand_spi->config->onfi_table_page_addr,
	               0,
	               onfi_table, sizeof(onfi_table));
	if (sc != SL_STATUS_OK) {
		// Read ONFI table failed
		return sc;
	}
	if (onfi_table[ONFI_SIG_0] == 'O' && onfi_table[ONFI_SIG_1] == 'N' &&
	    onfi_table[ONFI_SIG_2] == 'F' && onfi_table[ONFI_SIG_3] == 'I') {
		// ONFI table found
		data->page_size = onfi_table[ONFI_PAGE_SIZE_80] +
				  (onfi_table[ONFI_PAGE_SIZE_81] << 8) +
				  (onfi_table[ONFI_PAGE_SIZE_82] << 16);
		data->oob_size = onfi_table[ONFI_OOB_SIZE_84] + (onfi_table[ONFI_OOB_SIZE_85] << 8);
		data->page_num = onfi_table[ONFI_PAGE_NUM_92] + (onfi_table[ONFI_PAGE_NUM_93] << 8);
		data->block_num = onfi_table[ONFI_BLK_NUM_96] + (onfi_table[ONFI_BLK_NUM_97] << 8);
		data->prg_timeout =
			onfi_table[ONFI_BE_TIME_135] + (onfi_table[ONFI_BE_TIME_136] << 8);
		data->block_size = data->page_size * data->page_num;

	  switch (data->page_size) {
	  case 2048:
	    data->page_shift = 12;
	    break;
	  case 4096:
	    data->page_shift = 13;
	    break;
	  default:
	    return SL_STATUS_INVALID_PARAMETER;
	  }

		switch (data->page_num) {
		case 64:
			data->block_shift = data->page_shift + 6;
			break;
		case 128:
			data->block_shift = data->page_shift + 7;
			break;
		case 256:
			data->block_shift = data->page_shift + 8;
			break;
		}
		// Byte 112: Number of bits ECC correctability
		// This field indicates the number of bits
		// that the host should be able to correct per 512 bytes of data
		data->ecc_bits = onfi_table[ONFI_ECC_NUM_112];
		data->flash_size = data->block_num * data->page_num * data->page_size;

		if (data->ecc_bits > 0) {
		  // Software ECC is not supported
			sc = SL_STATUS_NOT_SUPPORTED;
			goto out1;
		} else {
			acquire_device(nand_spi);
			secur_reg |= SPINAND_SECURE_BIT_ECC_EN;
			sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, secur_reg);
			if (sc != SL_STATUS_OK) {
				goto out1;
			}
			release_device(nand_spi);
		}

		if ((onfi_table[ONFI_CONT_READ_168] & 0x02) != 0) {
			data->continuous_read = true;
			sc = nand_conti_read_enable(nand_spi, true);
			if (sc != SL_STATUS_OK) {
				return sc;
			}
		} else {
			data->continuous_read = false;
		}
	} else {
		return SL_STATUS_NOT_FOUND;
	}

	acquire_device(nand_spi);
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out1;
	}

	secur_reg &= ~SPINAND_SECURE_BIT_OTP_EN;
	sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out1;
	}

	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
	if (sc != SL_STATUS_OK) {
		goto out1;
	}

	if ((secur_reg & SPINAND_SECURE_BIT_OTP_EN) != 0) {
		sc = SL_STATUS_INVALID_STATE;
	}

out1:
	release_device(nand_spi);
	return sc;
}

/**
 * @brief Configure the flash
 *
 * @param nand_spi The flash device structure
 * @param info The flash info structure
 * @return 0 on success, negative errno code otherwise
 */
static sl_status_t nand_configure(struct nand_spi *nand_spi)
{
	uint8_t reg = 0;
	sl_status_t sc;

	sc = nand_check_id(nand_spi);
	if (sc != SL_STATUS_OK) {
		return SL_STATUS_NOT_READY;
	}
	/* Check for block protect bits that need to be cleared. */
	acquire_device(nand_spi);
	sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_BLOCK_PROT, &reg);

	if (sc != SL_STATUS_OK) {
		goto out;
	}
	/* Only clear if GET_FEATURE worked and something's set. */
	if ((reg & SPINAND_BLOCK_PROT_BIT_BP_MASK) != 0) {
		reg = 0;
		sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_BLOCK_PROT, reg);
	}
out:
	release_device(nand_spi);

	if (sc != SL_STATUS_OK) {
		return sc;
	}

	// Serial Flash Discoverable Parameter (SFDP)
	sc = nand_read_otp_onfi(nand_spi);
	if (sc != SL_STATUS_OK) {
		return SL_STATUS_NOT_READY;
	}

	return sc;
}

sl_status_t nand_enable_ecc(struct nand_spi *nand_spi, bool enable)
{
  sl_status_t sc;
  uint8_t secur_reg = 0;

  acquire_device(nand_spi);
  sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
  if (sc != SL_STATUS_OK) {
    goto out;
  }

  if (enable) {
    secur_reg |= SPINAND_SECURE_BIT_ECC_EN;
  } else {
    secur_reg &= ~SPINAND_SECURE_BIT_ECC_EN;
  }
  sc = nand_set_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, secur_reg);
  if (sc != SL_STATUS_OK) {
    goto out;
  }

  sc = nand_get_feature(nand_spi, SPI_NAND_FEA_ADDR_CONF_B0, &secur_reg);
  if (sc != SL_STATUS_OK) {
    goto out;
  }

  if ((enable && !(secur_reg & SPINAND_SECURE_BIT_ECC_EN))
      || (!enable && (secur_reg & SPINAND_SECURE_BIT_ECC_EN))) {
    sc = SL_STATUS_INVALID_STATE;
  }

out:
  release_device(nand_spi);
  return sc;
}

static struct nand_data nand_data;

/**
 * @brief Initialize and configure the flash
 *
 * @param name The flash name
 * @return 0 on success, negative errno code otherwise
 */
sl_status_t nand_init(struct nand_spi *nand_spi,
                      spi_master_t *spi,
                      const struct nand_config *config)
{
  nand_spi->spi = spi;
  nand_spi->config = config;
  nand_spi->data = &nand_data;
	return nand_configure(nand_spi);
}

uint32_t nand_get_block_size(struct nand_spi *nand_spi)
{
  return nand_spi->data->block_size;
}

uint16_t nand_get_page_size(struct nand_spi *nand_spi)
{
  return nand_spi->data->page_size;
}

uint8_t nand_get_oob_size(struct nand_spi *nand_spi)
{
  return nand_spi->data->oob_size;
}

uint16_t nand_get_page_num(struct nand_spi *nand_spi)
{
  return nand_spi->data->page_num;
}

uint16_t nand_get_block_num(struct nand_spi *nand_spi)
{
  return nand_spi->data->block_num;
}
