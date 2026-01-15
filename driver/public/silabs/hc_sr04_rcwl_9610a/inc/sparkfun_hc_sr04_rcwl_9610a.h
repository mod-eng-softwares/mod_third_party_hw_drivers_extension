/***************************************************************************//**
 * @file sparkfun_rcwl_9610a.h
 * @brief Header file for RCWL-9610A
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
#ifndef SPARKFUN_HC_SR04_RCWL_9610A_DRIVER_H_
#define SPARKFUN_HC_SR04_RCWL_9610A_DRIVER_H_

#include "drv_i2c_master.h"
#include "drv_uart.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Configuration Macros
 ******************************************************************************/
#define SPARKFUN_HC_SR04_RCWL_9610A_I2C_ADDRESS               0x57  ///< Default I2C address

// Measurement range limits
#define SPARKFUN_HC_SR04_RCWL_9610A_MAX_DISTANCE_CM           400   ///< Maximum measurable distance in cm
#define SPARKFUN_HC_SR04_RCWL_9610A_MIN_DISTANCE_CM           2     ///< Minimum measurable distance in cm
#define SPARKFUN_HC_SR04_RCWL_9610A_MAX_DISTANCE_MM           4000  ///< Maximum measurable distance in mm
#define SPARKFUN_HC_SR04_RCWL_9610A_MIN_DISTANCE_MM           20    ///< Minimum measurable distance in mm

// Command definitions
#define SPARKFUN_HC_SR04_RCWL_9610A_CMD_MEASURE_I2C           0x01  ///< I2C measurement trigger command
#define SPARKFUN_HC_SR04_RCWL_9610A_CMD_MEASURE_UART          0xA0  ///< UART measurement trigger command

// Timing constants
#define SPARKFUN_HC_SR04_RCWL_9610A_MEASUREMENT_DELAY_MS      150   ///< Time to wait for measurement completion
#define SPARKFUN_HC_SR04_RCWL_9610A_RESPONSE_SIZE             3     ///< Size of distance response in bytes
// Unit conversion factors
#define SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_MM                  1000.0f
#define SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_CM                  10000.0f
#define SPARKFUN_HC_SR04_RCWL_9610A_UM_TO_INCH                25400.0f
#define SPARKFUN_HC_SR04_RCWL_9610A_CM_TO_INCH                2.54f

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/

/**
 * @brief Connection type for sensor communication
 */
typedef enum {
  SPARKFUN_HC_SR04_RCWL_9610A_I2C_MODE = 0,   ///< I2C communication mode
  SPARKFUN_HC_SR04_RCWL_9610A_UART_MODE       ///< UART communication mode
} sparkfun_hc_sr04_rcwl_9610a_connection_type_t;

/**
 * @brief Measurement unit selection
 */
typedef enum {
  SPARKFUN_HC_SR04_RCWL_9610A_UNIT_MM = 0,    ///< Millimeters
  SPARKFUN_HC_SR04_RCWL_9610A_UNIT_CM,        ///< Centimeters
  SPARKFUN_HC_SR04_RCWL_9610A_UNIT_INCH       ///< Inches
} sparkfun_hc_sr04_rcwl_9610a_unit_t;

/**
 * @brief Error codes returned by driver functions
 */
typedef enum {
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NONE = 0,           ///< No error
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_NULL_INSTANCE,      ///< NULL instance pointer
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_CREATE_INSTANCE,    ///< Failed to allocate memory
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INIT,               ///< Initialization error
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_BEGIN,              ///< Failed to initialize communication
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MISMATCH_INSTANCE,
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_WRITE,          ///< I2C write failed
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_I2C_READ,           ///< I2C read failed
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_WRITE,         ///< UART write failed
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_UART_READ,          ///< UART read failed
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_MEASUREMENT,        ///< General measurement error
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_OUT_OF_RANGE,       ///< Distance out of valid range
  SPARKFUN_HC_SR04_RCWL_9610A_ERROR_INVALID_PARAMETER   ///< Invalid parameter provided
} sparkfun_hc_sr04_rcwl_9610a_error_t;

/**
 * @brief Driver configuration structure
 */
