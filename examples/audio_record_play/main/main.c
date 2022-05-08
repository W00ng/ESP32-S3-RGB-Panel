#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

#include "es8311.h"
#include "bsp_i2s.h"
#include "ui_record.h"

static const char *TAG = "main";

#define AUDIO_SAMPLE_RATE  AUDIO_HAL_16K_SAMPLES
/* i2s_Sample_Rate * T * sizeof(uint8_t)
16bits,Mono，Record_time = T/2(s) */
#define BUF_SIZE  (16000 * 20 * sizeof(uint8_t))  
#define FRAME_SIZE  (1600)

static uint32_t audio_index = 0;
static uint32_t audio_total = 0;
static uint8_t *audio_buffer = NULL;

void audio_record_task(void *args)
{
    static esp_err_t ret;
    static size_t bytes_read = 0;
    static size_t bytes_write = 0;

    while (1) 
    {
        if (mode == record) {
            ESP_LOGI(TAG, "record start");
            audio_index = 0;
            while ((mode == record) && (audio_index < (BUF_SIZE - FRAME_SIZE)))
            {
                ret = i2s_read(I2S_NUM_0, audio_buffer + audio_index, FRAME_SIZE, &bytes_read, 100);
                if (ret != ESP_OK) {
                    ESP_LOGE(TAG, "[echo] i2s read failed");
                    abort();
                }                 
                audio_index += FRAME_SIZE;
            }
            if (mode == record) mode = idle;            
            ESP_LOGI(TAG, "record end");
        }
        else if (mode == play)
        {
            ESP_LOGI(TAG, "play start");
            audio_total = audio_index;  //recorder length
            audio_index = 0;
            while ((mode == play) && (audio_index < audio_total))
            {
                ret = i2s_write(I2S_NUM_0, audio_buffer + audio_index, FRAME_SIZE, &bytes_write, 100);
                if (ret != ESP_OK) {
                    ESP_LOGE(TAG, "[echo] i2s write failed");
                    abort();
                }
                audio_index += FRAME_SIZE;
            }
            if (mode == play) mode = idle;   
            ESP_LOGI(TAG, "play end");
        }
        else
        {
            vTaskDelay((100) / portTICK_PERIOD_MS);  
        }
    }
    vTaskDelete(NULL);
}

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
    // lv_port_fs_init();         
    lv_port_tick_init();

    es8311_codec_config(AUDIO_SAMPLE_RATE);
    es8311_codec_set_voice_volume(80);
    bsp_i2s_init(I2S_NUM_0, 16000);   //config channel_format=mono

    ESP_LOGI(TAG, "init done");

    /* Allocate audio buffer */
    audio_buffer = (uint8_t *) heap_caps_malloc(BUF_SIZE, MALLOC_CAP_SPIRAM); 
    if (NULL == audio_buffer) {
        ESP_LOGE(TAG, "Failed allocate mem for audio buffer.");
        return;
    }

    /* Init UI of example */
    ui_record();

    xTaskCreate(lv_tick_task, "lv_tick_task", 4096, NULL, 1, NULL);
    xTaskCreate(audio_record_task, "audio_record_task", 4096 * 2, NULL, 5, NULL);
}



