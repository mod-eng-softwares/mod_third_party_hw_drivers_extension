/***************************************************************************//**
 * @file drv_spi_master_transceive.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - SPI Master
 * @version 1.0.0
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
#include "drv_spi_master.h"

#define buffer_is_valid(buf, len)  (((buf) != NULL && (len) != 0) \
                                    || ((buf) == NULL && (len) == 0))

static spi_master_t *_owner = NULL;

static err_t _acquire(spi_master_t *obj, bool obj_open_state);

err_t spi_master_transceive(spi_master_t *obj,
                            const struct spi_buf_set *tx_bufs,
                            const struct spi_buf_set *rx_bufs)
{
  const struct spi_buf *tx_buf;
  const struct spi_buf *rx_buf;
  size_t tx_count;
  size_t rx_count;
  err_t ret = SPI_MASTER_SUCCESS;

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (tx_bufs) {
    if (buffer_is_valid(tx_bufs->buffers, tx_bufs->count)) {
      tx_count = tx_bufs->count;
      tx_buf = tx_bufs->buffers;
    } else {
      return SPI_MASTER_ERROR;
    }
  } else {
    tx_count = 0;
    tx_buf = NULL;
  }
  if (rx_bufs) {
    if (buffer_is_valid(rx_bufs->buffers, rx_bufs->count)) {
      rx_count = rx_bufs->count;
      rx_buf = rx_bufs->buffers;
    } else {
      return SPI_MASTER_ERROR;
    }
  } else {
    rx_count = 0;
    rx_buf = NULL;
  }

  if (SPI_MASTER_SUCCESS != spi_device_configure(obj)) {
    return SPI_MASTER_ERROR;
  }

  spi_device_cs_control(obj, true);
  while (tx_count && rx_count) {
    ret = spi_device_transceive(obj,
                                tx_buf++,
                                rx_buf++);
    if (SPI_MASTER_SUCCESS != ret) {
      goto out;
    }
    tx_count--;
    rx_count--;
  }
  while (tx_count) {
    ret = spi_device_transceive(obj,
                                tx_buf++,
                                NULL);
    if (SPI_MASTER_SUCCESS != ret) {
      goto out;
    }
    tx_count--;
  }
  while (rx_count) {
    ret = spi_device_transceive(obj,
                                NULL,
                                rx_buf++);
    if (SPI_MASTER_SUCCESS != ret) {
      goto out;
    }
    rx_count--;
  }

out:
  spi_device_cs_control(obj, false);
  return ret;
}

/***************************************************************************//**
 * Write byte to SPI bus.
 ******************************************************************************/
err_t spi_master_write(spi_master_t *obj,
                       uint8_t *tx_buf,
                       size_t tx_len)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (SPI_MASTER_SUCCESS != spi_device_configure(obj)) {
    return SPI_MASTER_ERROR;
  }

  struct spi_buf spi_tx_buf = {
      .buf = tx_buf,
      .len = tx_len,
  };

  if (SPI_MASTER_SUCCESS != spi_device_transceive(obj,
                                                  &spi_tx_buf,
                                                  NULL)) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Read byte from SPI bus.
 ******************************************************************************/
err_t spi_master_read(spi_master_t *obj,
                      uint8_t *rx_buf,
                      size_t rx_len)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (SPI_MASTER_SUCCESS != spi_device_configure(obj)) {
    return SPI_MASTER_ERROR;
  }

  struct spi_buf spi_rx_buf = {
      .buf = rx_buf,
      .len = rx_len
  };

  if (SPI_MASTER_SUCCESS != spi_device_transceive(obj,
                                                  NULL,
                                                  &spi_rx_buf)) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Exchange bytes on SPI bus.
 ******************************************************************************/
err_t spi_master_exchange(spi_master_t *obj,
                          uint8_t *tx_buf,
                          uint8_t *rx_buf,
                          size_t length)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (SPI_MASTER_SUCCESS != spi_device_configure(obj)) {
    return SPI_MASTER_ERROR;
  }

  struct spi_buf spi_tx_buf = {
      .buf = tx_buf,
      .len = length,
  };
  struct spi_buf spi_rx_buf = {
      .buf = rx_buf,
      .len = length
  };

  if (SPI_MASTER_SUCCESS != spi_device_transceive(obj,
                                                  &spi_tx_buf,
                                                  &spi_rx_buf)) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Perform a sequence of SPI Master writes
 * immediately followed by a SPI Master read.
 ******************************************************************************/
err_t spi_master_write_then_read(spi_master_t *obj,
                                 uint8_t *tx_buf,
                                 size_t tx_len,
                                 uint8_t *rx_buf,
                                 size_t rx_len)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (SPI_MASTER_SUCCESS != spi_device_configure(obj)) {
    return SPI_MASTER_ERROR;
  }

  struct spi_buf spi_tx_buf = {
      .buf = tx_buf,
      .len = tx_len,
  };
  struct spi_buf spi_rx_buf = {
      .buf = rx_buf,
      .len = rx_len
  };

  if (SPI_MASTER_SUCCESS != spi_device_transceive(obj,
                                                  &spi_tx_buf,
                                                  NULL)) {
    return SPI_MASTER_ERROR;
  }

  if (SPI_MASTER_SUCCESS != spi_device_transceive(obj,
                                                  NULL,
                                                  &spi_rx_buf)) {
    return SPI_MASTER_ERROR;
  }

  return SPI_MASTER_SUCCESS;
}

static err_t _acquire(spi_master_t *obj, bool obj_open_state)
{
  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return ACQUIRE_SUCCESS;
}

err_t spi_master_open(spi_master_t *obj, spi_master_config_t *config)
{
  spi_device_open(obj, config);
  return _acquire(obj, true);
}

void spi_master_close(spi_master_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

/***************************************************************************//**
 * Set SPI Master driver communication speed.
 ******************************************************************************/
err_t spi_master_set_speed(spi_master_t *obj, uint32_t speed)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }
  return spi_device_set_speed(obj, speed);
}

/***************************************************************************//**
 * Set SPI Master driver communication mode.
 ******************************************************************************/
err_t spi_master_set_mode(spi_master_t *obj, spi_master_mode_t mode)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }
  obj->config.mode = mode;

  return spi_device_configure(obj);
}

/***************************************************************************//**
 * Set SPI Master driver default ( dummy ) write data.
 ******************************************************************************/
err_t spi_master_set_default_write_data(spi_master_t *obj,
                                        uint8_t default_write_data)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.default_write_data = default_write_data;
  return SPI_MASTER_SUCCESS;
}

void spi_master_set_chip_select_polarity(
  spi_master_chip_select_polarity_t polarity)
{
  spi_device_set_chip_select_polarity(polarity);
}

void spi_master_select_device(pin_name_t chip_select)
{
  spi_device_select_device(chip_select);
}

void spi_master_deselect_device(pin_name_t chip_select)
{
  spi_device_deselect_device(chip_select);
}

// ----------------------------------------------------------------------- END

