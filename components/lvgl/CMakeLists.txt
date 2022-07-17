cmake_minimum_required(VERSION 3.12.4)

# if(NOT ESP_PLATFORM)
#   project(lvgl HOMEPAGE_URL https://github.com/lvgl/lvgl)
# endif()

# set(LVGL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# if(ESP_PLATFORM)
#   include(${CMAKE_CURRENT_LIST_DIR}/cmake/esp.cmake)
# elseif(ZEPHYR_BASE)
#   include(${CMAKE_CURRENT_LIST_DIR}/cmake/zephyr.cmake)
# elseif(MICROPY_DIR)
#   include(${CMAKE_CURRENT_LIST_DIR}/cmake/micropython.cmake)
# else()
#   include(${CMAKE_CURRENT_LIST_DIR}/cmake/custom.cmake)
# endif()

idf_component_register(
    SRC_DIRS
        "src/core"
        "src/draw"
        "src/extra"
        "src/font"
        "src/gpu"
        "src/hal"
        "src/misc"
        "src/widgets"
        "src/extra/layouts/"
        "src/extra/layouts/flex"
        "src/extra/layouts/grid"
        "src/extra/libs/png"
        "src/extra/libs/fsdrv"
        "src/extra/libs/qrcode"
        "src/extra/themes"
        "src/extra/themes/basic"
        "src/extra/themes/default"
        "src/extra/themes/mono"
        "src/extra/widgets/animimg"
        "src/extra/widgets/calendar"
        "src/extra/widgets/chart"
        "src/extra/widgets/colorwheel"
        "src/extra/widgets/imgbtn"
        "src/extra/widgets/keyboard"
        "src/extra/widgets/led"
        "src/extra/widgets/list"
        "src/extra/widgets/menu"
        "src/extra/widgets/meter"
        "src/extra/widgets/msgbox"
        "src/extra/widgets/span"
        "src/extra/widgets/spinbox"
        "src/extra/widgets/spinner"
        "src/extra/widgets/tabview"
        "src/extra/widgets/tileview"
        "src/extra/widgets/win"
        "src/draw/sw"
        "porting"
        "demos"
        "demos/benchmark"
        "demos/benchmark/assets"
        "demos/keypad_encoder"
        "demos/music"
        "demos/music/assets"
        "demos/stress"
        "demos/stress/assets"
        "demos/widgets"
        "demos/widgets/assets"

    INCLUDE_DIRS
        "."
        "src/core"
        "src/draw"
        "src/extra"
        "src/font"
        "src/gpu"
        "src/hal"
        "src/misc"
        "src/widgets"
        "src/extra/layouts/"
        "src/extra/layouts/flex"
        "src/extra/layouts/grid"
        "src/extra/themes"
        "src/extra/themes/basic"
        "src/extra/themes/default"
        "src/extra/themes/mono"
        "src/extra/widgets/animimg"
        "src/extra/widgets/calendar"
        "src/extra/widgets/chart"
        "src/extra/widgets/colorwheel"
        "src/extra/widgets/imgbtn"
        "src/extra/widgets/keyboard"
        "src/extra/widgets/led"
        "src/extra/widgets/list"
        "src/extra/widgets/menu"
        "src/extra/widgets/meter"
        "src/extra/widgets/msgbox"
        "src/extra/widgets/span"
        "src/extra/widgets/spinbox"
        "src/extra/widgets/spinner"
        "src/extra/widgets/tabview"
        "src/extra/widgets/tileview"
        "src/extra/widgets/win"
        "src/draw/sw"
        "porting"
        "demos"
        "demos/benchmark"
        "demos/benchmark/assets"
        "demos/keypad_encoder"
        "demos/music"
        "demos/music/assets"
        "demos/stress"
        "demos/stress/assets"
        "demos/widgets"
        "demos/widgets/assets"
        
    REQUIRES
        bsp
        driver
        spiffs
        esp_common 
        log
        esp_timer
        )

        

