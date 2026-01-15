/***************************************************************************//**
 * @file mikroe_mpu6050.h
 * @brief SCL MPU6050 Prototypes
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

#ifndef MIKROE_MPU6050_H_
#define MIKROE_MPU6050_H_

#include "sl_status.h"
#include "accel8.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_mpu6050 - Accel 8 Click
 ******************************************************************************/
/**
* \defgroup registers Registers
* \{
*/
#define MIKROE_MPU6050_REG_SELF_TEST_X           0x0D
#define MIKROE_MPU6050_REG_SELF_TEST_Y           0x0E
#define MIKROE_MPU6050_REG_SELF_TEST_Z           0x0F
#define MIKROE_MPU6050_REG_SELF_TEST_A           0x10
#define MIKROE_MPU6050_REG_SMPLRT_DIV            0x19
#define MIKROE_MPU6050_REG_CONFIG                0x1A
#define MIKROE_MPU6050_REG_GYRO_CONFIG           0x1B
#define MIKROE_MPU6050_REG_ACCEL_CONFIG          0x1C
#define MIKROE_MPU6050_REG_FIFO_EN               0x23
#define MIKROE_MPU6050_REG_I2C_MST_CTRL          0x24
#define MIKROE_MPU6050_REG_I2C_SLV0_ADDR         0x25
#define MIKROE_MPU6050_REG_I2C_SLV0_REG          0x26
#define MIKROE_MPU6050_REG_I2C_SLV0_CTRL         0x27
#define MIKROE_MPU6050_REG_I2C_SLV1_ADDR         0x28
#define MIKROE_MPU6050_REG_I2C_SLV1_REG          0x29
#define MIKROE_MPU6050_REG_I2C_SLV1_CTRL         0x2A
#define MIKROE_MPU6050_REG_I2C_SLV2_ADDR         0x2B
#define MIKROE_MPU6050_REG_I2C_SLV2_REG          0x2C
#define MIKROE_MPU6050_REG_I2C_SLV2_CTRL         0x2D
#define MIKROE_MPU6050_REG_I2C_SLV3_ADDR         0x2E
#define MIKROE_MPU6050_REG_I2C_SLV3_REG          0x2F
#define MIKROE_MPU6050_REG_I2C_SLV3_CTRL         0x30
#define MIKROE_MPU6050_REG_I2C_SLV4_ADDR         0x31
#define MIKROE_MPU6050_REG_I2C_SLV4_REG          0x32
#define MIKROE_MPU6050_REG_I2C_SLV4_DO           0x33
#define MIKROE_MPU6050_REG_I2C_SLV4_CTRL         0x34
#define MIKROE_MPU6050_REG_I2C_SLV4_DI           0x35
#define MIKROE_MPU6050_REG_I2C_MST_STATUS        0x36
#define MIKROE_MPU6050_REG_INT_PIN_CFG           0x37
#define MIKROE_MPU6050_REG_INT_ENABLE            0x38
#define MIKROE_MPU6050_REG_INT_STATUS            0x3A
#define MIKROE_MPU6050_REG_ACCEL_XOUT_H          0x3B
#define MIKROE_MPU6050_REG_ACCEL_XOUT_L          0x3C
#define MIKROE_MPU6050_REG_ACCEL_YOUT_H          0x3D
#define MIKROE_MPU6050_REG_ACCEL_YOUT_L          0x3E
#define MIKROE_MPU6050_REG_ACCEL_ZOUT_H          0x3F
#define MIKROE_MPU6050_REG_ACCEL_ZOUT_L          0x40
#define MIKROE_MPU6050_REG_TEMP_OUT_H            0x41
#define MIKROE_MPU6050_REG_TEMP_OUT_L            0x42
#define MIKROE_MPU6050_REG_GYRO_XOUT_H           0x43
#define MIKROE_MPU6050_REG_GYRO_XOUT_L           0x44
#define MIKROE_MPU6050_REG_GYRO_YOUT_H           0x45
#define MIKROE_MPU6050_REG_GYRO_YOUT_L           0x46
#define MIKROE_MPU6050_REG_GYRO_ZOUT_H           0x47
#define MIKROE_MPU6050_REG_GYRO_ZOUT_L           0x48
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_00      0x49
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_01      0x4A
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_02      0x4B
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_03      0x4C
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_04      0x4D
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_05      0x4E
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_06      0x4F
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_07      0x50
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_08      0x51
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_09      0x52
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_10      0x53
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_11      0x54
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_12      0x55
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_13      0x56
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_14      0x57
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_15      0x58
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_16      0x59
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_17      0x5A
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_18      0x5B
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_19      0x5C
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_20      0x5D
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_21      0x5E
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_22      0x5F
#define MIKROE_MPU6050_REG_EXT_SENS_DATA_23      0x60
#define MIKROE_MPU6050_REG_I2C_SLV0_DO           0x63
#define MIKROE_MPU6050_REG_I2C_SLV1_DO           0x64
#define MIKROE_MPU6050_REG_I2C_SLV2_DO           0x65
#define MIKROE_MPU6050_REG_I2C_SLV3_DO           0x66
#define MIKROE_MPU6050_REG_I2C_MST_DELAY_CTRL    0x67
#define MIKROE_MPU6050_REG_SIGNAL_PATH_RESET     0x68
#define MIKROE_MPU6050_REG_USER_CTRL             0x6A
#define MIKROE_MPU6050_REG_PWR_MGMT_1            0x6B
#define MIKROE_MPU6050_REG_PWR_MGMT_2            0x6C
#define MIKROE_MPU6050_REG_FIFO_COUNTH           0x72
#define MIKROE_MPU6050_REG_FIFO_COUNTL           0x73
#define MIKROE_MPU6050_REG_FIFO_R_W              0x74
#define MIKROE_MPU6050_REG_WHO_AM_I              0x75
/** \} */

