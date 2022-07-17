/* MCPWM Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "pwm.h"


static const char *TAG = "pwm";


esp_err_t pwm_init(void)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_LCD_BL);

    mcpwm_config_t pwm_config = {
        .frequency = 1000, // frequency
        .cmpr_a = 50,     // duty cycle of PWMxA = 50%
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    ESP_LOGI(TAG, "pwm init freq=%dHz duty=%.1f%%", pwm_config.frequency, pwm_config.cmpr_a);

    return ESP_OK;
}

esp_err_t pwm_set_duty(float duty)
{
    return mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
}


