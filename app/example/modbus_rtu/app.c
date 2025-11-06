/***************************************************************************//**
 * @file app.c
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"

#if (defined(SLI_SI917))
#include "sl_si91x_usart.h"
#define USART_INSTANCE_USED            ULPUART
#else
#define USART_INSTANCE_USED            0
#endif

#define REG_COILS_START                1000
#define REG_COILS_SIZE                 16
static unsigned char ucRegCoilsBuf[REG_COILS_SIZE / 8];

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START                2000
#define REG_INPUT_NREGS                4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/**************************************************************************//**
 * @brief
 *    Main function
 *****************************************************************************/
void app_init(void)
{
  eMBInit(MB_RTU, 0x0A, USART_INSTANCE_USED, 115200, MB_PAR_NONE, 1);

  // Enable the Modbus protocol stack
  eMBEnable();
}

void app_process_action(void)
{
  // Poll the Modbus state machine
  eMBPoll();

  // Here we simply count the number of poll cycles.
  usRegInputBuf[1]++;
}

eMBErrorCode
eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int             iRegIndex;

  if ((usAddress >= REG_INPUT_START)
      && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
    iRegIndex = ( int )(usAddress - usRegInputStart);
    while (usNRegs > 0)
    {
      *pucRegBuffer++ =
        ( unsigned char )(usRegInputBuf[iRegIndex] >> 8);
      *pucRegBuffer++ =
        ( unsigned char )(usRegInputBuf[iRegIndex] & 0xFF);
      iRegIndex++;
      usNRegs--;
    }
  } else {
    eStatus = MB_ENOREG;
  }

  return eStatus;
}

eMBErrorCode
eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode)
{
  eMBErrorCode    eStatus = MB_ENOERR;
  short           iNCoils = ( short )usNCoils;
  unsigned short  usBitOffset;

  // Check if we have registers mapped at this block.
  if ((usAddress >= REG_COILS_START)
      && (usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE)) {
    usBitOffset = ( unsigned short )(usAddress - REG_COILS_START);
    switch (eMode)
    {
      // Read current values and pass to protocol stack.
      case MB_REG_READ:
        while (iNCoils > 0)
        {
          *pucRegBuffer++ =
            xMBUtilGetBits(ucRegCoilsBuf, usBitOffset,
                           ( unsigned char )(iNCoils
                                             > 8 ? 8
                                             :iNCoils));
          iNCoils -= 8;
          usBitOffset += 8;
        }
        break;

      // Update current register values.
      case MB_REG_WRITE:
        while (iNCoils > 0)
        {
          xMBUtilSetBits(ucRegCoilsBuf, usBitOffset,
                         ( unsigned char )(iNCoils > 8 ? 8 : iNCoils),
                         *pucRegBuffer++);
          iNCoils -= 8;
        }
        break;
    }
  } else {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
  (void)pucRegBuffer;
  (void)usAddress;
  (void)usNRegs;
  (void)eMode;
  return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
  (void)pucRegBuffer;
  (void)usAddress;
  (void)usNDiscrete;
  return MB_ENOREG;
}
