/***************************************************************************//**
 * @file mb_port_config.h
 * @brief Configuration file for Modbus Timer.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef MB_PORT_CONFIG_H_H
#define MB_PORT_CONFIG_H_H

#ifdef __cplusplus
extern "C" {
#endif

#define ULP_TIMER_0  0   ///< ULP Timer 0 Instance
#define ULP_TIMER_1  1   ///< ULP Timer 1 Instance
#define ULP_TIMER_2  2   ///< ULP Timer 2 Instance
#define ULP_TIMER_3  3   ///< ULP Timer 3 Instance

// <<< Use Configuration Wizard in Context Menu >>>
// <h> ULP Timer selection

// <o ULP_TIMER_INSTANCE> ULP Timer Instance
// <ULP_TIMER_0=> ULP Timer 0
// <ULP_TIMER_1=> ULP Timer 1
// <ULP_TIMER_2=> ULP Timer 2
// <ULP_TIMER_3=> ULP Timer 3
// <i> Default: ULP_TIMER_0
#define ULP_TIMER_INSTANCE ULP_TIMER_0

// </h>
// <<< end of configuration section >>>

#ifdef __cplusplus
extern "C"
}
#endif

#endif // MB_PORT_CONFIG_H_H
