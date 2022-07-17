/**
 * @file htu21.h
 * @brief HTU21 driver header file.
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

#include "esp_log.h"

#include "bsp_i2c.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    htu21_res_12bit_14bit       =  0x00, //RH=12bit, T=14bit
    htu21_res_8bit_12bit        =  0x01, //RH= 8bit, T=12bit
    htu21_res_10bit_13bit       =  0x80, //RH=10bit, T=13bit
    htu21_res_11bit_11bit       =  0x81, //RH=11bit, T=11bit
} htu21_res_t;

typedef enum {
    htu21_heater_on             =  0x04, //heater on
    htu21_heater_off            =  0x00, //heater off
} htu21_heater_t;


/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t htu21_init(void);

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t htu21_softreset(void);

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t htu21_set_mode(void);

/**
 * @brief 
 * 
 * @param measure_type 
 * @return esp_err_t 
 */
esp_err_t htu21_start_measure(uint8_t measure_type);

/**
 * @brief 
 * 
 * @param data 
 * @return esp_err_t 
 */
esp_err_t htu21_get_measure_data(uint8_t *data);

/**
 * @brief 
 * 
 * @return temp 
 */
float htu21_get_temp(void);

/**
 * @brief 
 * 
 * @return humid 
 */
float htu21_get_humid(void);


#ifdef __cplusplus
}
#endif

