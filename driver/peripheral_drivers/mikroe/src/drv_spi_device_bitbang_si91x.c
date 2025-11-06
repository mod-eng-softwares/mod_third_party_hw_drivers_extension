/***************************************************************************//**
 * @file drv_spi_master.c
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
#include "drv_digital_out.h"
#include "sl_si91x_driver_gpio.h"
#include "spi_bitbang_si91x_config.h"

#define DELAY_1US_COUNTER           4             // Delay count

#define delay_1us(n) \
  for (volatile uint32_t x = 0; x < DELAY_1US_COUNTER * (n); x++) {  \
    __NOP();                                                              \
  }
#define set_pin(pin)                                   \
    sl_gpio_set_pin_output(hal_gpio_port_index(pin),   \
                           hal_gpio_pin_index(pin))
#define clear_pin(pin)                                 \
    sl_gpio_clear_pin_output(hal_gpio_port_index(pin), \
                           hal_gpio_pin_index(pin))
#define pin_is_set(pin)                                \
    sl_gpio_get_pin_input(hal_gpio_port_index(pin),    \
                          hal_gpio_pin_index(pin))

// Minimum frequency is 500Khz
#define clock_period(speed)                            \
  (speed) > 500000 ? 0 : ((uint32_t)1000000 / (speed))

#define clock_delay(n)                                 \
    if (n) delay_1us(n)

static spi_master_chip_select_polarity_t spi_master_chip_select_polarity =
  SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY;

static err_t gspi_init(const spi_master_config_t *config);
static void spi_device_configure_gpio_pin(digital_out_t *out,
                                          pin_name_t name);

void spi_master_configure_default(spi_master_config_t *config)
{
  config->default_write_data = 0;
  config->sck = hal_gpio_pin_name(SPI_BITBANG_SI91X_CLK_PORT,
                                  SPI_BITBANG_SI91X_CLK_PIN);
  config->miso = hal_gpio_pin_name(SPI_BITBANG_SI91X_MISO_PORT,
                                   SPI_BITBANG_SI91X_MISO_PIN);
  config->mosi = hal_gpio_pin_name(SPI_BITBANG_SI91X_MOSI_PORT,
                                   SPI_BITBANG_SI91X_MOSI_PIN);
  config->cs = hal_gpio_pin_name(SPI_BITBANG_SI91X_CS_PORT,
                                 SPI_BITBANG_SI91X_CS_PIN);
  config->mode = SPI_MASTER_MODE_DEFAULT;
  config->cs_mode = SPI_MASTER_CS_MODE_SW;
  config->speed = clock_period(SPI_BITBANG_SI91X_BITRATE);
  config->spi_instance = NULL;
}

err_t spi_device_open(spi_master_t *obj, spi_master_config_t *config)
{
  obj->config = *config;

  obj->handle = NULL;
  if (SPI_MASTER_SUCCESS != gspi_init(config)) {
    return SPI_MASTER_ERROR;
  }
  obj->handle = config;
  return SPI_MASTER_SUCCESS;
}

void spi_device_select_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_device_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_high(&struct_cs);
  } else {
    digital_out_low(&struct_cs);
  }
}

void spi_device_deselect_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_device_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_low(&struct_cs);
  } else {
    digital_out_high(&struct_cs);
  }
}

void spi_device_set_chip_select_polarity(
  spi_master_chip_select_polarity_t polarity)
{
  if (polarity == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH;
  } else {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
  }
}

err_t spi_device_cs_control(spi_master_t *obj, bool on)
{
  if (!obj) {
    return SPI_MASTER_ERROR;
  }
  spi_master_config_t *config = &(obj->config);

  if (on
      && (spi_master_chip_select_polarity
          == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW)) {
    clear_pin(config->cs);
  } else {
    set_pin(config->cs);
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Set SPI Master driver communication speed.
 ******************************************************************************/
err_t spi_device_set_speed(spi_master_t *obj, uint32_t speed)
{
  if (!obj) {
    return SPI_MASTER_ERROR;
  }

  obj->config.speed = clock_period(speed);

  return SPI_MASTER_SUCCESS;
}

