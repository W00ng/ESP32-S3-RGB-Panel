/**
 * @file ui_record.c
 * @brief Audio record example ui
 * @version 0.1
 * @date 2021-03-04
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

#include "ui_record.h"

record_mode_t mode = idle;

#define COLOR_BAR   lv_color_make(86, 94, 102)
#define COLOR_THEME lv_color_make(252, 199, 0)
#define COLOR_DEEP  lv_color_make(246, 174, 61)
#define COLOR_TEXT  lv_color_make(56, 56, 56)
#define COLOR_BG    lv_color_make(238, 241, 245)

static lv_obj_t *slider_label;

static void btn_cb(lv_event_t *event);
static void volume_slider_cb(lv_event_t *event);

void ui_record(void)
{
    static lv_obj_t *default_src;
    default_src = lv_scr_act();		

    lv_obj_t *btn = lv_btn_create(default_src);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_set_style_radius(btn, 50, LV_STATE_DEFAULT);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -100);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btn, btn_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *label = lv_label_create(default_src);
    lv_label_set_text(label, "Press to Record & Play");
    lv_obj_set_style_text_font(label,   &lv_font_montserrat_18, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label,  lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    lv_obj_align_to(label, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);

    lv_obj_t *volume_slider = lv_slider_create(default_src);
    lv_obj_set_width(volume_slider, 200);
    // lv_obj_set_size(volume_slider, 250, 10);
    lv_obj_set_ext_click_area(volume_slider, 15);
    lv_slider_set_range(volume_slider, 0, 90);
    lv_slider_set_value(volume_slider, 80, LV_ANIM_ON);
    lv_obj_align(volume_slider, LV_ALIGN_CENTER, 0, 80);
    lv_obj_add_event_cb(volume_slider, volume_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_label = lv_label_create(default_src);
    lv_label_set_text(slider_label, "80%");
    lv_obj_set_style_text_font(slider_label, &lv_font_montserrat_16, LV_STATE_DEFAULT);
    lv_obj_align_to(slider_label, volume_slider, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    // lv_port_sem_give();
}

static void btn_cb(lv_event_t *event)
{
    lv_obj_t *btn = (lv_obj_t *) event->target;

    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        // audio_record_start();
        // ESP_LOGI("main", "audio_record_start");
        mode = record;
    } else {
        // audio_play_start();
        // ESP_LOGI("main", "audio_play_start");
        mode = play;
    }
}

static void volume_slider_cb(lv_event_t *event)
{
    static char buf[8];

    lv_obj_t *slider = (lv_obj_t *) event->target;
    int volume = lv_slider_get_value(slider);
    ESP_ERROR_CHECK(es8311_codec_set_voice_volume(volume));
    sprintf(buf, "%d%%", volume);
    lv_label_set_text(slider_label, buf);
}
