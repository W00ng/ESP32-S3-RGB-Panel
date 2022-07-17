/**
 * @file cst826.c
 * @brief CST826 Capacitive Touch Panel Controller Driver
 * @version 0.1
 * @date 2021-01-13
 * 
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0

 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "cst826.h"

static const char *TAG = "cst826";

static i2c_bus_device_handle_t cst826_handle = NULL;

esp_err_t cst826_read_byte(uint8_t reg_addr, uint8_t *data)
{
    return i2c_bus_read_byte(cst826_handle, reg_addr, data);
}

esp_err_t cst826_read_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_read_bytes(cst826_handle, reg_addr, data_len, data);
}

esp_err_t cst826_write_byte(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_byte(cst826_handle, reg_addr, data);
}

esp_err_t cst826_write_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_write_bytes(cst826_handle, reg_addr, data_len, data);
}

esp_err_t cst826_init(void)
{
    if (NULL != cst826_handle) {
        return ESP_FAIL;
    }

    bsp_i2c_add_device(&cst826_handle, CST826_ADDR);
#if 1
    uint8_t temp = 0;
    esp_err_t ret = cst826_read_byte(CST826_CHIP_ID, &temp);
#else
    static uint8_t data[8];
    esp_err_t ret = cst826_read_bytes(CST826_CHIP_ID, 2, data);
    data[2] = 0;
    ESP_LOGI(TAG, "cst826 read reg");
    for(size_t i = 0; i < 2; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\r\n");
#endif
    if(ret == ESP_OK) ESP_LOGI(TAG, "cst826 init ok");
    else ESP_LOGI(TAG, "cst826 init fail");
    return ret;
}

esp_err_t cst826_get_touch_points_num(uint8_t *touch_points_num)
{
    esp_err_t ret = ESP_OK;
    uint8_t temp = 0;

    ret = cst826_read_byte(CST826_TP_NUM_REG, &temp);
    *touch_points_num = temp;
    // ESP_LOGI(TAG, "num=%d", *touch_points_num);
    return ret;
}

esp_err_t cst826_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y)
{
    static uint8_t data[4];

    cst826_get_touch_points_num(touch_points_num);
    if (*touch_points_num > 0) {
        cst826_read_bytes(CST826_TP1_POS_REG, 4, data);
        *x = ((data[0] & 0x0f) << 8) + data[1];
        *y = (((data[2] & 0x0f) << 8) + data[3]);
        return ESP_OK;  
    }
    else {
        return ESP_FAIL;
    }
}


