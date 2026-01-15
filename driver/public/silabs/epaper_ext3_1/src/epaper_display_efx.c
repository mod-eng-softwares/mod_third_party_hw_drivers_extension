/***************************************************************************//**
 * @file epaper_display_efx.c
 * @brief E-Paper Display Header File for EFx series
 * @version 1.0.0
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#include <string.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "sl_sleeptimer.h"
#include "sl_udelay.h"
#include "epaper_display.h"
#include "epaper_display_config.h"

#define HIGH                         true
#define LOW                          false

#define USART_CONCAT2(x, y)          x ## y
#define USART_CONCAT_DEV(periph_nbr) USART_CONCAT2(USART, periph_nbr)
#define USART_CONCAT_CLK(periph_nbr) USART_CONCAT2(cmuClock_USART, periph_nbr)

#define USART_DEV            USART_CONCAT_DEV(SPI_EPD_PERIPHERAL_NO)
#define USART_CLK            USART_CONCAT_CLK(SPI_EPD_PERIPHERAL_NO)

// Internal variables
static uint8_t otp_data[128];
static uint8_t epd_update_mode = UPDATE_NORMAL;

static void wait_busy(bool state);
static void cog_initial(void);
static void cog_reset(void);
static void cog_stop_dcdc(void);
static void cog_get_otp(void);
static void spi3_write(uint8_t value);
static uint8_t spi3_read(void);
static void send_command_data(uint8_t command, uint8_t data);
static void send_index_data(uint8_t index, const uint8_t * data, uint32_t size);
static void send_index_data_fixed(uint8_t index, uint8_t data, uint32_t size);
static void gpio_init(void);
static void usart_init(void);
static void cog_send_image(const uint8_t * next_frame,
                           const uint8_t * previous_frame,
                           uint32_t size_frame);
static void cog_update(uint8_t updateMode);
static uint8_t check_range(uint8_t value, uint8_t value_min, uint8_t value_max);

void epd_init(void)
{
  gpio_init();
  cog_reset();
  cog_get_otp();
  usart_init();
}

void epd_set_update_mode(uint8_t mode)
{
  epd_update_mode = mode;
}

void epd_update_display(uint8_t *next_frame,
                        uint8_t *previous_frame,
                        uint32_t frame_size)
{
  cog_reset(); // Reset
  cog_initial();

  // Send image data
  if (epd_update_mode == UPDATE_FAST) {
    cog_send_image(next_frame, previous_frame, frame_size);
  } else {
    cog_send_image(next_frame, NULL, frame_size);
  }
  cog_update(epd_update_mode);
  cog_stop_dcdc(); // Power off COG internal DC/DC
}

static void wait_busy(bool state)
{
  while (GPIO_PinInGet(EPD_BUSY_PORT, EPD_BUSY_PIN) != state) {
     // sl_sleeptimer_delay_millisecond(32);
  }
}

static void cog_initial(void)
{
  uint8_t workDCTL[2];
  workDCTL[0] = otp_data[0x10]; // DCTL
  workDCTL[1] = 0x00;
  send_index_data(0x01, workDCTL, 2);
}

static void cog_reset(void)
{
    // Application note § 2. Power on COG driver
  GPIO_PinOutSet(EPD_RST_PORT, EPD_RST_PIN); // RESET = HIGH
  sl_sleeptimer_delay_millisecond(2);

  GPIO_PinOutClear(EPD_RST_PORT, EPD_RST_PIN); // RESET = LOW
  sl_sleeptimer_delay_millisecond(4);

  GPIO_PinOutSet(EPD_RST_PORT, EPD_RST_PIN); // RESET = HIGH
  sl_sleeptimer_delay_millisecond(20);
}

static void cog_stop_dcdc(void)
{
  // Application note § 7. Turn-off DC/DC
  // Application note § 5. Turn-off DC/DC

  // DC-DC off
  wait_busy(HIGH);

  send_command_data(0x09, 0x7f);
  send_command_data(0x05, 0x3d);
  send_command_data(0x09, 0x7e);
  sl_sleeptimer_delay_millisecond(60);
  send_command_data(0x09, 0x00);
}

static void cog_get_otp(void)
{
  // Start of OTP reading
  GPIO_PinModeSet(SPI_EPD_CLK_PORT,
                  SPI_EPD_CLK_PIN,
                  gpioModePushPull, 0);
  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  spi3_write(0xb9); // Send OTP read command
  sl_sleeptimer_delay_millisecond(5);

  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN);
  spi3_read(); // Dummy
  // Populate otp_data
  for (int index = 0; index < 128; index++) {
    otp_data[index] = spi3_read(); // Read OTP
  }

  // End of OTP reading
  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN); // Unselect
  // for (int i = 0; i< 128; i++) printf ("otp[%d]: 0x%x\n", i, otp_data[i]);
}

// 3-wire SPI pins read function
static uint8_t spi3_read(void)
{
  uint8_t value = 0;

  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModeInput, 0);

  for (uint8_t i = 0; i < 8; i++) {
    GPIO_PinOutSet(SPI_EPD_CLK_PORT, SPI_EPD_CLK_PIN);
    sl_udelay_wait(1);
    value |= GPIO_PinInGet(SPI_EPD_TX_PORT,
                           SPI_EPD_TX_PIN) << (7 - i);
    GPIO_PinOutClear(SPI_EPD_CLK_PORT, SPI_EPD_CLK_PIN);
    sl_udelay_wait(1);
  }
  return value;
}

// 3-wire SPI pins write function
static void spi3_write(uint8_t value)
{
  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModePushPull, 0);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(value & (1 << (7 - i)))) {
      GPIO_PinOutClear(SPI_EPD_TX_PORT, SPI_EPD_TX_PIN);
    } else {
      GPIO_PinOutSet(SPI_EPD_TX_PORT, SPI_EPD_TX_PIN);
    }
    sl_udelay_wait(1);
    GPIO_PinOutSet(SPI_EPD_CLK_PORT, SPI_EPD_CLK_PIN);
    sl_udelay_wait(1);
    GPIO_PinOutClear(SPI_EPD_CLK_PORT, SPI_EPD_CLK_PIN);
    sl_udelay_wait(1);
  }
}

static void send_command_data(uint8_t command, uint8_t data)
{
  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN); // DC Low = Command
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN); // Select
  USART_SpiTransfer(USART_DEV, command);
  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN); // DC High = Data
  USART_SpiTransfer(USART_DEV, data);
  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN); // CS = HIGH, unselect
}

static void send_index_data(uint8_t index, const uint8_t* data, uint32_t size)
{
  // Command mode
  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  USART_SpiTransfer(USART_DEV, index);

  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN);

  while (size--) {
    USART_SpiTransfer(USART_DEV, *data++);
  }

  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);
}

static void send_index_data_fixed(uint8_t index, uint8_t data, uint32_t size)
{
  // Command mode
  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  USART_SpiTransfer(USART_DEV, index);

  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN);

  while (size--) {
    USART_SpiTransfer(USART_DEV, data);
  }

  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);
}

static void gpio_init(void)
{
  // Enable clock to GPIO
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Config pin
  GPIO_PinModeSet(EPD_BUSY_PORT, EPD_BUSY_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(EPD_DC_PORT, EPD_DC_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(EPD_RST_PORT, EPD_RST_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN, gpioModePushPull, 1);
}

static void usart_init(void)
{
  CMU_ClockEnable(USART_CLK, true);

  GPIO_PinModeSet(SPI_EPD_TX_PORT, SPI_EPD_TX_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_EPD_RX_PORT, SPI_EPD_RX_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(SPI_EPD_CLK_PORT, SPI_EPD_CLK_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN, gpioModePushPull, 1);

  // Default asynchronous initializer (main mode, 1 Mbps, 8-bit data)
  USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

  init.msbf = true;   // MSB first transmission for SPI compatibility
  init.baudrate = SPI_EPD_BITRATE;

  /*
   * Route USARTx RX, TX, and CLK to the specified pins.  Note that CS is
   * not controlled by USARTx so there is no write to the corresponding
   * USARTROUTE register to do this.
   */
  GPIO->USARTROUTE[SPI_EPD_PERIPHERAL_NO].TXROUTE = (SPI_EPD_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
      | (SPI_EPD_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[SPI_EPD_PERIPHERAL_NO].RXROUTE = (SPI_EPD_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
      | (SPI_EPD_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[SPI_EPD_PERIPHERAL_NO].CLKROUTE = (SPI_EPD_CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
      | (SPI_EPD_CLK_PIN << _GPIO_USART_CLKROUTE_PIN_SHIFT);

  // Enable USART interface pins
  GPIO->USARTROUTE[SPI_EPD_PERIPHERAL_NO].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN |    // MISO
                                                    GPIO_USART_ROUTEEN_TXPEN |    // MOSI
                                                    GPIO_USART_ROUTEEN_CLKPEN;

  // Configure and enable USART
  USART_InitSync(USART_DEV, &init);
}

static void cog_send_image(const uint8_t * next_frame,
                           const uint8_t * previous_frame,
                           uint32_t size_frame)
{
  // Application note § 3.2 Input image to the EPD

  // Send image data
  send_index_data(0x13, &otp_data[0x15], 6); // DUW
  send_index_data(0x90, &otp_data[0x0c], 4); // DRFW

  // Next frame
  send_index_data(0x12, &otp_data[0x12], 3); // RAM_RW
  send_index_data(0x10, next_frame, size_frame); // Next frame

  // Previous frame
  send_index_data(0x12, &otp_data[0x12], 3); // RAM_RW
  if (epd_update_mode == UPDATE_NORMAL) {
    send_index_data_fixed(0x11, 0x00, size_frame); // Previous frame = dummy
  } else {
    send_index_data(0x11, previous_frame, size_frame); // Next frame
  }
}

static void cog_update(uint8_t updateMode)
{
  // Initial COG
  // Application note § 3.1 Initial flow chart
  send_command_data(0x05, 0x7d);
  sl_sleeptimer_delay_millisecond(50);
  send_command_data(0x05, 0x00);
  sl_sleeptimer_delay_millisecond(1);
  send_command_data(0xd8, otp_data[0x1c]); // MS_SYNC
  send_command_data(0xd6, otp_data[0x1d]); // BVSS

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  // Temperature sequence
  send_command_data(0x44, 0x00);
  send_command_data(0x45, 0x80);

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  int8_t u_temperature = 25;
  uint8_t indexTemperature = 25;

  switch (updateMode) {
    case UPDATE_FAST:
      indexTemperature = (u_temperature + 0x28) + 0x80;
      // indexTemperature = (u_temperature > 50) ? 0xda : indexTemperature;
      // indexTemperature = (u_temperature < 0) ? 0xa8 : indexTemperature;
      indexTemperature = check_range(indexTemperature, (uint8_t)0xa8, (uint8_t)0xda);
      break;

    case UPDATE_NORMAL:
      indexTemperature = u_temperature + 0x28; // Temperature 0x41@25C
      // indexTemperature = (u_temperature > 60) ? 0x64 : indexTemperature;
      // indexTemperature = (u_temperature < -15) ? 0x19 : indexTemperature;
      indexTemperature = check_range(indexTemperature, (uint8_t)0x19, (uint8_t)0x64);
      break;

    default:
      break;
  }

  send_command_data(0x44, 0x06);
  send_command_data(0x45, indexTemperature);

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  send_command_data(0x60, otp_data[0x0b]); // TCON
  send_command_data(0x61, otp_data[0x1b]); // STV_DIR
  // No DCTL here
  send_command_data(0x02, otp_data[0x11]); // VCOM

  send_command_data(0x03, otp_data[0x1f]); // VCOM_CTRL

  // DC/DC Soft-start
  // Application note § 3.3 DC/DC soft-start
  // DRIVER_B = 0x28, DRIVER_8 = 0x20
  uint8_t offsetFrame = 0x28;

  // Filter for register 0x09
  uint8_t _filter09 = 0xff;

  for (uint8_t stage = 0; stage < 4; stage++) {
    uint8_t offset = offsetFrame + 0x08 * stage;
    // uint8_t FORMAT = otp_data[offset] & 0x80;
    uint8_t REPEAT = otp_data[offset] & 0x7f;

    uint8_t PHL_PHH[2];
    PHL_PHH[0] = otp_data[offset + 1]; // PHL_INI
    PHL_PHH[1] = otp_data[offset + 2]; // PHH_INI
    uint8_t PHL_VAR = otp_data[offset + 3];
    uint8_t PHH_VAR = otp_data[offset + 4];
    uint8_t BST_SW_a = otp_data[offset + 5] & _filter09;
    uint8_t BST_SW_b = otp_data[offset + 6] & _filter09;
    // uint8_t DELAY_SCALE = otp_data[offset + 7] & 0x80;
    uint16_t DELAY_VALUE = otp_data[offset + 7] & 0x7f;

    for (uint8_t i = 0; i < REPEAT; i++) {
      send_command_data(0x09, BST_SW_a);
      PHL_PHH[0] += PHL_VAR; // PHL
      PHL_PHH[1] += PHH_VAR; // PHH
      send_index_data(0x51, PHL_PHH, 2);
      send_command_data(0x09, BST_SW_b);

      sl_sleeptimer_delay_millisecond(DELAY_VALUE); // ms
    }
  }

  // Display Refresh Start
  // Application note § 4 Send updating command
  wait_busy(HIGH);
  send_command_data(0x15, 0x3c);
}

static uint8_t check_range(uint8_t value, uint8_t value_min, uint8_t value_max)
{
  uint8_t local_min = (value_min < value_max) ? value_min : value_max;
  uint8_t local_max = (value_min > value_max) ? value_min : value_max;

  uint8_t result = value;

  result = (local_min > result) ? local_min : result;
  result = (local_max < result) ? local_max : result;

  return result;
}
