/**
 * @file gt911.c
 * @brief GT911 Capacitive Touch Panel Controller Driver
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

#include "gt911.h"

static const char *TAG = "gt911";

uint8_t GT911_ADDR[2] = {0x5d, 0x14};

uint8_t CTP_CFG_GT911[184] =  {
    0x61,0xE0,0x01,0xE0,0x01,0x01,0x05,0x00,0x01,0xC8,
    0x28,0x0F,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x18,0x1A,0x1E,0x14,0x85,0x25,0x0A,
    0xEA,0xEC,0xB5,0x06,0x00,0x00,0x00,0x20,0x21,0x11,
    0x00,0x01,0x00,0x0F,0x00,0x2A,0x64,0x32,0x19,0x50,
    0x32,0xDC,0xFA,0x94,0x55,0x02,0x08,0x00,0x00,0x04,
    0x80,0xDE,0x00,0x80,0xE4,0x00,0x80,0xEA,0x00,0x7F,
    0xF0,0x00,0x7F,0xF6,0x00,0x7F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x14,0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,
    0x04,0x02,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x21,0x20,0x1F,
    0x1E,0x1D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,//0x24,0x00,
};

static i2c_bus_device_handle_t gt911_handle = NULL;

esp_err_t gt911_read_bytes(uint16_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_read_reg16(gt911_handle, reg_addr, data_len, data);
}

esp_err_t gt911_write_bytes(uint16_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_write_reg16(gt911_handle, reg_addr, data_len, data);
}

esp_err_t gt911_init(void)
{
    if (NULL != gt911_handle) {
        return ESP_FAIL;
    }

    uint8_t temp[5] = {0};
    bsp_i2c_add_device(&gt911_handle, GT911_ADDR[0]);
    esp_err_t ret = gt911_read_bytes(GT_PID_REG, 4, temp); //read tp driver ic

    if(ESP_OK != ret) 
    {
        bsp_i2c_add_device(&gt911_handle, GT911_ADDR[1]);
        gt911_read_bytes(GT_PID_REG, 4, temp); //read tp driver ic
    }
    temp[4] = 0;
    ESP_LOGI(TAG, "tp driver ic:GT%s", temp);

    ret = ESP_OK;
    temp[0] = 0X02;
    ret |= gt911_write_bytes(GT_CTRL_REG, 1, temp);   //soft reset
    ret |= gt911_read_bytes(GT_CFGS_REG, 1, temp);    //read config version
    ESP_LOGI(TAG, "tp config version:0x%02X", temp[0]);
#if  1    //write
    if (temp[0] < 0X61) // update the config
    {
        temp[0] = 0; temp[1] = 1;
        for (uint8_t i = 0; i < sizeof(CTP_CFG_GT911); i++)
            temp[0] += CTP_CFG_GT911[i]; // chksum
        temp[0] = (~temp[0]) + 1;
        ESP_LOGI(TAG, "run config:%d %02X %02X", sizeof(CTP_CFG_GT911), temp[0], temp[1]);
        gt911_write_bytes(GT_CFGS_REG, sizeof(CTP_CFG_GT911), (uint8_t *)CTP_CFG_GT911); // update the config
        gt911_write_bytes(GT_CHECK_REG, 2, temp);                                        // write config chksum
    }
#else    //read
    uint8_t buf[186] = {0};
    gt911_read_bytes(GT_CFGS_REG, sizeof(buf), (uint8_t *)buf); // read the config
    ESP_LOGI(TAG, "gt911 read register:");
    for(uint8_t i = 0; i < sizeof(buf); i++)
    {
        printf("0x%02X,", buf[i]);
    }
    printf("\n");
#endif
    vTaskDelay(pdMS_TO_TICKS(10));

    if(ret == ESP_OK) ESP_LOGI(TAG, "gt911 init ok");
    else ESP_LOGI(TAG, "gt911 init fail");
    return ret;
}

static esp_err_t gt911_get_touch_points_num(uint8_t *touch_points_num)
{
    uint8_t temp = 0;
    gt911_read_bytes(GT_GSTID_REG, 1, &temp);
    if (temp & 0x80) {
        *touch_points_num = temp & 0x0F;
        // ESP_LOGI(TAG, "reg=0x%X num=%d", temp, *touch_points_num);
        return ESP_OK;
    }
    else {
        *touch_points_num = 0;
        return ESP_FAIL;
    }
}

esp_err_t gt911_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y)
{
    static uint8_t data[4];

    if(ESP_OK == gt911_get_touch_points_num(touch_points_num))
    {
        if (0 == *touch_points_num) {
        } else {
            gt911_read_bytes(GT_TP1_REG, 4, data);
            *x = ((data[1] & 0x0f) << 8) + data[0];
            *y = ((data[3] & 0x0f) << 8) + data[2];
        }

        data[0] = 0;
        gt911_write_bytes(GT_GSTID_REG, 1, data);
    }

    return ESP_OK;
}



