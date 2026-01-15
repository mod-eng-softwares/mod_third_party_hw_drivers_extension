/***************************************************************************//**
 * @file sparkfun_rcwl_9610a.c
 * @brief Source file for RCWL-9610A
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sl_sleeptimer.h"
#include "sparkfun_hc_sr04_rcwl_9610a.h"

/*******************************************************************************
 * Private Function Declarations
 ******************************************************************************/
static bool validate_instance(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);
static sparkfun_hc_sr04_rcwl_9610a_error_t validate_distance_range(float distance, sparkfun_hc_sr04_rcwl_9610a_unit_t unit);
static float convert_micrometers_to_unit(uint32_t micrometers, sparkfun_hc_sr04_rcwl_9610a_unit_t unit);
static sparkfun_hc_sr04_rcwl_9610a_error_t perform_i2c_measurement(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self, uint8_t *response);
static sparkfun_hc_sr04_rcwl_9610a_error_t perform_uart_measurement(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self, uint8_t *response);
/*******************************************************************************
 * Public Function Implementations
 *****************************************************************************/

sparkfun_hc_sr04_rcwl_9610a_driver_instance_t* sparkfun_hc_sr04_rcwl_9610a_driver_create_instance(void)
{
  sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *instance =
      (sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *)malloc(sizeof(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t));

  if (instance == NULL) {
      return NULL;
  }

  // Zero-initialize entire structure
  memset(instance, 0, sizeof(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t));

  // Set default values
  instance->last_measurement = 0.0f;
  instance->is_initialized = false;
  instance->connection_type = SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE;
  instance->measurement_unit = SPARKFUN_HC_SR04_RCWL_9610A_UNIT_CM;

  return instance;
}

sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_init(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                            const sparkfun_hc_sr04_rcwl_9610a_config_t *config)
{
  if (!validate_instance(self)) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE;
  }

  if (config == NULL) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  // Store configuration
  self->connection_type = config->connection_type;
  self->measurement_unit = config->unit;

  // Configure communication interface
  if (config->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE) {
      i2c_master_configure_default(&self->attributes.i2c_context.config);

      self->attributes.i2c_context.config.addr = 
          (config->i2c_address != 0) ? config->i2c_address : SPARKFUN_HC_SR04_RCWL_9610A_I2C_ADDRESS;
      if (config->i2c_handle == NULL) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MISMATCH_INSTANCE;
      }
      self->attributes.i2c_context.handle = config->i2c_handle;

  } else if (config->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE) {
      uart_configure_default(&self->attributes.uart_context.config);

      self->attributes.uart_context.rx_ring_buffer = self->attributes.uart_rx_buffer;
      self->attributes.uart_context.tx_ring_buffer = self->attributes.uart_tx_buffer;
      self->attributes.uart_context.config.rx_ring_size = sizeof(self->attributes.uart_rx_buffer);
      self->attributes.uart_context.config.tx_ring_size = sizeof(self->attributes.uart_tx_buffer);

      if (config->uart_handle == NULL) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MISMATCH_INSTANCE;
      }
      self->attributes.uart_context.handle = config->uart_handle;

  } else {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  self->is_initialized = true;
  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

void sparkfun_hc_sr04_rcwl_9610a_driver_set_i2c_handle(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                       mikroe_i2c_handle_t handle)
{
  if (validate_instance(self) && self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE) {
      self->attributes.i2c_context.handle = handle;
  }
}

void sparkfun_hc_sr04_rcwl_9610a_driver_set_uart_handle(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                        mikroe_uart_handle_t handle)
{
  if (validate_instance(self) && self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE) {
      self->attributes.uart_context.handle = handle;
  }
}

sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_begin(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  if (!validate_instance(self)) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE;
  }

  if (!self->is_initialized) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INIT;
  }

  if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE) {
      // Open I2C communication
      if (i2c_master_open(&self->attributes.i2c_context, 
                         &self->attributes.i2c_context.config) == I2C_MASTER_ERROR) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_BEGIN;
      }

  } else if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE) {
      // Open UART communication
      if (uart_open(&self->attributes.uart_context, 
                   &self->attributes.uart_context.config) == UART_ERROR) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_BEGIN;
      }

      // Configure UART parameters (9600 baud, 8N1, non-blocking)
      uart_set_baud(&self->attributes.uart_context, 9600);
      uart_set_parity(&self->attributes.uart_context, UART_PARITY_DEFAULT);
      uart_set_stop_bits(&self->attributes.uart_context, UART_STOP_BITS_DEFAULT);
      uart_set_data_bits(&self->attributes.uart_context, UART_DATA_BITS_DEFAULT);
      uart_set_blocking(&self->attributes.uart_context, false);

  } else {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_measure_distance(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  if (!validate_instance(self)) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE;
  }

  if (!self->is_initialized) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INIT;
  }

  uint8_t response[SPARKFUN_HC_SR04_RCWL_9610A_RESPONSE_SIZE] = {0};
  sparkfun_hc_sr04_rcwl_9610a_error_t error;
  // Perform measurement based on connection type
  if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE) {
      error = perform_i2c_measurement(self, response);
  } else if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE) {
      error = perform_uart_measurement(self, response);
  } else {
      error = SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  if (error != SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE) {
      return error;
  }

  // Convert 24-bit response to micrometers (MSB first)
  uint32_t distance_micrometers = ((uint32_t)response[0] << 16) | 
                                  ((uint32_t)response[1] << 8) | 
                                  response[2];

  // Convert to configured unit
  float distance = convert_micrometers_to_unit(distance_micrometers, self->measurement_unit);

  // Validate range
  error = validate_distance_range(distance, self->measurement_unit);
  if (error != SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE) {
      return error;
  }

  // Update last measurement and timestamp
  self->last_measurement = distance;

  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

float sparkfun_hc_sr04_rcwl_9610a_driver_get_distance(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  if (!validate_instance(self)) {
      return -1.0f;
  }
  return self->last_measurement;
}

sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_set_unit(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                                sparkfun_hc_sr04_rcwl_9610a_unit_t unit)
{
  if (!validate_instance(self)) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE;
  }

  if (unit > SPARKFUN_HC_SR04_RCWL_9610A_UNIT_INCH) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  self->measurement_unit = unit;
  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

const char* sparkfun_hc_sr04_rcwl_9610a_driver_error_to_string(sparkfun_hc_sr04_rcwl_9610a_error_t error)
{
  switch (error) {
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE:
      return "No error";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE:
      return "NULL instance pointer";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_CREATE_INSTANCE:
      return "Failed to create instance";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_BEGIN:
      return "Failed to begin communication";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MISMATCH_INSTANCE:
      return "Mismatched communication instance";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INIT:
      return "Driver not initialized";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_WRITE:
      return "I2C write failed";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_READ:
      return "I2C read failed";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_WRITE:
      return "UART write failed";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_READ:
      return "UART read failed";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MEASUREMENT:
      return "Measurement error";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_OUT_OF_RANGE:
      return "Distance out of valid range";
    case SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER:
      return "Invalid parameter";
    default:
      return "Unknown error";
  }
}

bool sparkfun_hc_sr04_rcwl_9610a_driver_is_initialized(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  return (self != NULL) && self->is_initialized;
}

void sparkfun_hc_sr04_rcwl_9610a_driver_destroy(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  if (self == NULL) {
      return;
  }

  // Close communication interfaces
  if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE) {
      i2c_master_close(&self->attributes.i2c_context);
  } else if (self->connection_type == SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE) {
      uart_close(&self->attributes.uart_context);
  }

  // Free memory
  free(self);
}

