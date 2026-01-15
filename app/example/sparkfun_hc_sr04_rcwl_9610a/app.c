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
#include "sparkfun_hc_sr04_rcwl_9610a.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_i2c_instances.h"
#include "sl_si91x_button_instances.h"
#else
#include "app_log.h"
#include "sl_i2cspm_instances.h"
#include "sl_simple_button_instances.h"
#endif

#if defined(SLI_SI917)
#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#if defined(SLI_SI917)
#define I2C_INSTANCE_USED               SL_I2C2 // Should be changed accordingly
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;

static sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *my_rcwl_9610a_i2c_driver;

static sparkfun_hc_sr04_rcwl_9610a_config_t my_rcwl_9610a_i2c_config = {
  .connection_type = SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE,
  .unit = SPARKFUN_HC_SR04_RCWL_9610A_UNIT_CM,
  .i2c_address = SPARKFUN_HC_SR04_RCWL_9610A_I2C_ADDRESS,
  .i2c_handle = NULL,
  .uart_handle = NULL
};

static sparkfun_hc_sr04_rcwl_9610a_error_t operation_error =
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;

static bool start_measurement = false;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_printf("========RCWL-9610A Ultrasonic Sensor Example========\n");

#if defined(SLI_SI917)
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  my_rcwl_9610a_i2c_driver =
    sparkfun_hc_sr04_rcwl_9610a_driver_create_instance();

  my_rcwl_9610a_i2c_config.i2c_handle = app_i2c_instance;

  operation_error = sparkfun_hc_sr04_rcwl_9610a_driver_init(
    my_rcwl_9610a_i2c_driver,
    &my_rcwl_9610a_i2c_config);
  app_printf("My RCWL-9610A Driver Initialization: %s\r\n",
             sparkfun_hc_sr04_rcwl_9610a_driver_error_to_string(operation_error));

  operation_error = sparkfun_hc_sr04_rcwl_9610a_driver_begin(
    my_rcwl_9610a_i2c_driver);

  app_printf("My RCWL-9610A Driver Begin: %s\r\n",
             sparkfun_hc_sr04_rcwl_9610a_driver_error_to_string(operation_error));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (start_measurement) {
    operation_error = sparkfun_hc_sr04_rcwl_9610a_driver_measure_distance(
      my_rcwl_9610a_i2c_driver);
    app_printf("My RCWL-9610A Driver Measurement Operation: %s\r\n",
               sparkfun_hc_sr04_rcwl_9610a_driver_error_to_string(
                 operation_error));
    app_printf("My RCWL-9610A Driver Result in Centimeters: %.3f\r\n",
               sparkfun_hc_sr04_rcwl_9610a_driver_get_distance(
                 my_rcwl_9610a_i2c_driver));
    start_measurement = false;
  }

  // In your own application, remember to destroy the instance to avoid leakage
  // sparkfun_hc_sr04_rcwl_9610a_driver_destroy(my_rcwl_9610a_i2c_driver);
}

#if (defined(SLI_SI917))
void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
  if (pin == button_btn0.pin) {
    if (state == BUTTON_PRESSED) {
#else
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (&sl_button_btn0 == handle) {
#endif
      start_measurement = true;
    }
  }
}
