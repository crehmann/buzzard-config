/*
 * Copyright (c) 2020 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include "output_status.h"
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>
#include <zmk/usb.h>
#include <zmk/ble.h>
#include <zmk/endpoints.h>

LV_IMG_DECLARE(output_ble_adv_1_img);
LV_IMG_DECLARE(output_ble_adv_2_img);
LV_IMG_DECLARE(output_ble_adv_3_img);
LV_IMG_DECLARE(output_ble_adv_4_img);
LV_IMG_DECLARE(output_ble_adv_5_img);
LV_IMG_DECLARE(output_ble_con_1_img);
LV_IMG_DECLARE(output_ble_con_2_img);
LV_IMG_DECLARE(output_ble_con_3_img);
LV_IMG_DECLARE(output_ble_con_4_img);
LV_IMG_DECLARE(output_ble_con_5_img);
LV_IMG_DECLARE(output_ble_dis_1_img);
LV_IMG_DECLARE(output_ble_dis_2_img);
LV_IMG_DECLARE(output_ble_dis_3_img);
LV_IMG_DECLARE(output_ble_dis_4_img);
LV_IMG_DECLARE(output_ble_dis_5_img);
LV_IMG_DECLARE(output_usb_img);


static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct output_status_state {
    enum zmk_endpoint selected_endpoint;
    bool active_profile_connected;
    bool active_profile_bonded;
    uint8_t active_profile_index;
};

static struct output_status_state get_state(const zmk_event_t *_eh) {
    return (struct output_status_state){.selected_endpoint = zmk_endpoints_selected(),
                                        .active_profile_connected =
                                            zmk_ble_active_profile_is_connected(),
                                        .active_profile_bonded = !zmk_ble_active_profile_is_open(),
                                        .active_profile_index = zmk_ble_active_profile_index()};
    ;
}

void set_status_symbol(lv_obj_t *icon, struct output_status_state state) {
    switch (state.selected_endpoint) {
    case ZMK_ENDPOINT_USB:
        lv_img_set_src(icon, &output_usb_img);
        break;
    case ZMK_ENDPOINT_BLE:
        if (state.active_profile_bonded) {
            if (state.active_profile_connected) {
                switch (state.active_profile_index) {
                case 0:
                    lv_img_set_src(icon, &output_ble_con_1_img);
                    break;
                case 1:
                    lv_img_set_src(icon, &output_ble_con_2_img);
                    break;
                case 2:
                    lv_img_set_src(icon, &output_ble_con_3_img);
                    break;
                case 3:
                    lv_img_set_src(icon, &output_ble_con_4_img);
                    break;
                case 4:
                    lv_img_set_src(icon, &output_ble_con_5_img);
                    break;
                }
            } else {
                switch (state.active_profile_index) {
                case 0:
                    lv_img_set_src(icon, &output_ble_dis_1_img);
                    break;
                case 1:
                    lv_img_set_src(icon, &output_ble_dis_2_img);
                    break;
                case 2:
                    lv_img_set_src(icon, &output_ble_dis_3_img);
                    break;
                case 3:
                    lv_img_set_src(icon, &output_ble_dis_4_img);
                    break;
                case 4:
                    lv_img_set_src(icon, &output_ble_dis_5_img);
                    break;
                }
            }
        } else {
            switch (state.active_profile_index) {
                case 0:
                    lv_img_set_src(icon, &output_ble_adv_1_img);
                    break;
                case 1:
                    lv_img_set_src(icon, &output_ble_adv_2_img);
                    break;
                case 2:
                    lv_img_set_src(icon, &output_ble_adv_3_img);
                    break;
                case 3:
                    lv_img_set_src(icon, &output_ble_adv_4_img);
                    break;
                case 4:
                    lv_img_set_src(icon, &output_ble_adv_5_img);
                    break;
            }    
        }
        break;
    }
}

static void output_status_update_cb(struct output_status_state state) {
    struct zmk_widget_output_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_status_symbol(widget->obj, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_selection_changed);

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif

int zmk_widget_output_status_init(struct zmk_widget_output_status *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);

    lv_obj_set_size(widget->obj, 52, 38);

    sys_slist_append(&widgets, &widget->node);

    widget_output_status_init();
    return 0;
}

lv_obj_t *zmk_widget_output_status_obj(struct zmk_widget_output_status *widget) {
    return widget->obj;
}
