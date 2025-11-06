/***************************************************************************//**
 * @file   porttimer_efx.c
 * @brief  Porting Timer for EFx devices
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include <stdbool.h>
#include "si91x_device.h"
#include "rsi_timers.h"
#include "rsi_rom_ulpss_clk.h"
#include "clock_update.h"

#include "mb.h"
#include "mbport.h"
#include "mb_port_config.h"

#define ULP_TIMER0_IRQHandler IRQ002_Handler // ULP Timer0 IRQ Handler
#define ULP_TIMER1_IRQHandler IRQ003_Handler // ULP Timer1 IRQ Handler
#define ULP_TIMER2_IRQHandler IRQ004_Handler // ULP Timer2 IRQ Handler
#define ULP_TIMER3_IRQHandler IRQ005_Handler // ULP Timer3 IRQ Handler

#define TIMER_CONCAT2(x, y)                    x ## y
#define TIMER_CONCAT_DEV(periph_nbr)          TIMER_CONCAT2(TIMER, periph_nbr)
#define TIMER_CONCAT3(x, y, z)                x ## y ## z
#define TIMER_CONCAT_IRQ_NUMBER(periph_nbr)   TIMER_CONCAT3(TIMER, periph_nbr, _IRQn)
#define TIMER_CONCAT_IRQ_HANDLER(periph_nbr)  TIMER_CONCAT3(ULP_TIMER, periph_nbr, _IRQHandler)

#define TIMER_DEV          TIMER_CONCAT_DEV(ULP_TIMER_INSTANCE)
#define TIMER_CLK          TIMER_CONCAT_CLK(ULP_TIMER_INSTANCE)
#define TIMER_IRQ          TIMER_CONCAT_IRQ_NUMBER(ULP_TIMER_INSTANCE)
#define TIMER_IRQHandler   TIMER_CONCAT_IRQ_HANDLER(ULP_TIMER_INSTANCE)

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
  // Timer Peripheral input clock source configuration
  RSI_ULPSS_TimerClkConfig(ULPCLK, ENABLE_STATIC_CLK, 0, ULP_TIMER_REF_CLK, 0);

  // Reading ulp-timer peripheral clock frequency
  uint32_t ulp_timer_clock = RSI_CLK_GetBaseClock(ULPSS_TIMER);
  uint32_t clock_cycles_per_us = ulp_timer_clock / 1000000;
  uint32_t match_value = usTim1Timerout50us * clock_cycles_per_us * 50;

  // Configure periodic timer with count down mode, this timer will be always running
  RSI_TIMERS_SetTimerMode(TIMERS, ONESHOT_TIMER, ULP_TIMER_INSTANCE);
  RSI_TIMERS_SetTimerType(TIMERS, COUNTER_DOWN_MODE, ULP_TIMER_INSTANCE);
  // RSI_TIMERS_SetDirection(TIMERS, TIMER_0, UP_COUNTER);
  // TIMERS->MATCH_CTRL[TIMER_0].MCUULP_TMR_CNTRL_b.COUNTER_UP = 1;
  RSI_TIMERS_SetMatch(TIMERS, ULP_TIMER_INSTANCE, match_value);

  return TRUE;
}

void vMBPortTimersEnable(void)
{
  RSI_TIMERS_InterruptEnable(TIMERS, ULP_TIMER_INSTANCE);
  NVIC_EnableIRQ(TIMER_IRQ);
  RSI_TIMERS_TimerStart(TIMERS, ULP_TIMER_INSTANCE);
}

void vMBPortTimersDisable(void)
{
  RSI_TIMERS_InterruptDisable(TIMERS, ULP_TIMER_INSTANCE);
  NVIC_DisableIRQ(TIMER_IRQ);
  RSI_TIMERS_TimerStop(TIMERS, ULP_TIMER_INSTANCE);
}

void TIMER_IRQHandler(void)
{
  uint8_t int_status = RSI_TIMERS_InterruptStatus(TIMERS, ULP_TIMER_INSTANCE);
  if (int_status) {
    // Clearing interrupt if not clear
    RSI_TIMERS_InterruptClear(TIMERS, ULP_TIMER_INSTANCE);
  }
  // Timer expired, call the callback function
  pxMBPortCBTimerExpired();
}
