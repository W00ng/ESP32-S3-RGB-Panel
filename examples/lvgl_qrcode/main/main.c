#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

static const char *TAG = "main";

void ui_task(void *arg)
{
    static lv_obj_t *default_src;
    default_src = lv_scr_act();

    lv_style_t style2;
    lv_style_set_bg_color(&style2, lv_color_white());
    lv_obj_add_style(default_src, &style2, _LV_STYLE_STATE_CMP_SAME);

    static char *str = "this is test qrcode!";
    lv_obj_t *qrcode = lv_qrcode_create(default_src, 250, lv_color_black(), lv_color_white());
    lv_qrcode_update(qrcode, str, strlen(str));
    lv_obj_align(qrcode, LV_ALIGN_CENTER, 0, 0); 
    // lv_obj_set_pos(qrcode, 160, 30);
    // vTaskDelay((3000) / portTICK_PERIOD_MS);
    // lv_qrcode_delete(qrcode);

    while(1) 
    {
        vTaskDelay((1000) / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void lv_tick_task(void *arg)
{
    while(1) 
    {
        vTaskDelay((20) / portTICK_PERIOD_MS);
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
    // lv_port_fs_init();       
    lv_port_tick_init();

    xTaskCreate(lv_tick_task, "lv_tick_task", 4096, NULL, 1, NULL);
    xTaskCreate(ui_task, "ui_task", 4096*4, NULL, 3, NULL);
}
