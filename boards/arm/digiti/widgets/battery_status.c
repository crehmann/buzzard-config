/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/usb.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct battery_status_state {
    uint8_t level;
#if IS_ENABLED(CONFIG_USB)
    bool usb_present;
#endif
};

LV_IMG_DECLARE(battery_5_img);
LV_IMG_DECLARE(battery_4_img);
LV_IMG_DECLARE(battery_3_img);
LV_IMG_DECLARE(battery_2_img);
LV_IMG_DECLARE(battery_1_img);
LV_IMG_DECLARE(battery_charging_img);

void set_battery_symbol(lv_obj_t *icon, struct battery_status_state state) {
    uint8_t level = state.level;

    LOG_DBG("set_battery_symbol level: %i", level);
#if IS_ENABLED(CONFIG_USB)
    if (state.usb_present) {
        lv_img_set_src(icon, &battery_charging_img);
        return;
    } 
#endif /* IS_ENABLED(CONFIG_USB) */
    
    if (level > 80) {
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
}

void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_battery_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_symbol(widget->obj, state); }
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    return (struct battery_status_state) {
        .level = bt_bas_get_battery_level(),
#if IS_ENABLED(CONFIG_USB)
        .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB) */
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state)

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB)
    ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB) */

int zmk_widget_battery_status_init(struct zmk_widget_battery_status *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);
    lv_obj_set_size(widget->obj, 25, 40);
    sys_slist_append(&widgets, &widget->node);
    widget_battery_status_init();
    return 0;
}

lv_obj_t *zmk_widget_battery_status_obj(struct zmk_widget_battery_status *widget) {
    return widget->obj;
}
