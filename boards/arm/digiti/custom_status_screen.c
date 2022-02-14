/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include <zmk/display/status_screen.h>
#include "widgets/battery_status.h"

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);


#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
static struct zmk_widget_battery_status battery_status_widget;
#endif


lv_style_t global_style;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;

    lv_obj_t *center_frame;

    lv_style_init(&global_style);
    lv_style_set_text_font(&global_style, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_style_set_text_letter_space(&global_style, LV_STATE_DEFAULT, 1);
    lv_style_set_text_line_space(&global_style, LV_STATE_DEFAULT, 1);

    screen = lv_obj_create(NULL, NULL);
    lv_obj_add_style(screen, LV_LABEL_PART_MAIN, &global_style);



    center_frame = lv_cont_create(screen, NULL);
    lv_obj_set_auto_realign(center_frame, true);
    lv_obj_align(center_frame, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_cont_set_fit(center_frame, LV_FIT_TIGHT);
    lv_cont_set_layout(center_frame, LV_LAYOUT_CENTER);

    
#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
    zmk_widget_battery_status_init(&battery_status_widget, center_frame);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
#endif

    return screen;
}
