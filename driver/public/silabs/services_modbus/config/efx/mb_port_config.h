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

// <<< sl:start pin_tool >>>
// <timer> MBTIMER
// $[TIMER_MBTIMER]
#ifndef MBTIMER_PERIPHERAL                      
#define MBTIMER_PERIPHERAL                       TIMER0
#endif
#ifndef MBTIMER_PERIPHERAL_NO                   
#define MBTIMER_PERIPHERAL_NO                    0
#endif
// [TIMER_MBTIMER]$

// <usart signal=TX,RX> MBUSART
// $[USART_MBUSART]
#ifndef MBUSART_PERIPHERAL        
#define MBUSART_PERIPHERAL         USART0
#endif
#ifndef MBUSART_PERIPHERAL_NO     
#define MBUSART_PERIPHERAL_NO      0
#endif

// USART1 TX on PA05
#ifndef MBUSART_TX_PORT           
#define MBUSART_TX_PORT            SL_GPIO_PORT_A
#endif
#ifndef MBUSART_TX_PIN            
#define MBUSART_TX_PIN             5
#endif

// USART1 RX on PA06
#ifndef MBUSART_RX_PORT           
#define MBUSART_RX_PORT            SL_GPIO_PORT_A
#endif
#ifndef MBUSART_RX_PIN            
#define MBUSART_RX_PIN             6
#endif

// [USART_MBUSART]$

// <<< sl:end pin_tool >>>

#define MBTIMER_TIMER_NO MBTIMER_PERIPHERAL_NO
#define MBUSART_USART_NO MBUSART_PERIPHERAL_NO

#ifdef __cplusplus
extern "C"
}
#endif

#endif // MB_PORT_CONFIG_H_H
