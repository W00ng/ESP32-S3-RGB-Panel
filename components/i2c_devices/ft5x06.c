/**
 * @file ft5x06.c
 * @brief FT5x06 Capacitive Touch Panel Controller Driver
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

#include "ft5x06.h"


static i2c_bus_device_handle_t ft5x06_handle = NULL;

esp_err_t ft5x06_read_byte(uint8_t reg_addr, uint8_t *data)
{
    return i2c_bus_read_byte(ft5x06_handle, reg_addr, data);
}

esp_err_t ft5x06_read_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_read_bytes(ft5x06_handle, reg_addr, data_len, data);
}

static esp_err_t ft5x06_write_byte(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_byte(ft5x06_handle, reg_addr, data);
}

esp_err_t ft5x06_init(void)
{
    esp_err_t ret;

    if (NULL != ft5x06_handle) {
        return ESP_FAIL;
    }

    bsp_i2c_add_device(&ft5x06_handle, FT5x06_ADDR);

    if (NULL == ft5x06_handle) {
        return ESP_FAIL;
    }

#if 0
    // Valid touching detect threshold
    ft5x06_write_byte(FT5x06_ID_G_THGROUP, 70);

    // valid touching peak detect threshold
    ft5x06_write_byte(FT5x06_ID_G_THPEAK, 60);

    // Touch focus threshold
    ft5x06_write_byte(FT5x06_ID_G_THCAL, 16);

    // threshold when there is surface water
    ft5x06_write_byte(FT5x06_ID_G_THWATER, 60);

    // threshold of temperature compensation
    ft5x06_write_byte(FT5x06_ID_G_THTEMP, 10);

    // Touch difference threshold
    ft5x06_write_byte(FT5x06_ID_G_THDIFF, 20);

    // Delay to enter 'Monitor' status (s)
    ft5x06_write_byte(FT5x06_ID_G_TIME_ENTER_MONITOR, 2);

    // Period of 'Active' status (ms)
    ft5x06_write_byte(FT5x06_ID_G_PERIODACTIVE, 12);

    // Timer to enter 'idle' when in 'Monitor' (ms)
    ret = ft5x06_write_byte(FT5x06_ID_G_PERIODMONITOR, 40);

    #else

    static uint8_t data;

    ret = ft5x06_read_byte(FT5x06_ID_G_CIPHER, &data);
    ESP_LOGI("ft5x06", "0x%02X", data);

    #endif

    if(ret == ESP_OK) ESP_LOGI("ft5x06", "ft5x06 init ok");
    else ESP_LOGI("ft5x06", "ft5x06 init fail");

    return ret;    
}

static esp_err_t ft5x06_get_touch_points_num(uint8_t *touch_points_num)
{
    return ft5x06_read_byte(FT5x06_TOUCH_POINTS, touch_points_num);
}

esp_err_t ft5x06_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y)
{
    static uint8_t data[4];

    ft5x06_get_touch_points_num(touch_points_num);

    if (0 == *touch_points_num) {
    } else {
        ft5x06_read_bytes(FT5x06_TOUCH1_XH, 4, data);

        *x = ((data[0] & 0x0f) << 8) + data[1];
        *y = ((data[2] & 0x0f) << 8) + data[3];
    }

    return ESP_OK;
}

esp_err_t fx5x06_read_gesture(ft5x06_gesture_t *gesture)
{
    return ft5x06_read_byte(FT5x06_GESTURE_ID, (uint8_t *)gesture);
}
