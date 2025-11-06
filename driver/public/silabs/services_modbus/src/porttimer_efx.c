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
#include "em_cmu.h"
#include "em_timer.h"

#include "mb.h"
#include "mbport.h"
#include "mb_port_config.h"

#define TIMER_CONCAT2(x, y)                    x ## y
#define TIMER_CONCAT_DEV(periph_nbr)          TIMER_CONCAT2(TIMER, periph_nbr)
#define TIMER_CONCAT_CLK(periph_nbr)          TIMER_CONCAT2(cmuClock_TIMER, periph_nbr)
#define TIMER_CONCAT3(x, y, z)                x ## y ## z
#define TIMER_CONCAT_IRQ_NUMBER(periph_nbr)   TIMER_CONCAT3(TIMER, periph_nbr, _IRQn)
#define TIMER_CONCAT_IRQ_HANDLER(periph_nbr)  TIMER_CONCAT3(TIMER, periph_nbr, _IRQHandler)

#define TIMER_DEV          TIMER_CONCAT_DEV(MBTIMER_TIMER_NO)
#define TIMER_CLK          TIMER_CONCAT_CLK(MBTIMER_TIMER_NO)
#define TIMER_IRQ          TIMER_CONCAT_IRQ_NUMBER(MBTIMER_TIMER_NO)
#define TIMER_IRQHandler   TIMER_CONCAT_IRQ_HANDLER(MBTIMER_TIMER_NO)

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
  CMU_ClockEnable(TIMER_CLK, true);

  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

  // Do not start counter upon initialization
  timerInit.enable = false;
  timerInit.prescale = timerPrescale2;

  TIMER_Init(TIMER_DEV, &timerInit);

  uint32_t timerFreq = CMU_ClockFreqGet(TIMER_CLK) / (timerInit.prescale + 1);
  TIMER_TopSet(TIMER_DEV, (usTim1Timerout50us * timerFreq / 20000)); // 1000 for 1ms, 20000 for 50us
  return TRUE;
}

void vMBPortTimersEnable(void)
{
  TIMER_IntDisable(TIMER_DEV, _TIMER_IEN_MASK);
  TIMER_IntEnable(TIMER_DEV, TIMER_IEN_OF);
  NVIC_ClearPendingIRQ(TIMER_IRQ);
  NVIC_EnableIRQ(TIMER_IRQ);
  
  TIMER_CounterSet(TIMER_DEV, 0);
  TIMER_Enable(TIMER_DEV, true);
}

void vMBPortTimersDisable(void)
{
  NVIC_DisableIRQ(TIMER_IRQ);
  TIMER_Enable(TIMER_DEV, false);
}

void TIMER_IRQHandler(void)
{
  uint32_t flags = TIMER_IntGet(TIMER_DEV);
  TIMER_IntClear(TIMER_DEV, flags);
  // Timer expired, call the callback function
  pxMBPortCBTimerExpired();
}
