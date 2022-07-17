/**
 * @file gt911.h
 * @brief GT911 driver header file.
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


/** @brief GT911 register map and function codes */
#define GT_CTRL_REG 	(0x8040) 
#define GT_CFGS_REG 	(0x8047)  
#define GT_CHECK_REG 	(0x80FF)  

#define GT_PID_REG 		(0x8140)   
#define GT_GSTID_REG 	(0x814E)  
#define GT_TP1_REG 		(0x8150)  
#define GT_TP2_REG 		(0x8158)   
#define GT_TP3_REG 		(0x8160) 
#define GT_TP4_REG 		(0x8168)   
#define GT_TP5_REG 		(0x8170)   

esp_err_t gt911_read_bytes(uint16_t reg_addr, size_t data_len, uint8_t *data);
esp_err_t gt911_write_bytes(uint16_t reg_addr, size_t data_len, uint8_t *data);
esp_err_t gt911_init(void);
esp_err_t gt911_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif
