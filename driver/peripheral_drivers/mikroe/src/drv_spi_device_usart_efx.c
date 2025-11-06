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
#include "em_gpio.h"
#include "drv_spi_master.h"
#include "drv_digital_out.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "spi_usart_efx_config.h"

#define SPI_GPIO_SLOW_SLEWRATE 5
#define SPI_GPIO_FAST_SLEWRATE 7

static uint32_t last_spi_speed_used;
static spi_master_mode_t last_spi_mode_used;

static spi_master_chip_select_polarity_t spi_master_chip_select_polarity =
  SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY;

static void usart_init_spi(USART_TypeDef *usart,
                           const spi_master_config_t *config);
static void spi_device_configure_gpio_pin(digital_out_t *out,
                                          pin_name_t name);
static err_t spi_device_set_config(spi_master_t *obj);

void spi_master_configure_default(spi_master_config_t *config)
{
  config->default_write_data = 0;
  config->cs = hal_gpio_pin_name(SPI_USART_EFX_CS_PORT,
                                 SPI_USART_EFX_CS_PIN);
  config->sck = hal_gpio_pin_name(SPI_USART_EFX_CLK_PORT,
                                  SPI_USART_EFX_CLK_PIN);
  config->miso = hal_gpio_pin_name(SPI_USART_EFX_RX_PORT,
                                   SPI_USART_EFX_RX_PIN);
  config->mosi = hal_gpio_pin_name(SPI_USART_EFX_TX_PORT,
                                   SPI_USART_EFX_TX_PIN);
  config->speed = SPI_USART_EFX_BITRATE;
  config->mode = SPI_MASTER_MODE_DEFAULT;
//  config->cs_mode = SPI_USART_EFX_CS_CONTROL;
  config->cs_mode = SPI_MASTER_CS_MODE_SW;
  config->spi_instance = SPI_USART_EFX_PERIPHERAL;
}

err_t spi_device_open(spi_master_t *obj, spi_master_config_t *config)
{
  obj->config = *config;
  obj->handle = config->spi_instance;
  usart_init_spi((USART_TypeDef *)obj->handle, config);
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

  if (config->cs_mode == SPI_MASTER_CS_MODE_SW) {
    if (on
        && (spi_master_chip_select_polarity
            == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW)) {
      GPIO_PinOutClear(hal_gpio_port_index(config->cs),
                       hal_gpio_pin_index(config->cs));
    } else {
      GPIO_PinOutSet(hal_gpio_port_index(config->cs),
                     hal_gpio_pin_index(config->cs));
    }
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Set SPI Master driver communication speed.
 ******************************************************************************/
err_t spi_device_set_speed(spi_master_t *obj, uint32_t speed)
{
  if (!obj || !obj->handle) {
    return SPI_MASTER_ERROR;
  }
  USART_TypeDef *usart = (USART_TypeDef *)obj->handle;
  uint32_t required_slewrate =
    (speed >= 10000000) ? SPI_GPIO_FAST_SLEWRATE : SPI_GPIO_SLOW_SLEWRATE;
  GPIO_SlewrateSet(hal_gpio_port_index(obj->config.sck),
                   required_slewrate,
                   required_slewrate);
  GPIO_SlewrateSet(hal_gpio_port_index(obj->config.mosi),
                   required_slewrate,
                   required_slewrate);

  obj->config.speed = speed;
  last_spi_speed_used = speed;
  // Set SPI bus bitrate.
  USART_BaudrateSyncSet(usart, 0, speed);
  return SPI_MASTER_SUCCESS;
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
  USART_TypeDef *usart = (USART_TypeDef *)obj->handle;

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
    *rx_buf++ = USART_SpiTransfer(usart, *tx_buf++);
    tx_len--;
    rx_len--;
  }
  while (tx_len) {
    USART_SpiTransfer(usart, *tx_buf++);
    tx_len--;
  }
  while (rx_len) {
    *rx_buf++ = USART_SpiTransfer(usart, obj->config.default_write_data);
    rx_len--;
  }
  return SPI_MASTER_SUCCESS;
}

err_t spi_device_configure(spi_master_t *obj)
{
  if (!obj || !obj->handle) {
    return SPI_MASTER_ERROR;
  }

  if (last_spi_speed_used != obj->config.speed) {
    last_spi_speed_used = obj->config.speed;
    // Update SPI bus bitrate.
    USART_BaudrateSyncSet((USART_TypeDef *)obj->handle,
                          0,
                          last_spi_speed_used);
  }

  return spi_device_set_config(obj);
}

static void usart_init_spi(USART_TypeDef *usart,
                           const spi_master_config_t *config)
{
  USART_InitSync_TypeDef usart_init = USART_INITSYNC_DEFAULT;
  uint8_t uart_index = 0xff;

  usart_init.msbf = true;
  usart_init.master = true;
  usart_init.databits = usartDatabits8;
  usart_init.baudrate = config->speed;
  switch (config->mode) {
    case SPI_MASTER_MODE_0:
      usart_init.clockMode = usartClockMode0;
      break;
    case SPI_MASTER_MODE_1:
      usart_init.clockMode = usartClockMode1;
      break;
    case SPI_MASTER_MODE_2:
      usart_init.clockMode = usartClockMode2;
      break;
    case SPI_MASTER_MODE_3:
      usart_init.clockMode = usartClockMode3;
      break;
  }

  if (config->cs_mode == SPI_MASTER_CS_MODE_HW) {
    usart_init.autoCsEnable = true;
    usart_init.autoCsHold = _USART_TIMING_CSHOLD_SEVEN;
    usart_init.autoCsSetup = _USART_TIMING_CSSETUP_SEVEN;
  } else {
    usart_init.autoCsEnable = false;
  }

#if defined(_CMU_HFPERCLKEN0_MASK)
  CMU_ClockEnable(cmuClock_HFPER, true);
#endif
  CMU_ClockEnable(cmuClock_GPIO, true);
#ifdef USART0
  if (USART0 == usart) {
    CMU_ClockEnable(cmuClock_USART0, true);
    uart_index = 0;
  }
#endif
#ifdef USART1
  else if (USART1 == usart) {
    CMU_ClockEnable(cmuClock_USART1, true);
    uart_index = 1;
  }
#endif
#ifdef USART2
  else if (USART2 == usart) {
    CMU_ClockEnable(cmuClock_USART2, true);
    uart_index = 2;
  }
#endif
  else {
    return;
  }

  USART_InitSync(usart, &usart_init);

  GPIO->USARTROUTE[uart_index].TXROUTE =
    ((uint32_t)hal_gpio_port_index(config->mosi) << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)hal_gpio_pin_index(config->mosi) << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[uart_index].RXROUTE =
    ((uint32_t)hal_gpio_port_index(config->miso) << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)hal_gpio_pin_index(config->miso) << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[uart_index].CLKROUTE =
    ((uint32_t)hal_gpio_port_index(config->sck) << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | ((uint32_t)hal_gpio_pin_index(config->sck) << _GPIO_USART_CLKROUTE_PIN_SHIFT);

  if (config->cs_mode == SPI_MASTER_CS_MODE_HW) {
    // SPI 4 wire mode, Chip Select controled by the peripheral
    GPIO->USARTROUTE[uart_index].CSROUTE =
      ((uint32_t)hal_gpio_port_index(config->cs) << _GPIO_USART_CSROUTE_PORT_SHIFT)
      | ((uint32_t)hal_gpio_pin_index(config->cs) << _GPIO_USART_CSROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[uart_index].ROUTEEN =
      GPIO_USART_ROUTEEN_TXPEN
      | GPIO_USART_ROUTEEN_RXPEN
      | GPIO_USART_ROUTEEN_CLKPEN
      | GPIO_USART_ROUTEEN_CSPEN;
  } else {
    GPIO->USARTROUTE[uart_index].ROUTEEN =
      GPIO_USART_ROUTEEN_TXPEN
      | GPIO_USART_ROUTEEN_RXPEN
      | GPIO_USART_ROUTEEN_CLKPEN;
  }

  if (config->mode == SPI_MASTER_MODE_0
      || config->mode == SPI_MASTER_MODE_1) {
    GPIO_PinModeSet(hal_gpio_port_index(config->sck),
                    hal_gpio_pin_index(config->sck),
                    gpioModePushPull,
                    0);
  } else {
    GPIO_PinModeSet(hal_gpio_port_index(config->sck),
                    hal_gpio_pin_index(config->sck),
                    gpioModePushPull,
                    1);
  }
  GPIO_PinModeSet(hal_gpio_port_index(config->mosi),
                  hal_gpio_pin_index(config->mosi),
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(hal_gpio_port_index(config->miso),
                  hal_gpio_pin_index(config->miso),
                  gpioModeInput,
                  0);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW) {
    GPIO_PinModeSet(hal_gpio_port_index(config->cs),
                    hal_gpio_pin_index(config->cs),
                    gpioModePushPull,
                    1);
  } else {
    GPIO_PinModeSet(hal_gpio_port_index(config->cs),
                    hal_gpio_pin_index(config->cs),
                    gpioModePushPull,
                    0);
  }
}

static void spi_device_configure_gpio_pin(digital_out_t *out,
                                          pin_name_t name)
{
  out->pin.base = hal_gpio_port_index(name);
  out->pin.mask = hal_gpio_pin_index(name);
}

static err_t spi_device_set_config(spi_master_t *obj)
{
  if (!obj || !obj->handle) {
    return SPI_MASTER_ERROR;
  }
  USART_TypeDef *usart = (USART_TypeDef *)obj->handle;

  if (last_spi_mode_used != obj->config.mode) {
    last_spi_mode_used = obj->config.mode;

    // DeInitialize an SPI driver instance.
    USART_Reset(usart);

    // Initialize an SPI driver instance with new mode.
    usart_init_spi((USART_TypeDef *)obj->handle, &(obj->config));
  }
  return SPI_MASTER_SUCCESS;
}

// ------------------------------------------------------------------------- END
