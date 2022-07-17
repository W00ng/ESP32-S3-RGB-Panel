#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "bsp_board.h"
#include "driver/mcpwm.h"


#ifdef __cplusplus
extern "C"
{
#endif


esp_err_t pwm_init(void);

esp_err_t pwm_set_duty(float duty);


#ifdef __cplusplus
}
#endif