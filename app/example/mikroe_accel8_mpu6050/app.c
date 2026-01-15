/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "sl_sleeptimer.h"
#include "mikroe_mpu6050.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;
static mikroe_mpu6050_range_t rng;

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint8_t data;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Mikroe Accel 8 Click Driver - Example\n");

  sc = mikroe_mpu6050_init(app_i2c_instance);
  if (SL_STATUS_OK != sc) {
    app_printf("Accel 8 Click initialized failed!\n");
    return;
  } else {
    app_printf("Accel 8 Click initialized successfully!\n");
  }

  app_printf("Device Reset\n");
  data = MIKROE_MPU6050_PM1_DEVICE_RESET;
  mikroe_mpu6050_generic_write(MIKROE_MPU6050_REG_PWR_MGMT_1, &data, 1);
  sl_sleeptimer_delay_millisecond(500);

  data = MIKROE_MPU6050_GYRO_RESET
         | MIKROE_MPU6050_ACCEL_RESET
         | MIKROE_MPU6050_TEMP_RESET;
  mikroe_mpu6050_generic_write(MIKROE_MPU6050_REG_SIGNAL_PATH_RESET, &data, 1);
  sl_sleeptimer_delay_millisecond(500);

  app_printf("Device Configuration \n");
  mikroe_mpu6050_default_cfg(MIKROE_MPU6050_ACCEL_CFG_FULL_SCALE_RANGE_2g,
                             MIKROE_MPU6050_GYRO_CFG_FULL_SCALE_RANGE_250dbs,
                             &rng);
  sl_sleeptimer_delay_millisecond(500);

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        2000,
                                        app_timer_callback,
                                        0, 1, 1);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float temperature;
  int16_t x_gyro_axis;
  int16_t y_gyro_axis;
  int16_t z_gyro_axis;
  int16_t x_accel_axis;
  int16_t y_accel_axis;
  int16_t z_accel_axis;

  if (trigger_process) {
    trigger_process = false;
    mikroe_mpu6050_get_accel_axis(&x_accel_axis, &y_accel_axis, &z_accel_axis);
    mikroe_mpu6050_get_gyro_axis(&x_gyro_axis, &y_gyro_axis, &z_gyro_axis);
    temperature = mikroe_mpu6050_get_temperature();
    app_printf("|  Data   | X axis | Y axis | Z axis |  Range  |\r\n");
    app_printf("|  Accel  | %d     | %d     | %d     | %dg     |\r\n",
               x_accel_axis, y_accel_axis, z_accel_axis, rng.accel_range);
    app_printf("|  Gyro   | %d     | %d     | %d     | %ddps   |\r\n",
               x_gyro_axis, y_gyro_axis, z_gyro_axis, rng.gyro_range);
    app_printf("|  Temp   | %.2f  C         |\r\n", temperature);
    app_printf(" \r\n");
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;
  trigger_process = true;
}
