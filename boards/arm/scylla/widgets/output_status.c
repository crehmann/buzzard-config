/*
 * Copyright (c) 2020 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <bluetooth/services/bas.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "output_status.h"
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
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
LV_IMG_DECLARE(output_ble_adv_img);
LV_IMG_DECLARE(output_ble_con_img);
LV_IMG_DECLARE(output_ble_dis_img);


static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

void set_status_symbol(lv_obj_t *icon) {
    bool active_profile_connected = zmk_ble_active_profile_is_connected();
    bool active_profie_bonded = !zmk_ble_active_profile_is_open();

#if defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    uint8_t active_profile_index = zmk_ble_active_profile_index();
    enum zmk_endpoint selected_endpoint = zmk_endpoints_selected();

    switch (selected_endpoint) {
    case ZMK_ENDPOINT_USB:
        lv_img_set_src(icon, &output_usb_img);
        break;
    case ZMK_ENDPOINT_BLE:
        if (active_profie_bonded) {
            if (active_profile_connected) {
                switch (active_profile_index) {
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
                switch (active_profile_index) {
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
            switch (active_profile_index) {
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
#endif
#if !defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    if (active_profie_bonded) {
        if (active_profile_connected) {
            lv_img_set_src(icon, &output_ble_con_img);
        } else {
            lv_img_set_src(icon, &output_ble_dis_img);
        }
    } else {
        lv_img_set_src(icon, &output_ble_adv_img);
    }
#endif
}

int zmk_widget_output_status_init(struct zmk_widget_output_status *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent, NULL);
    lv_obj_set_size(widget->obj, 38, 53);
    set_status_symbol(widget->obj);
    sys_slist_append(&widgets, &widget->node);
    return 0;
}

lv_obj_t *zmk_widget_output_status_obj(struct zmk_widget_output_status *widget) {
    return widget->obj;
}

int output_status_listener(const zmk_event_t *eh) {
    struct zmk_widget_output_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_status_symbol(widget->obj); }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(widget_output_status, output_status_listener)
#if defined(CONFIG_USB)
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif