/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006-2018 Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Defines ------------------------------------------*/
/*! \defgroup modbus_cfg Modbus Configuration
 *
 * Most modules in the protocol stack are completly optional and can be
 * excluded. This is specially important if target resources are very small
 * and program memory space should be saved.
 *
 * All of these settings are available in the file mbconfig.h
 */
/*! \addtogroup modbus_cfg
 *  @{
 */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> Modbus transmission modes

// <q MB_ASCII_ENABLED> If Modbus ASCII support is enabled
// <i> Default: 0
#define MB_ASCII_ENABLED                        0 

// <q MB_RTU_ENABLED> If Modbus RTU support is enabled
// <i> Default: 1
#define MB_RTU_ENABLED                          1

// <q MB_TCP_ENABLED> If Modbus TCP support is enabled
// <i> Default: 0
#define MB_TCP_ENABLED                          0

// </h>

// <h> General

// <o MB_ASCII_TIMEOUT_SEC> The character timeout value for Modbus ASCII
// <i> Default: 1
// <i> The character timeout value is not fixed for Modbus ASCII and is therefore
// <i> a configuration option. It should be set to the maximum expected delay time
// <i> of the network.
#define MB_ASCII_TIMEOUT_SEC                    1

// <o MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS> Timeout to wait in ASCII prior to enabling transmitter
// <i> Default: 0
// <i> If defined the function calls vMBPortSerialDelay with the argument
// <i> MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
// <i> the serial transmitter is enabled. This is required because some
// <i> targets are so fast that there is no time between receiving and
// <i> transmitting the frame. If the master is to slow with enabling its 
// <i> receiver then he will not receive the response correctly.
#define MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    0

// <o MB_RTU_TIMEOUT_WAIT_BEFORE_SEND_MS> Timeout to wait in RTU prior to enabling transmitter
// <i> Default: 0
// <i> As per ASCII, this waits before replying, to help master devices
// <i> that are too slow to switch to listen mode
#define MB_RTU_TIMEOUT_WAIT_BEFORE_SEND_MS      0

// <o MB_FUNC_HANDLERS_MAX> Maximum number of Modbus functions codes the protocol stack should support
// <i> Default: 16
// <i> The maximum number of supported Modbus functions must be greater than
// <i> the sum of all enabled functions in this file and custom function
// <i> handlers. If set to small adding more functions will fail.
#define MB_FUNC_HANDLERS_MAX                    16

// <o MB_FUNC_OTHER_REP_SLAVEID_BUF> Number of bytes which should be allocated for the Report Slave ID command
// <i> Default: 32
// <i> This number limits the maximum size of the additional segment in the
// <i> report slave id function. See eMBSetSlaveID(  ) for more information on
// <i> how to set this value. It is only used if MB_FUNC_OTHER_REP_SLAVEID_ENABLED
// <i> is set to 1
#define MB_FUNC_OTHER_REP_SLAVEID_BUF           32

// </h>

// <h> Supported Functions

// <q MB_FUNC_OTHER_REP_SLAVEID_ENABLED> If the Report Slave ID function should be enabled
// <i> Default: 1
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED       1

// <q MB_FUNC_READ_INPUT_ENABLED> If the Read Input Registers function should be enabled
// <i> Default: 1
#define MB_FUNC_READ_INPUT_ENABLED              1

// <q MB_FUNC_READ_HOLDING_ENABLED> If the Read Holding Registers function should be enabled
// <i> Default: 1
#define MB_FUNC_READ_HOLDING_ENABLED            1

// <q MB_FUNC_WRITE_HOLDING_ENABLED> If the Write Single Register function should be enabled
// <i> Default: 1
#define MB_FUNC_WRITE_HOLDING_ENABLED           1

// <q MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED> If the Write Multiple registers function should be enabled
// <i> Default: 1
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  1

// <q MB_FUNC_READ_COILS_ENABLED> If the Read Coils function should be enabled
// <i> Default: 1
#define MB_FUNC_READ_COILS_ENABLED              1

// <q MB_FUNC_WRITE_COIL_ENABLED> If the Write Coils function should be enabled
// <i> Default: 1
#define MB_FUNC_WRITE_COIL_ENABLED              1

// <q MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED> If the Write Multiple Coils function should be enabled
// <i> Default: 1
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    1

// <q MB_FUNC_READ_DISCRETE_INPUTS_ENABLED> If the Read Discrete Inputs function should be enabled
// <i> Default: 1
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    1

// <q MB_FUNC_READWRITE_HOLDING_ENABLED> If the Read/Write Multiple Registers function should be enabled
// <i> Default: 1
#define MB_FUNC_READWRITE_HOLDING_ENABLED       1

// <q MB_FUNC_WRITE_FILE_ENABLED> If the Write File Record function should be enabled
// <i> Default: 0
#define MB_FUNC_WRITE_FILE_ENABLED              0

// <q MB_FUNC_READ_FILE_ENABLED> If the Read File Record function should be enabled
// <i> Default: 0
#define MB_FUNC_READ_FILE_ENABLED               0

// </h>

//------------- <<< end of configuration section >>> ---------------------------

/*! @} */
#ifdef __cplusplus
    PR_END_EXTERN_C
#endif
#endif