/**
* \defgroup configuration Configuration
* \{
*/
#define MIKROE_MPU6050_CFG_EXT_SYNC_INPUT_DISABLED    0x00 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_TEMP_OUTPUT       0x01 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_GYRO_X_OUTPUT     0x02 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_GYRO_Y_OUTPUT     0x03 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_GYRO_Z_OUTPUT     0x04 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_ACCEL_X_OUTPUT    0x05 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_ACCEL_Y_OUTPUT    0x06 << 3
#define MIKROE_MPU6050_CFG_EXT_SYNC_ACCEL_Z_OUTPUT    0x07 << 3
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A260_G256      0x00
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A184_G188      0x01
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A94_G98        0x02
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A44_G42        0x03
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A21_G20        0x04
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A10_G10        0x05
#define MIKROE_MPU6050_CFG_DLPF_CFG_BW_A5_G5          0x06
/** \} */

/**
* \defgroup gyroscope_configuration Gyroscope Configuration
* \{
*/
#define MIKROE_MPU6050_GYRO_CFG_X_SELF_TEST                 0x80
#define MIKROE_MPU6050_GYRO_CFG_Y_SELF_TEST                 0x40
#define MIKROE_MPU6050_GYRO_CFG_Z_SELF_TEST                 0x20
#define MIKROE_MPU6050_GYRO_CFG_FULL_SCALE_RANGE_250dbs     0x00
#define MIKROE_MPU6050_GYRO_CFG_FULL_SCALE_RANGE_500dbs     0x08
#define MIKROE_MPU6050_GYRO_CFG_FULL_SCALE_RANGE_1000dbs    0x10
#define MIKROE_MPU6050_GYRO_CFG_FULL_SCALE_RANGE_2000dbs    0x18
/** \} */

