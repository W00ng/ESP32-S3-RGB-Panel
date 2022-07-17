/**
 * @file ui_audio.h
 * @brief 
 * @version 0.1
 * @date 2021-11-11
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

#pragma once

#include "esp_err.h"
#include "esp_log.h"

#include "lvgl.h"
#include "audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create audio player UI
 * 
 */
void ui_audio_start(void);

#ifdef __cplusplus
}
#endif
