/**
 * @file htu21.c
 * @brief HTU21 driver.
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

#include "htu21.h"

static const char *TAG = "htu21";

#define I2C_ACK_CHECK_EN 0x1     /*!< I2C master will check ack from slave*/
#define I2C_ACK_CHECK_DIS 0x0     /*!< I2C master will not check ack from slave */

#define HTU21_DEV_ADDR    (0x40)

#define TEMP_MEASUREMENT_HM       (0xE3)
#define HUMI_MEASUREMENT_HM       (0xE5)
#define TEMP_MEASUREMENT_POLL     (0xF3)
#define HUMI_MEASUREMENT_POLL     (0xF5)
#define USER_REG_WRITE            (0xE6)
#define USER_REG_READ             (0xE7)
#define SOFT_RESET                (0xFE)

static i2c_bus_device_handle_t htu21_handle = NULL;

static esp_err_t htu21_write_byte(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_byte(htu21_handle, reg_addr, data);
}

static esp_err_t htu21_write_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_write_bytes(htu21_handle, reg_addr, data_len, data);
}

static esp_err_t htu21_read_byte(uint8_t reg_addr, uint8_t *data)
{
    return i2c_bus_read_byte(htu21_handle, reg_addr, data);
}

static esp_err_t htu21_read_bytes(uint8_t reg_addr, size_t data_len, uint8_t *data)
{
    return i2c_bus_read_bytes(htu21_handle, reg_addr, data_len, data);
}

esp_err_t htu21_init(void)
{
    if (NULL != htu21_handle) {
        return ESP_FAIL;
    }

    bsp_i2c_add_device(&htu21_handle, HTU21_DEV_ADDR);

    htu21_softreset();
    vTaskDelay(pdMS_TO_TICKS(20));

    esp_err_t ret = htu21_set_mode();

    if (ret != ESP_OK)  ESP_LOGI(TAG, "htu21d init fail");
    else  ESP_LOGI(TAG, "htu21d init ok");
    
    return ESP_OK;
}

esp_err_t htu21_softreset(void)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HTU21_DEV_ADDR << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SOFT_RESET, I2C_ACK_CHECK_EN);
    i2c_master_stop(cmd);

    i2c_bus_cmd_begin(htu21_handle, cmd);
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

esp_err_t htu21_set_mode(void)
{
    uint8_t data;

    htu21_read_byte(USER_REG_READ, &data);

    data = (data & 0x7E) | htu21_res_11bit_11bit;

    return htu21_write_byte(USER_REG_WRITE, data);
}

esp_err_t htu21_start_measure(uint8_t measure_type)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HTU21_DEV_ADDR << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
    i2c_master_write_byte(cmd, measure_type, I2C_ACK_CHECK_EN);
    i2c_master_stop(cmd);

    i2c_bus_cmd_begin(htu21_handle, cmd);
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

esp_err_t htu21_get_measure_data(uint8_t *data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HTU21_DEV_ADDR << 1) | I2C_MASTER_READ, I2C_ACK_CHECK_EN);
    i2c_master_read(cmd, data, 2, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    i2c_bus_cmd_begin(htu21_handle, cmd);
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

float htu21_get_temp(void)
{
    uint8_t data[2];
    htu21_start_measure((uint8_t)TEMP_MEASUREMENT_POLL);
    /* *******************************************
     * You must wait for htu21 convertion done *
     * Temperature convertion time in spec:    *
     *  11 Bit  8 ms                           *
     *  12 Bit  15 ms                          *
     *  13 Bit  29 ms                          *
     *  14 Bit  58 ms                          *
     * *******************************************/
    vTaskDelay(pdMS_TO_TICKS(60));
    
    htu21_get_measure_data(data);

    return (float)(((data[0] << 8) + data [1]) & 0xFFFC) / 65535.0 * 175.72 - 46.85;
}

float htu21_get_humid(void)
{
    uint8_t data[2];
    htu21_start_measure((uint8_t)HUMI_MEASUREMENT_POLL);

    /* *******************************************
     * You must wait for htu21 convertion done *
     * Humidity convertion time in spec:       *
     *  10 Bit  5 ms                           *
     *  11 Bit  9 ms                           *
     *  12 Bit  18 ms                          *
     * *******************************************/
    vTaskDelay(pdMS_TO_TICKS(20));
    
    htu21_get_measure_data(data);

    return (float)(((data[0] << 8) + data [1]) & 0xFFFC) / 65535.0 * 125 - 6;
}



