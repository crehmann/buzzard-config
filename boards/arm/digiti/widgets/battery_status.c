/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <bluetooth/services/bas.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "battery_status.h"

#include <src/lv_themes/lv_theme.h>
#include <zmk/usb.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

LV_IMG_DECLARE(battery_5_img);
LV_IMG_DECLARE(battery_4_img);
LV_IMG_DECLARE(battery_3_img);
LV_IMG_DECLARE(battery_2_img);
LV_IMG_DECLARE(battery_1_img);
LV_IMG_DECLARE(battery_charging_img);

void set_battery_symbol(lv_obj_t *icon) {
    uint8_t level = bt_bas_get_battery_level();

#if IS_ENABLED(CONFIG_USB)
    if (zmk_usb_is_powered()) {
        lv_img_set_src(icon, &battery_charging_img);
    } else if (level > 80) {
        lv_img_set_src(icon, &battery_5_img);
    } else if (level > 60) {
        lv_img_set_src(icon, &battery_4_img);
    } else if (level > 20) {
        lv_img_set_src(icon, &battery_3_img);
    } else if (level > 20) {
        lv_img_set_src(icon, &battery_2_img);
    } else {
        lv_img_set_src(icon, &battery_1_img);
    }
#endif /* IS_ENABLED(CONFIG_USB) */
}

int zmk_widget_battery_status_init(struct zmk_widget_battery_status *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent, NULL);
    lv_img_set_auto_size(widget->obj, true);
    set_battery_symbol(widget->obj);
    sys_slist_append(&widgets, &widget->node);
    return 0;
}

lv_obj_t *zmk_widget_battery_status_obj(struct zmk_widget_battery_status *widget) {
    return widget->obj;
}

int battery_status_listener(const zmk_event_t *eh) {
    struct zmk_widget_battery_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_symbol(widget->obj); }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(widget_battery_status, battery_status_listener)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB) */

