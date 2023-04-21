/*
 * Copyright (c) 2022 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include "peripheral_status.h"
#include <zmk/event_manager.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/events/split_peripheral_status_changed.h>


LV_IMG_DECLARE(output_ble_con_img);
LV_IMG_DECLARE(output_ble_dis_img);


static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct peripheral_status_state {
    bool connected;
};

static struct peripheral_status_state get_state(const zmk_event_t *_eh) {
    return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()};
}

static void set_status_symbol(lv_obj_t *icon, struct peripheral_status_state state) {
    LOG_DBG("connected? %s", state.connected ? "true" : "false");

    if (state.connected) {
        lv_img_set_src(icon, &output_ble_con_img);
    } else {
        lv_img_set_src(icon, &output_ble_dis_img);
    }

}

static void output_status_update_cb(struct peripheral_status_state state) {
    struct zmk_widget_peripheral_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_status_symbol(widget->obj, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state,
                            output_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);

int zmk_widget_peripheral_status_init(struct zmk_widget_peripheral_status *widget,
                                      lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);

    lv_obj_set_size(widget->obj, 38, 53);

    sys_slist_append(&widgets, &widget->node);

    widget_peripheral_status_init();
    return 0;
}

lv_obj_t *zmk_widget_peripheral_status_obj(struct zmk_widget_peripheral_status *widget) {
    return widget->obj;
}
