/***************************************************************************//**
 * @file epaper_display.c
 * @brief E-Paper Display Header File
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
#include <string.h>
#include "sl_si91x_peripheral_gpio.h"
#include "sl_si91x_gspi_common_config.h"
#include "sl_si91x_gspi.h"

#include "epaper_display.h"
#include "epaper_display_config.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"

#include "sl_sleeptimer.h"
#include "sl_udelay.h"

#define HIGH                         true
#define LOW                          false

#define GSPI_BUFFER_SIZE             1024      // Size of buffer
#define GSPI_INTF_PLL_CLK            180000000 // Intf pll clock frequency
#define GSPI_INTF_PLL_REF_CLK        40000000  // Intf pll reference clock frequency
#define GSPI_SOC_PLL_CLK             20000000  // Soc pll clock frequency
#define GSPI_SOC_PLL_REF_CLK         40000000  // Soc pll reference clock frequency
#define GSPI_INTF_PLL_500_CTRL_VALUE 0xD900    // Intf pll control value
#define GSPI_SOC_PLL_MM_COUNT_LIMIT  0xA4      // Soc pll count limit
#define GSPI_DVISION_FACTOR          0         // Division factor
#define GSPI_SWAP_READ_DATA          1         // true to enable and false to disable swap read
#define GSPI_SWAP_WRITE_DATA         0         // true to enable and false to disable swap write
#define GSPI_BITRATE                 10000000  // Bitrate for setting the clock division factor
#define GSPI_BIT_WIDTH               8         // Default Bit width
#define GSPI_MAX_BIT_WIDTH           16        // Maximum Bit width

#define wait_spi_transfer_ready(handle)               \
  do {                                                \
    sl_gspi_status_t gspi_status;                     \
    do {                                              \
      gspi_status = sl_si91x_gspi_get_status(handle); \
    } while (gspi_status.busy);                       \
  } while (0)

static sl_gspi_handle_t spi_epd_handle = NULL;

static digital_in_t busy_pin;
static digital_out_t dc_pin;
static digital_out_t rst_pin;
static digital_out_t cs_pin;
static digital_out_t sck_pin;
static digital_out_t mosi_pin;

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
static sl_status_t spi_init(void);
static void gpio_init(void);
static void cog_send_image(const uint8_t * next_frame,
                           const uint8_t * previous_frame,
                           uint32_t size_frame);
static void cog_update(uint8_t updateMode);
static uint8_t check_range(uint8_t value, uint8_t value_min, uint8_t value_max);

static void event_callback(uint32_t event);

void epd_init(void)
{
  gpio_init();
  cog_reset();
  cog_get_otp();
  spi_init();
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
  while ((digital_in_read(&busy_pin)) != state) {
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
  digital_out_high(&rst_pin); // RESET = HIGH
  sl_sleeptimer_delay_millisecond(2);

  digital_out_low(&rst_pin); // RESET = LOW
  sl_sleeptimer_delay_millisecond(4);

  digital_out_high(&rst_pin); // RESET = HIGH
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
  pin_name_t pin = hal_gpio_pin_name(SL_GSPI_MASTER_MOSI__PORT,
                                     SL_GSPI_MASTER_MOSI__PIN);
  digital_out_init(&mosi_pin, pin);
  digital_out_low(&mosi_pin);
  pin = hal_gpio_pin_name(SL_GSPI_MASTER_SCK__PORT, SL_GSPI_MASTER_SCK__PIN);
  digital_out_init(&sck_pin, pin);
  digital_out_low(&sck_pin);

  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  spi3_write(0xb9); // Send OTP read command
  sl_sleeptimer_delay_millisecond(5);

  digital_out_high(&dc_pin);
  spi3_read(); // Dummy
  // Populate otp_data
  for (int index = 0; index < 128; index++) {
    otp_data[index] = spi3_read(); // Read OTP
  }

  // End of OTP reading
  digital_out_high(&cs_pin); // Unselect
  // for (int i = 0; i< 128; i++) printf ("otp[%d]: 0x%x\n", i, otp_data[i]);
}

// 3-wire SPI pins read function
static uint8_t spi3_read(void)
{
  uint8_t value = 0;

  digital_out_low(&sck_pin);
  sl_si91x_gpio_set_pin_direction(mosi_pin.pin.base,
                                  mosi_pin.pin.mask,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  for (uint8_t i = 0; i < 8; i++) {
    digital_out_high(&sck_pin);
    sl_udelay_wait(1);
    value |= digital_in_read((digital_in_t *)&mosi_pin) << (7 - i);
    digital_out_low(&sck_pin);
    sl_udelay_wait(1);
  }

  return value;
}

// 3-wire SPI pins write function
static void spi3_write(uint8_t value)
{
  digital_out_low(&sck_pin);
  sl_si91x_gpio_set_pin_direction(mosi_pin.pin.base,
                                  mosi_pin.pin.mask,
                                  (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(value & (1 << (7 - i)))) {
      digital_out_low(&mosi_pin);
    } else {
      digital_out_high(&mosi_pin);
    }
    sl_udelay_wait(1);
    digital_out_high(&sck_pin);
    sl_udelay_wait(1);
    digital_out_low(&sck_pin);
    sl_udelay_wait(1);
  }
}

static void send_command_data(uint8_t command, uint8_t data)
{
  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  sl_si91x_gspi_send_data(spi_epd_handle, &command, 1);
  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&dc_pin);

  sl_si91x_gspi_send_data(spi_epd_handle, &data, 1);
  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&cs_pin);
}

static void send_index_data(uint8_t index, const uint8_t* data, uint32_t size)
{
  // Command mode
  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  sl_si91x_gspi_send_data(spi_epd_handle, &index, 1);
  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&dc_pin);

  sl_si91x_gspi_send_data(spi_epd_handle, data, size);
  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&cs_pin);
}

static void send_index_data_fixed(uint8_t index, uint8_t data, uint32_t size)
{
  // Command mode
  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  sl_si91x_gspi_send_data(spi_epd_handle, &index, 1);
  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&dc_pin);

  while (size--) {
    sl_si91x_gspi_send_data(spi_epd_handle, &data, 1);
    wait_spi_transfer_ready(spi_epd_handle);
  }

  digital_out_high(&cs_pin);
}

static void gpio_init(void)
{
  pin_name_t pin = hal_gpio_pin_name(EPD_BUSY_PORT, EPD_BUSY_PIN);
  digital_in_init(&busy_pin, pin);

  pin = hal_gpio_pin_name(EPD_DC_PORT, EPD_DC_PIN);
  digital_out_init(&dc_pin, pin);
  digital_out_high(&dc_pin);

  pin = hal_gpio_pin_name(EPD_RST_PORT, EPD_RST_PIN);
  digital_out_init(&rst_pin, pin);
  digital_out_high(&rst_pin);

  pin = hal_gpio_pin_name(SL_GSPI_MASTER_CS0__PORT, SL_GSPI_MASTER_CS0__PIN);
  digital_out_init(&cs_pin, pin);
  digital_out_high(&cs_pin);
}

static sl_status_t spi_init(void)
{
  sl_status_t status;
  sl_gspi_clock_config_t clock_config = {
    .soc_pll_mm_count_value = GSPI_SOC_PLL_MM_COUNT_LIMIT,
    .intf_pll_500_control_value = GSPI_INTF_PLL_500_CTRL_VALUE,
    .intf_pll_clock = GSPI_INTF_PLL_CLK,
    .intf_pll_reference_clock = GSPI_INTF_PLL_REF_CLK,
    .soc_pll_clock = GSPI_SOC_PLL_CLK,
    .soc_pll_reference_clock = GSPI_SOC_PLL_REF_CLK,
    .division_factor = GSPI_DVISION_FACTOR
  };
  sl_gspi_control_config_t control_config = {
    .bit_width = GSPI_BIT_WIDTH,
    .bitrate = GSPI_BITRATE,
    .clock_mode = SL_GSPI_MODE_0,
    .slave_select_mode = SL_GSPI_MASTER_HW_OUTPUT,
    .swap_read = GSPI_SWAP_READ_DATA,
    .swap_write = GSPI_SWAP_WRITE_DATA,
  };

//  gspi_configuration = control_config;
  // Configuration of clock with the default clock parameters
  status = sl_si91x_gspi_configure_clock(&clock_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
  // Pass the address of void pointer, it will be updated with the address
  // of GSPI instance which can be used in other APIs.
  status = sl_si91x_gspi_init(SL_GSPI_MASTER, &spi_epd_handle);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Fetching the status of GSPI i.e., busy, data lost and mode fault
  // Configuration of all other parameters that are required by GSPI
  // gspi_configuration structure is from sl_si91x_gspi_init.h file.
  // The user can modify this structure with the configuration of
  // his choice by filling this structure.
  status = sl_si91x_gspi_set_configuration(spi_epd_handle,
                                           &control_config);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Register user callback function
  status = sl_si91x_gspi_register_event_callback(spi_epd_handle,
                                                 event_callback);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Validation for executing the API only once
  status = sl_si91x_gspi_set_slave_number(GSPI_SLAVE_0);
  if (status != SL_STATUS_OK) {
    return status;
  }
  return SL_STATUS_OK;
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

static void event_callback(uint32_t event)
{
  switch (event) {
    case SL_GSPI_TRANSFER_COMPLETE:
      break;
    case SL_GSPI_DATA_LOST:
      break;
    case SL_GSPI_MODE_FAULT:
      break;
  }
}