typedef struct {
  sparkfun_hc_sr04_rcwl_9610a_connection_type_t connection_type;  ///< Communication mode
  sparkfun_hc_sr04_rcwl_9610a_unit_t unit;                        ///< Measurement unit
  uint8_t i2c_address;                           ///< I2C address (if using I2C)
  mikroe_i2c_handle_t i2c_handle;                ///< Optional: Pre-configured I2C handle (NULL for auto)
  mikroe_uart_handle_t uart_handle;              ///< Optional: Pre-configured UART handle (NULL for auto)
} sparkfun_hc_sr04_rcwl_9610a_config_t;

/**
 * @brief Internal driver attributes
 */
typedef struct {
  i2c_master_t i2c_context;           ///< I2C master context
  uart_t uart_context;                ///< UART context
  uint8_t uart_rx_buffer[10];         ///< UART receive buffer
  uint8_t uart_tx_buffer[10];         ///< UART transmit buffer
} sparkfun_hc_sr04_rcwl_9610a_attributes_t;

/**
 * @brief RCWL-9610A driver instance structure
 */
typedef struct {
  sparkfun_hc_sr04_rcwl_9610a_connection_type_t connection_type;  ///< Active connection type
  sparkfun_hc_sr04_rcwl_9610a_unit_t measurement_unit;            ///< Current measurement unit
  sparkfun_hc_sr04_rcwl_9610a_attributes_t attributes;            ///< Communication attributes
  float last_measurement;                        ///< Last distance measurement
  bool is_initialized;                           ///< Initialization status flag
} sparkfun_hc_sr04_rcwl_9610a_driver_instance_t;

/*******************************************************************************
 * Public Function Declarations
 ******************************************************************************/

/**
 * @brief Create and allocate a new driver instance
 * @return Pointer to new instance, or NULL on failure
 */
sparkfun_hc_sr04_rcwl_9610a_driver_instance_t* sparkfun_hc_sr04_rcwl_9610a_driver_create_instance(void);

/**
 * @brief Initialize driver with configuration
 * @param[in] self Pointer to driver instance
 * @param[in] config Configuration structure
 * @return Error code
 */
sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_init(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                            const sparkfun_hc_sr04_rcwl_9610a_config_t *config);

/**
 * @brief Set I2C communication handle
 * @param[in] self Pointer to driver instance
 * @param[in] handle I2C handle
 */
void sparkfun_hc_sr04_rcwl_9610a_driver_set_i2c_handle(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                       mikroe_i2c_handle_t handle);

/**
 * @brief Set UART communication handle
 * @param[in] self Pointer to driver instance
 * @param[in] handle UART handle
 */
void sparkfun_hc_sr04_rcwl_9610a_driver_set_uart_handle(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                        mikroe_uart_handle_t handle);

/**
 * @brief Begin communication with sensor
 * @param[in] self Pointer to driver instance
 * @return Error code
 */
sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_begin(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);

/**
 * @brief Perform distance measurement
 * @param[in] self Pointer to driver instance
 * @return Error code
 */
sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_measure_distance(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);

/**
 * @brief Get last measurement value
 * @param[in] self Pointer to driver instance
 * @return Distance value in configured unit, or -1.0 on error
 */
float sparkfun_hc_sr04_rcwl_9610a_driver_get_distance(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);

/**
 * @brief Set measurement unit
 * @param[in] self Pointer to driver instance
 * @param[in] unit Desired measurement unit
 * @return Error code
 */
sparkfun_hc_sr04_rcwl_9610a_error_t sparkfun_hc_sr04_rcwl_9610a_driver_set_unit(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self,
                                                                                sparkfun_hc_sr04_rcwl_9610a_unit_t unit);

/**
 * @brief Convert error code to string
 * @param[in] error Error code
 * @return Human-readable error string
 */
const char* sparkfun_hc_sr04_rcwl_9610a_driver_error_to_string(sparkfun_hc_sr04_rcwl_9610a_error_t error);

/**
 * @brief Check if driver is initialized
 * @param[in] self Pointer to driver instance
 * @return true if initialized, false otherwise
 */
bool sparkfun_hc_sr04_rcwl_9610a_driver_is_initialized(const sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);

/**
 * @brief Close communication and free driver instance
 * @param[in] self Pointer to driver instance
 */
void sparkfun_hc_sr04_rcwl_9610a_driver_destroy(sparkfun_hc_sr04_rcwl_9610a_driver_instance_t *self);

#ifdef __cplusplus
}
#endif

#endif /* SPARKFUN_HC_SR04_RCWL_9610A_DRIVER_H_ */
