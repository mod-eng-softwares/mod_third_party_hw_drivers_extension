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

#ifndef SPI_NAND_EXAMPLE_H
#define SPI_NAND_EXAMPLE_H

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void spi_nand_example_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void spi_nand_example_process_action(void);

#endif // SPI_NAND_EXAMPLE_H