/**
* \defgroup accelerometer_configuration Accelerometer Configuration
* \{
*/
#define MIKROE_MPU6050_ACCEL_CFG_X_SELF_TEST             0x80
#define MIKROE_MPU6050_ACCEL_CFG_Y_SELF_TEST             0x40
#define MIKROE_MPU6050_ACCEL_CFG_Z_SELF_TEST             0x20
#define MIKROE_MPU6050_ACCEL_CFG_FULL_SCALE_RANGE_2g     0x00
#define MIKROE_MPU6050_ACCEL_CFG_FULL_SCALE_RANGE_4g     0x08
#define MIKROE_MPU6050_ACCEL_CFG_FULL_SCALE_RANGE_8g     0x10
#define MIKROE_MPU6050_ACCEL_CFG_FULL_SCALE_RANGE_16g    0x18
/** \} */

/**
* \defgroup fifo_enable FIFO Enable
* \{
*/
#define MIKROE_MPU6050_FIFO_ENABLE_TEMP                  0x80
#define MIKROE_MPU6050_FIFO_ENABLE_X_AXIS_GYRO           0x40
#define MIKROE_MPU6050_FIFO_ENABLE_Y_AXIS_GYRO           0x20
#define MIKROE_MPU6050_FIFO_ENABLE_Z_AXIS_GYRO           0x10
#define MIKROE_MPU6050_FIFO_ENABLE_ACCEL                 0x08
#define MIKROE_MPU6050_FIFO_ENABLE_EXT_SENS_DATA_SLV2    0x04
#define MIKROE_MPU6050_FIFO_ENABLE_EXT_SENS_DATA_SLV1    0x02
#define MIKROE_MPU6050_FIFO_ENABLE_EXT_SENS_DATA_SLV0    0x01
/** \} */

/**
* \defgroup i2c_master_control I2C Master Control
* \{
*/
#define MIKROE_MPU6050_I2C_MST_CTRL_MUL_MST_ENABLE               0x80
#define MIKROE_MPU6050_I2C_MST_CTRL_WAIT_FOR_ES                  0x40
#define MIKROE_MPU6050_I2C_MST_CTRL_EXT_SENS_DATA_SLV3           0x20
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_RESTART_BETWEEN_READS    0x00
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_STOP_AND_START           0x10
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_348kHz             0x00
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_333kHz             0x01
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_320kHz             0x02
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_308kHz             0x03
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_296kHz             0x04
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_286kHz             0x05
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_276kHz             0x06
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_367kHz             0x07
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_358kHz             0x08
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_500kHz             0x09
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_471kHz             0x0A
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_444kHz             0x0B
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_421kHz             0x0C
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_400kHz             0x0D
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_381kHz             0x0E
#define MIKROE_MPU6050_I2C_MST_CTRL_I2C_CLOCK_364kHz             0x0F
/** \} */

/**
* \defgroup int_pin_bypass_enable_cfg INT Pin / Bypass Enable Configuration
* \{
*/
#define MIKROE_MPU6050_INTC_INT_PIN_IS_ACTIVE_HIGH                 0x00
#define MIKROE_MPU6050_INTC_INT_PIN_IS_ACTIVE_LOW                  0x80
#define MIKROE_MPU6050_INTC_INT_PIN_IS_CONFIGURED_AS_PUSH_PULL     0x00
#define MIKROE_MPU6050_INTC_INT_PIN_IS_CONFIGURED_AS_OPEN_DRAIN    0x40
#define MIKROE_MPU6050_INTC_LATCH_INT_ENABLE                       0x20
#define MIKROE_MPU6050_INTC_INT_READ_CLEAR                         0x10
#define MIKROE_MPU6050_INTC_FSYNC_INT_LEVEL_ACTIVE_HIGH            0x00
#define MIKROE_MPU6050_INTC_FSYNC_INT_LEVEL_ACTIVE_LOW             0x08
#define MIKROE_MPU6050_INTC_FSYNC_INT_ENABLE                       0x04
#define MIKROE_MPU6050_INTC_I2C_BYPASS_ENABLE                      0x02
#define MIKROE_MPU6050_INTC_I2C_BYPASS_DISABLE                     0x00
/** \} */

