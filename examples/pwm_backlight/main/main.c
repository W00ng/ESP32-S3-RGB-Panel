#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

#include "pwm.h"

static const char *TAG = "main";

static lv_obj_t *slider_label;

static void backlight_slider_cb(lv_event_t *event)
{
    static char buf[8];

    lv_obj_t *slider = (lv_obj_t *) event->target;
    int brightness = lv_slider_get_value(slider);
    ESP_ERROR_CHECK(pwm_set_duty(brightness));
    sprintf(buf, "%d%%", brightness);
    lv_label_set_text(slider_label, buf);
}

void ui_init(void)
{
    static lv_obj_t *default_src;
    default_src = lv_scr_act();

    lv_obj_t *backlight_slider = lv_slider_create(default_src);
    lv_obj_set_width(backlight_slider, 250);
    // lv_obj_set_size(backlight_slider, 250, 10);
    lv_obj_set_ext_click_area(backlight_slider, 15);
    lv_slider_set_range(backlight_slider, 0, 100);
    lv_slider_set_value(backlight_slider, 50, LV_ANIM_ON);
    lv_obj_align(backlight_slider, LV_ALIGN_CENTER, 0, 80);
    lv_obj_add_event_cb(backlight_slider, backlight_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_label = lv_label_create(default_src);
    lv_label_set_text(slider_label, "50%");
    lv_obj_set_style_text_font(slider_label, &lv_font_montserrat_28, LV_STATE_DEFAULT);
    lv_obj_align_to(slider_label, backlight_slider, LV_ALIGN_OUT_TOP_MID, 0, -100);
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

    pwm_init();

    ui_init();

    xTaskCreate(lv_tick_task, "lv_tick_task", 4096, NULL, 1, NULL);
}

