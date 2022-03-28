/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include <zmk/display/status_screen.h>
#include "widgets/battery_status.h"
#include "widgets/layer_status.h"
#include "widgets/output_status.h"

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);


#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
static struct zmk_widget_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_LAYER_STATUS)
static struct zmk_widget_layer_status layer_status_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_OUTPUT_STATUS)
static struct zmk_widget_output_status output_status_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BONGO_CAT)
#include "widgets/bongo_cat_widget.h"
static struct zmk_widget_bongo_cat bongo_widget;
#endif


lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;



    screen = lv_obj_create(NULL, NULL);

    
#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
    LOG_DBG("Battery");
    zmk_widget_battery_status_init(&battery_status_widget, screen);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), NULL, LV_ALIGN_IN_TOP_LEFT, 14, 8);
#endif


#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_OUTPUT_STATUS)
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), NULL, LV_ALIGN_IN_TOP_RIGHT, -8, 8);
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_LAYER_STATUS)
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -8);
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BONGO_CAT)
    LOG_DBG("Bongo");
    zmk_widget_bongo_cat_init(&bongo_widget, screen);
    lv_obj_align(zmk_widget_bongo_cat_obj(&bongo_widget), NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
#endif

    return screen;
}
