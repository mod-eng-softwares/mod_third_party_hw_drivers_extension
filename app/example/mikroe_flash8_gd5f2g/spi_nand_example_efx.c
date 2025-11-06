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
#include <stdlib.h>
#include "em_usart.h"
#include "app_assert.h"
#include "app_log.h"
#include "nand_spi.h"
#include "mikroe_gd5f2g.h"
#include <stdio.h>

#define USE_PAGE_ADDRESSING 0
#define USE_TEST_OOB_AREA   1

#define TEST_BUFFER_SIZE    4096
#define TEST_OFFSET         12

static struct nand_spi nand_spi;
static uint8_t test_rand_buffer[TEST_BUFFER_SIZE];
static uint8_t test_write_buffer[TEST_BUFFER_SIZE];
static uint8_t test_read_buffer[TEST_BUFFER_SIZE];

static uint16_t page_size;
static uint32_t block_size;
static uint8_t oob_size;
static uint16_t page_num;
static uint16_t block_num;

static void gen_random_buffer(uint8_t *buf, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    buf[i] = (uint8_t)rand();
  }
}

static void buffer_fill_zero(uint8_t *buf, size_t size)
{
  while (size--) {
    *buf++ = 0;
  }
}

static size_t buffer_compare(uint8_t *buf1, uint8_t *buf2, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    if (buf1[i] != buf2[i]) {
      return i;
    }
  }
  return 0;
}

static void buffer_copy(uint8_t *dst, uint8_t *src, size_t size)
{
  while (size--) {
    *dst++ = *src++;
  }
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void spi_nand_example_init(void)
{
  sl_status_t sc = mikroe_gd5f2g_init(&nand_spi);
  app_assert_status(sc);

#if USE_TEST_OOB_AREA
  sc = nand_enable_ecc(&nand_spi, false);
  app_assert_status(sc);
#endif

  page_size = nand_get_page_size(&nand_spi);
  if (page_size > TEST_BUFFER_SIZE) {
    page_size = TEST_BUFFER_SIZE;
  }
  block_size = nand_get_block_size(&nand_spi);
  oob_size = nand_get_oob_size(&nand_spi);
  page_num = nand_get_page_num(&nand_spi);
  block_num = nand_get_block_num(&nand_spi);
  app_assert(page_size > TEST_OFFSET,
             "Error: read offset must be within a page %d", page_size);
  app_log("Test is started...\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void spi_nand_example_process_action(void)
{
  sl_status_t sc;

  for (uint16_t block = 0; block < block_num; block++) {
#if USE_PAGE_ADDRESSING
    sc = nand_erase_block(&nand_spi, block);
    app_assert_status(sc);
#else
    sc = nand_erase(&nand_spi,
                    (block << 6) << 12,
                    block_size);
    app_assert_status(sc);
#endif
    for (uint16_t page = 0; page < page_num; page++) {
      gen_random_buffer(test_rand_buffer, page_size);
      buffer_copy(test_write_buffer, test_rand_buffer, page_size);
#if USE_PAGE_ADDRESSING
      buffer_copy(test_write_buffer, test_rand_buffer, oob_size);
      sc = nand_write_page(&nand_spi,
                           (block << 6) | page, // 6-bit page(row) address
                           TEST_OFFSET,
                           &test_write_buffer[TEST_OFFSET],
                           page_size - TEST_OFFSET);

      buffer_fill_zero(test_read_buffer, page_size);
      sc = nand_read_page(&nand_spi,
                          (block << 6) | page, // 6-bit page(row) address
                          TEST_OFFSET,
                          &test_read_buffer[TEST_OFFSET],
                          page_size - TEST_OFFSET);
      app_assert_status(sc);
#else
      sc = nand_write(&nand_spi,
                      (((block << 6) | page) << 12) + TEST_OFFSET, // 12-bit column address
                      &test_write_buffer[TEST_OFFSET],
                      page_size - TEST_OFFSET);
      app_assert_status(sc);

      buffer_fill_zero(test_read_buffer, page_size);
      sc = nand_read(&nand_spi,
                     (((block << 6) | page) << 12) + TEST_OFFSET,
                     &test_read_buffer[TEST_OFFSET],
                     page_size - TEST_OFFSET);
      app_assert_status(sc);
#endif
      size_t index = buffer_compare(&test_rand_buffer[TEST_OFFSET],
                                    &test_read_buffer[TEST_OFFSET],
                                    page_size - TEST_OFFSET);
      app_assert(0 == index,
                 "Error: data is not matched at %d", index);

#if USE_TEST_OOB_AREA
      buffer_copy(test_write_buffer, test_rand_buffer, oob_size);
      sc = nand_write_page_oob(&nand_spi,
                               (block << 6) | page, // 6-bit page(row) address
                               test_write_buffer, oob_size);

      buffer_fill_zero(test_read_buffer, oob_size);
      sc = nand_read_page_oob(&nand_spi,
                              (block << 6) | page, // 6-bit page(row) address
                              test_read_buffer, oob_size);
      app_assert_status(sc);

      index = buffer_compare(test_rand_buffer,
                             test_read_buffer,
                             oob_size);
      app_assert(0 == index,
                 "Error: oob data is not matched at %d", index);
#endif
    }
    app_log("Test[%d/%d] is passed, block: %d, number of pages: %d\n",
            block, block_num, block, page_num);
  }
}