/*******************************************************************************
 * Private Function Implementations
 ******************************************************************************/

static bool validate_instance(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self)
{
  return (self != NULL);
}

static sparkfun_hc_sr04_rcwl_9610a_error_t validate_distance_range(float distance, sparkfun_hc_sr04_rcwl_9610a_unit_t unit)
{
  switch (unit) {
    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_MM:
      if (distance < SPARKFUN_HC_SR04_RCWL_9610A_MIN_DISTANCE_MM || distance > SPARKFUN_HC_SR04_RCWL_9610A_MAX_DISTANCE_MM) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_OUT_OF_RANGE;
      }
      break;

    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_CM:
      if (distance < SPARKFUN_HC_SR04_RCWL_9610A_MIN_DISTANCE_CM || distance > SPARKFUN_HC_SR04_RCWL_9610A_MAX_DISTANCE_CM) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_OUT_OF_RANGE;
      }
      break;

    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_INCH:
      if (distance < (SPARKFUN_HC_SR04_RCWL_9610A_MIN_DISTANCE_CM / SPARKFUN_HC_SR04_RCWL_9610A_CM_TO_INCH) ||
          distance > (SPARKFUN_HC_SR04_RCWL_9610A_MAX_DISTANCE_CM / SPARKFUN_HC_SR04_RCWL_9610A_CM_TO_INCH)) {
          return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_OUT_OF_RANGE;
      }
      break;

    default:
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER;
  }

  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

static float convert_micrometers_to_unit(uint32_t micrometers, sparkfun_hc_sr04_rcwl_9610a_unit_t unit)
{
  switch (unit) {
    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_MM:
      return (float)micrometers / SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_MM;
    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_CM:
      return (float)micrometers / SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_CM;
    case SPARKFUN_HC_SR04_RCWL_9610A_UNIT_INCH:
      return (float)micrometers / SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_INCH;
    default:
      return 0.0f;
  }
}

static sparkfun_hc_sr04_rcwl_9610a_error_t perform_i2c_measurement(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                   uint8_t *response)
{
  uint8_t cmd = SPARKFUN_HC_SR04_RCWL_9610A_CMD_MEASURE_I2C;

  // Send measurement command
  if (i2c_master_write(&self->attributes.i2c_context, &cmd, sizeof(cmd)) == I2C_MASTER_ERROR) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_WRITE;
  }

  // Wait for measurement to complete
  sl_sleeptimer_delay_millisecond(SPARKFUN_HC_SR04_RCWL_9610A_MEASUREMENT_DELAY_MS);
  // Read response
  if (i2c_master_read(&self->attributes.i2c_context, response, 
                      SPARKFUN_HC_SR04_RCWL_9610A_RESPONSE_SIZE) == I2C_MASTER_ERROR) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_READ;
  }

  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}

static sparkfun_hc_sr04_rcwl_9610a_error_t perform_uart_measurement(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                    uint8_t *response)
{
  uint8_t cmd = SPARKFUN_HC_SR04_RCWL_9610A_CMD_MEASURE_UART;

  // Clear UART buffers
  uart_clear(&self->attributes.uart_context);

  // Send measurement command
  if (uart_write(&self->attributes.uart_context, &cmd, sizeof(cmd)) == UART_ERROR) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_WRITE;
  }

  // Wait for measurement to complete
  sl_sleeptimer_delay_millisecond(SPARKFUN_HC_SR04_RCWL_9610A_MEASUREMENT_DELAY_MS);
  // Read response
  if (uart_read(&self->attributes.uart_context, response, 
                SPARKFUN_HC_SR04_RCWL_9610A_RESPONSE_SIZE) == UART_ERROR) {
      return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_READ;
  }

  return SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE;
}
