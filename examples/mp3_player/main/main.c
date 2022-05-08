#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "bsp_sdcard.h"
#include "ui_audio.h"

static const char *TAG = "main";

void lv_tick_task(void *arg)
{
    while(1) 
    {
        vTaskDelay((10) / portTICK_PERIOD_MS);
        lv_task_handler();        
    }
}

void app_main()
{
    /* Initialize I2C 400KHz */
    ESP_ERROR_CHECK(bsp_i2c_init(I2C_NUM_0, 400000));

    /* LVGL init */
    lv_init();                
    lv_port_disp_init();	  
    lv_port_indev_init();     
    lv_port_fs_init();         
    lv_port_tick_init();

    ESP_LOGI(TAG, "init done");

    ui_audio_start();

    mp3_player_start("/spiffs");         //play mp3 from spiffs

    xTaskCreate(lv_tick_task, "lv_tick_task", 4096, NULL, 1, NULL);
}