err_t spi_device_configure(spi_master_t *obj)
{
  (void) obj;
  return SPI_MASTER_SUCCESS;
}

static err_t gspi_init(const spi_master_config_t *config)
{
  sl_si91x_gpio_pin_config_t sl_gpio_pin_config;

  sl_gpio_pin_config.port_pin.port = hal_gpio_port_index(config->sck);
  sl_gpio_pin_config.port_pin.pin = hal_gpio_pin_index(config->sck);
  sl_gpio_pin_config.direction = GPIO_OUTPUT;
  if (SL_STATUS_OK != sl_gpio_set_configuration(sl_gpio_pin_config)) {
    return SPI_MASTER_ERROR;
  }
  // clock idle state
  if (config->mode == SPI_MASTER_MODE_0
      || config->mode == SPI_MASTER_MODE_1) {
    sl_gpio_driver_clear_pin(&sl_gpio_pin_config.port_pin);
  } else {
    sl_gpio_driver_set_pin(&sl_gpio_pin_config.port_pin);
  }

  sl_gpio_pin_config.port_pin.port = hal_gpio_port_index(config->miso);
  sl_gpio_pin_config.port_pin.pin = hal_gpio_pin_index(config->miso);
  sl_gpio_pin_config.direction = GPIO_INPUT;
  if (SL_STATUS_OK != sl_gpio_set_configuration(sl_gpio_pin_config)) {
    return SPI_MASTER_ERROR;
  }
  sl_gpio_driver_clear_pin(&sl_gpio_pin_config.port_pin);

  sl_gpio_pin_config.port_pin.port = hal_gpio_port_index(config->mosi);
  sl_gpio_pin_config.port_pin.pin = hal_gpio_pin_index(config->mosi);
  sl_gpio_pin_config.direction = GPIO_OUTPUT;
  if (SL_STATUS_OK != sl_gpio_set_configuration(sl_gpio_pin_config)) {
    return SPI_MASTER_ERROR;
  }
  sl_gpio_driver_clear_pin(&sl_gpio_pin_config.port_pin);

  sl_gpio_pin_config.port_pin.port = hal_gpio_port_index(config->cs);
  sl_gpio_pin_config.port_pin.pin = hal_gpio_pin_index(config->cs);
  sl_gpio_pin_config.direction = GPIO_OUTPUT;
  if (SL_STATUS_OK != sl_gpio_set_configuration(sl_gpio_pin_config)) {
    return SPI_MASTER_ERROR;
  }
  sl_gpio_driver_set_pin(&sl_gpio_pin_config.port_pin);

  return SPI_MASTER_SUCCESS;
}

static void spi_device_configure_gpio_pin(digital_out_t *out,
                                          pin_name_t name)
{
  out->pin.base = hal_gpio_port_index(name);
  out->pin.mask = hal_gpio_pin_index(name);
}

static inline uint8_t exchange_byte_mode0(const spi_master_config_t *config,
                                          uint8_t tx)
{
  uint8_t rx = 0;

  clock_delay(config->speed); // first clock idle state
  for (uint8_t bit = 0; bit < 8; bit++) {
    if (tx & 0x80) {
      set_pin(config->mosi);
    } else {
      clear_pin(config->mosi);
    }
    tx <<= 1;
    set_pin(config->sck);            // transmit edge: positive
    clock_delay(config->speed);                   // wait for rx data ready
    rx <<= 1;
    if (pin_is_set(config->miso)) {
      rx |= 0x01;
    }
    clear_pin(config->sck);         // clock idle state
    if (bit < 7) {                  // no delay at last idle clock state
      clock_delay(config->speed);
    }
  }
  return rx;
}

static inline uint8_t exchange_byte_mode1(const spi_master_config_t *config,
                                          uint8_t tx)
{
  uint8_t rx = 0;

  set_pin(config->sck);
  clock_delay(config->speed);
  for (uint8_t bit = 0; bit < 8; bit++) {
    if (tx & 0x80) {
      set_pin(config->mosi);
    } else {
      clear_pin(config->mosi);
    }
    tx <<= 1;
    clear_pin(config->sck);         // transmit edge: negative
    clock_delay(config->speed);                  // wait for rx data ready
    rx <<= 1;
    if (pin_is_set(config->miso)) {
      rx |= 0x01;
    }
    if (bit < 7) {                 // keep clock idle state at the last bit
      set_pin(config->sck);
      clock_delay(config->speed);
    }
  }
  return rx;
}