/**
* \defgroup int_enable Interrupt Enable
* \{
*/
#define MIKROE_MPU6050_INTE_FIFO_OFLOW_ENABLE     0x10
#define MIKROE_MPU6050_INTE_I2C_MST_INT_ENABLE    0x08
#define MIKROE_MPU6050_INTE_DATA_RDY_ENABLE       0x01
/** \} */

/**
* \defgroup int_status Interrupt Status
* \{
*/
#define MIKROE_MPU6050_INTS_FIFO_OFLOW     0x10
#define MIKROE_MPU6050_INTS_I2C_MST_INT    0x08
#define MIKROE_MPU6050_INTS_DATA_RDY       0x01
/** \} */

/**
* \defgroup data_output Data Output
* \{
*/
#define MIKROE_MPU6050_ACCEL_X_AXIS_DATA    0x3B
#define MIKROE_MPU6050_ACCEL_Y_AXIS_DATA    0x3D
#define MIKROE_MPU6050_ACCEL_Z_AXIS_DATA    0x3F
#define MIKROE_MPU6050_TEMP_DATA            0x41
#define MIKROE_MPU6050_GYRO_X_AXIS_DATA     0x43
#define MIKROE_MPU6050_GYRO_Y_AXIS_DATA     0x45
#define MIKROE_MPU6050_GYRO_Z_AXIS_DATA     0x47
/** \} */

/**
* \defgroup reset Reset
* \{
*/
#define MIKROE_MPU6050_GYRO_RESET     0x04
#define MIKROE_MPU6050_ACCEL_RESET    0x02
#define MIKROE_MPU6050_TEMP_RESET     0x01
/** \} */

/**
* \defgroup user_control User Control
* \{
*/
#define MIKROE_MPU6050_UC_FIFO_ENABLE         0x40
#define MIKROE_MPU6050_UC_I2C_MASTER_MODE     0x20
#define MIKROE_MPU6050_UC_FIFO_RESET          0x04
#define MIKROE_MPU6050_UC_I2C_MASTER_RESET    0x02
#define MIKROE_MPU6050_UC_SIG_COND_RESET      0x01
/** \} */

/**
* \defgroup power_management_1 Power Management 1
* \{
*/
#define MIKROE_MPU6050_PM1_DEVICE_RESET                          0x80
#define MIKROE_MPU6050_PM1_GO_TO_SLEEP                           0x40
#define MIKROE_MPU6050_PM1_CYCLE                                 0x20
#define MIKROE_MPU6050_PM1_TEMP_DISABLE                          0x08
#define MIKROE_MPU6050_PM1_CLKSEL_INTERNAL_8MHZ_OSCILLATOR       0x00
#define MIKROE_MPU6050_PM1_CLKSEL_PLL_WITH_X_AXIS_GYROSCOPE      0x01
#define MIKROE_MPU6050_PM1_CLKSEL_PLL_WITH_Y_AXIS_GYROSCOPE      0x02
#define MIKROE_MPU6050_PM1_CLKSEL_PLL_WITH_Z_AXIS_GYROSCOPE      0x03
#define MIKROE_MPU6050_PM1_CLKSEL_PLL_WITH_EXTERNAL_32_768kHz    0x04
#define MIKROE_MPU6050_PM1_CLKSEL_PLL_WITH_EXTERNAL_19_2kHz      0x05
#define MIKROE_MPU6050_PM1_CLKSEL_STOPS_THE_CLOCK                0x07
/** \} */

/**
* \defgroup power_management_2 Power Management 2
* \{
*/
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_WAKE_UP_FREQ_1_25Hz          0x00
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_WAKE_UP_FREQ_5Hz             0x40
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_WAKE_UP_FREQ_20Hz            0x80
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_WAKE_UP_FREQ_40Hz            0xC0
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_X_AXIS_ACCEL_STANDBY_MODE    0x20
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_Y_AXIS_ACCEL_STANDBY_MODE    0x10
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_Z_AXIS_ACCEL_STANDBY_MODE    0x08
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_X_AXIS_GYRO_STANDBY_MODE     0x04
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_Y_AXIS_GYRO_STANDBY_MODE     0x02
#define MIKROE_MPU6050_PM2_LP_WAKE_CTRL_Z_AXIS_GYRO_STANDBY_MODE     0x01
/** \} */

