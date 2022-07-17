/**
 * @file cst826.h
 * @brief CST826 driver header file.
 * @version 0.1
 * @date 2021-03-07
 * 
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"

#include "bsp_i2c.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CST826_ADDR   (0x15)   //cst826
// #define CST826_ADDR   (0x1A)   //cst918

/** @brief CST826 register map and function codes */
#define CST826_FW_VER          (0xA6)
#define CST826_VENDOR_ID       (0xA8)
#define CST826_PROJECT_ID      (0xA9)
#define CST826_CHIP_ID         (0xAA)
#define CST826_CHIP_CHECKSUM   (0xAC)

#define CST826_TP_NUM_REG      (0x02)
#define CST826_TP1_POS_REG     (0x03)
#define CST826_TP2_POS_REG     (0x09)


esp_err_t cst826_read_byte(uint8_t reg_addr, uint8_t *data);
esp_err_t cst826_read_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data);
esp_err_t cst826_init(void);
esp_err_t cst826_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y);


#ifdef __cplusplus
}
#endif

