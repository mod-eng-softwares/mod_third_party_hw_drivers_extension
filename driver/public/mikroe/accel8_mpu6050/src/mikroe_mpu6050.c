/***************************************************************************//**
 * @file mikroe_mpu6050.c
 * @brief Mikroe MPU6050 Source File
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

#include "mikroe_mpu6050_config.h"
#include "mikroe_mpu6050.h"

static accel8_t accel8;
static accel8_cfg_t accel8_cfg;

/***************************************************************************//**
 *    Initialization function.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  accel8.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  accel8_cfg_setup(&accel8_cfg);

#if defined(ACCEL8_DA_PORT) && defined(ACCEL8_DA_PIN)
  accel8_cfg.da = hal_gpio_pin_name(ACCEL8_DA_PORT, ACCEL8_DA_PIN);
#endif

#if defined(ACCEL8_CL_PORT) && defined(ACCEL8_CL_PIN)
  accel8_cfg.cl = hal_gpio_pin_name(ACCEL8_CL_PORT, ACCEL8_CL_PIN);
#endif

#if defined(ACCEL8_INT_PORT) && defined(ACCEL8_INT_PIN)
  accel8_cfg.int_pin = hal_gpio_pin_name(ACCEL8_INT_PORT, ACCEL8_INT_PIN);
#endif

  if (accel8_init(&accel8, &accel8_cfg) != ACCEL8_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    This function sets the I2CSPM instance used by platform functions.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_set_i2cspm_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  accel8.i2c.handle = i2cspm_instance;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Sets default configuration.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_default_cfg(uint8_t accel_cfg,
                                       uint8_t gyro_cfg,
                                       mikroe_mpu6050_range_t *rng)
{
  range_retval_t range;

  accel8_default_cfg(&accel8, accel_cfg, gyro_cfg, &range);
  rng->accel_range = range.accel_range;
  rng->gyro_range = range.gyro_range;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Write register content.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_generic_write(uint8_t reg,
                                         uint8_t *data_buf,
                                         uint8_t len)
{
  accel8_generic_write(&accel8, reg, data_buf, len);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Read register content.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_generic_read(uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len)
{
  accel8_generic_read(&accel8, reg, data_buf, len);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    This function reads 1 word from the desired register.
 ******************************************************************************/
uint16_t mikroe_mpu6050_read_data(uint8_t reg)
{
  return accel8_read_data(&accel8, reg);
}

/***************************************************************************//**
 *    Functions for read Temperature data in C.
 ******************************************************************************/
float mikroe_mpu6050_get_temperature(void)
{
  return accel8_get_temperature(&accel8);
}

/***************************************************************************//**
 *    Functions for read Accel axis data.
 ******************************************************************************/
void mikroe_mpu6050_get_accel_axis(int16_t *x_axis,
                                   int16_t *y_axis,
                                   int16_t *z_axis)
{
  accel8_get_accel_axis(&accel8, x_axis, y_axis, z_axis);
}

/***************************************************************************//**
 *    Functions for read Gyro axis data.
 ******************************************************************************/
void mikroe_mpu6050_get_gyro_axis(int16_t *x_axis,
                                  int16_t *y_axis,
                                  int16_t *z_axis)
{
  accel8_get_gyro_axis(&accel8, x_axis, y_axis, z_axis);
}

/***************************************************************************//**
 *    Functions for read INT pin state.
 ******************************************************************************/
uint8_t mikroe_mpu6050_get_interrupt(void)
{
  return accel8_get_interrupt(&accel8);
}