/**
* \defgroup slave_addresses Slave Addresses
* \{
*/
#define MIKROE_MPU6050_DEVICE_SLAVE_ADDRESS_ADD    0x69
#define MIKROE_MPU6050_DEVICE_SLAVE_ADDRESS_SEL    0x68
/** \} */

/***************************************************************************//**
* @brief
*    Structure for Click range
******************************************************************************/
typedef struct {
  uint8_t accel_range;
  uint16_t gyro_range;
} mikroe_mpu6050_range_t;

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the I2CSPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_set_i2cspm_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets default configuration.
 *
 * @param[in] accel_cfg
 *    Accel configuration.
 *
 * @param[in] gyro_cfg
 *    Gyroscope configuration
 *
 * @param[out] rng
 *    Range value object.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.

 ******************************************************************************/
sl_status_t mikroe_mpu6050_default_cfg(uint8_t accel_cfg,
                                       uint8_t gyro_cfg,
                                       mikroe_mpu6050_range_t *rng);

/***************************************************************************//**
 * @brief
 *    Write register content.
 *
 * @param[in] reg
 *    Register address.
 *
 * @param[in] data_buf
 *    Data buffer to be written.
 *
 * @param[in] len
 *    Number of the bytes in data buffer.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if register_buffer is null
 *    or n_data is invalid.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_generic_write(uint8_t reg,
                                         uint8_t *data_buf,
                                         uint8_t len);

/***************************************************************************//**
 * @brief
 *    Read register content.
 *
 * @param[in] reg
 *    Register address.
 *
 * @param[out] data_buf
 *    Output data buffer.
 *
 * @param[in] len
 *    Number of the bytes to be read.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if register_buffer is null
 *    or n_data is invalid.
 ******************************************************************************/
sl_status_t mikroe_mpu6050_generic_read(uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function reads 1 word from the desired register.
 *
 * @param[in] reg
 *    Register address.
 *
 * @return
 *    One word read from the register.
 *
 ******************************************************************************/
uint16_t mikroe_mpu6050_read_data(uint8_t reg);

/***************************************************************************//**
 * @brief
 *    Functions for read Temperature data in C.
 *
 * @return
 *    Temperature data.
 *    Temperature = Temp_OUT / 340 + 36.53
 *    Temp_OUT = 16bit Temp data
 ******************************************************************************/
float mikroe_mpu6050_get_temperature(void);

/***************************************************************************//**
 * @brief
 *    Functions for read Accel axis data.
 *
 * @param[out] x_axis
 *    Accel X axis data.
 *
 * @param[out] y_axis
 *    Accel Y axis data.
 *
 * @param[out] z_axis
 *    Accel Z axis data.
 ******************************************************************************/
void mikroe_mpu6050_get_accel_axis(int16_t *x_axis,
                                   int16_t *y_axis,
                                   int16_t *z_axis);

/***************************************************************************//**
 * @brief
 *    Functions for read Gyro axis data.
 *
 * @param[out] x_axis
 *    Gyro X axis data.
 *
 * @param[out] y_axis
 *    Gyro Y axis data.
 *
 * @param[out] z_axis
 *    Gyro Z axis data.
 ******************************************************************************/
void mikroe_mpu6050_get_gyro_axis(int16_t *x_axis,
                                  int16_t *y_axis,
                                  int16_t *z_axis);

/***************************************************************************//**
 * @brief
 *    Functions for read INT pin state.
 *
 * @return
 *    Interrupt state.
 ******************************************************************************/
uint8_t mikroe_mpu6050_get_interrupt(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup mikroe_mpu6050) */

#endif // _MIKROE_MPU6050_H_
