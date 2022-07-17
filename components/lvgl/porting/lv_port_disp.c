/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "../../lvgl.h"

/*********************
 *      DEFINES
 *********************/
static const char *TAG = "lv_port_disp";

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
static bool notify_lvgl_flush_ready(esp_lcd_panel_handle_t panel, esp_lcd_rgb_panel_event_data_t *event_data, void *user_data);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_port_disp_init(void)
{
    static lv_disp_drv_t disp_drv;      // contains callback functions

    st7701_reg_init();    // st7701 register config

    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << GPIO_LCD_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ESP_LOGI(TAG, "Install RGB panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_rgb_panel_config_t panel_config = {
        .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
        .psram_trans_align = 64,
        .clk_src = LCD_CLK_SRC_PLL160M,
        .disp_gpio_num = GPIO_NUM_NC,
        .pclk_gpio_num = GPIO_LCD_PCLK,
        .vsync_gpio_num = GPIO_LCD_VSYNC,
        .hsync_gpio_num = GPIO_LCD_HSYNC,
        .de_gpio_num = GPIO_LCD_DE,
        .data_gpio_nums = {
            GPIO_LCD_B0, GPIO_LCD_B1, GPIO_LCD_B2, GPIO_LCD_B3, GPIO_LCD_B4,         
            GPIO_LCD_G0, GPIO_LCD_G1, GPIO_LCD_G2, GPIO_LCD_G3, GPIO_LCD_G4, GPIO_LCD_G5,
            GPIO_LCD_R0, GPIO_LCD_R1, GPIO_LCD_R2, GPIO_LCD_R3, GPIO_LCD_R4,
        },
        .timings = {
            .pclk_hz = 10 * 1000 * 1000,
            .h_res = LCD_WIDTH,
            .v_res = LCD_HEIGHT,
            // The following parameters should refer to LCD spec
            .hsync_back_porch = 50,
            .hsync_front_porch = 10,
            .hsync_pulse_width = 8,
            .vsync_back_porch = 20,
            .vsync_front_porch = 10,
            .vsync_pulse_width = 8,
            .flags.pclk_active_neg = 0, // RGB data is clocked out on falling edge
        },
        .flags.fb_in_psram = 1, // allocate frame buffer in PSRAM
        .on_frame_trans_done = notify_lvgl_flush_ready,
        .user_ctx = &disp_drv,
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_set_level(GPIO_LCD_BL, 1);

    /* initialize LVGL draw buffers */
    static lv_disp_draw_buf_t draw_buf; 
    lv_color_t *buf1 = heap_caps_malloc(LCD_WIDTH * 120 * sizeof(lv_color_t), MALLOC_CAP_DMA);   /* MALLOC_CAP_SPIRAM  MALLOC_CAP_DMA  MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT */
    assert(buf1);
#if 1
    lv_color_t *buf2 = heap_caps_malloc(LCD_WIDTH * 120 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LCD_WIDTH * 120);
#else                       
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LCD_WIDTH * 120);   /*Initialize the display buffer*/
#endif

    /* Register the display in LVGL */
    lv_disp_drv_init(&disp_drv);   

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = lvgl_flush_cb;
    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf;

    disp_drv.user_data = panel_handle;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
}

/**
 * @brief Tell LVGL that LCD flush done.
 * 
 * @return true Call `portYIELD_FROM_ISR()` after esp-lcd ISR return.
 * @return false Do nothing after esp-lcd ISR return.v
 */
static bool notify_lvgl_flush_ready(esp_lcd_panel_handle_t panel, esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_data;
    lv_disp_flush_ready(disp_driver);
    return false;
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}

/**
 * @brief Task to generate ticks for LVGL.
 * 
 * @param pvParam Not used. 
 */
static void lv_tick_inc_cb(void *data)
{
    uint32_t tick_inc_period_ms = *((uint32_t *) data);

    lv_tick_inc(tick_inc_period_ms);
}

/**
 * @brief Create tick task for LVGL.
 * 
 * @return esp_err_t 
 */
esp_err_t lv_port_tick_init(void)
{
    static const uint32_t tick_inc_period_ms = 5;
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = lv_tick_inc_cb,
            .arg = &tick_inc_period_ms,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "",     /* name is optional, but may help identify the timer when debugging */
            .skip_unhandled_events = true,
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet. Start the timer now */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, tick_inc_period_ms * 1000));

    return ESP_OK;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