static inline uint8_t exchange_byte_mode2(const spi_master_config_t *config,
                                          uint8_t tx)
{
  uint8_t rx = 0;

  for (uint8_t bit = 0; bit < 8; bit++) {
    if (tx & 0x80) {
      set_pin(config->mosi);
    } else {
      clear_pin(config->mosi);
    }
    tx <<= 1;
    clear_pin(config->sck);            // transmit edge: negative
    clock_delay(config->speed);                     // wait for rx data ready
    rx <<= 1;
    if (pin_is_set(config->miso)) {
      rx |= 0x01;
    }
    set_pin(config->sck);             // clock idle state
    if (bit < 7) {                    // no delay at last idle clock state
      clock_delay(config->speed);
    }
  }
  return rx;
}

static inline uint8_t exchange_byte_mode3(const spi_master_config_t *config,
                                          uint8_t tx)
{
  uint8_t rx = 0;

  clear_pin(config->sck);
  clock_delay(config->speed);
  for (uint8_t bit = 0; bit < 8; bit++) {
    if (tx & 0x80) {
      set_pin(config->mosi);
    } else {
      clear_pin(config->mosi);
    }
    tx <<= 1;
    set_pin(config->sck);           // transmit edge: positive
    clock_delay(config->speed);                  // wait for rx data ready
    rx <<= 1;
    if (pin_is_set(config->miso)) {
      rx |= 0x01;
    }
    if (bit < 7) {                 // keep clock idle state at the last bit
      clear_pin(config->sck);
      clock_delay(config->speed);
    }
  }
  return rx;
}

static inline uint8_t exchange_byte(const spi_master_config_t *config,
                                    uint8_t tx)
{
  switch (config->mode) {
    case SPI_MASTER_MODE_0:
      return exchange_byte_mode0(config, tx);
    case SPI_MASTER_MODE_1:
      return exchange_byte_mode1(config, tx);
    case SPI_MASTER_MODE_2:
      return exchange_byte_mode2(config, tx);
    case SPI_MASTER_MODE_3:
      return exchange_byte_mode3(config, tx);
    default:
      return 0;
  }
}

#define buffer_is_valid(buf, len)  (((buf) != NULL && (len) != 0) \
                                    || ((buf) == NULL && (len) == 0))

err_t spi_device_transceive(spi_master_t *obj,
                            const struct spi_buf *tx_buffers,
                            const struct spi_buf *rx_buffers)
{
  size_t tx_len;
  size_t rx_len;
  uint8_t *tx_buf;
  uint8_t *rx_buf;
  if (!obj || !obj->handle) {
    return SPI_MASTER_ERROR;
  }

  if (tx_buffers) {
    if (buffer_is_valid(tx_buffers->buf, tx_buffers->len)) {
      tx_len = tx_buffers->len;
      tx_buf = tx_buffers->buf;
    } else {
      return SPI_MASTER_ERROR;
    }
  } else {
    tx_len = 0;
    tx_buf = NULL;
  }
  if (rx_buffers) {
    if (buffer_is_valid(rx_buffers->buf, rx_buffers->len)) {
      rx_len = rx_buffers->len;
      rx_buf = rx_buffers->buf;
    } else {
      return SPI_MASTER_ERROR;
    }
  } else {
    rx_len = 0;
    rx_buf = NULL;
  }

  while (tx_len && rx_len) {
    *rx_buf++ = exchange_byte(&(obj->config), *tx_buf++);
    tx_len--;
    rx_len--;
  }
  while (tx_len) {
    exchange_byte(&(obj->config), *tx_buf++);
    tx_len--;
  }
  while (rx_len) {
    *rx_buf++ = exchange_byte(&(obj->config),
                              obj->config.default_write_data);
    rx_len--;
  }
  return SPI_MASTER_SUCCESS;
}

// ------------------------------------------------------------------------- END
