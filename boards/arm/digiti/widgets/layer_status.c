/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "layer_status.h"
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

LV_IMG_DECLARE(layer_adj_img);
LV_IMG_DECLARE(layer_base_img);
LV_IMG_DECLARE(layer_fun_img);
LV_IMG_DECLARE(layer_num_img);
LV_IMG_DECLARE(layer_sym_img);
LV_IMG_DECLARE(layer_key_img);

void set_layer_symbol(lv_obj_t *icon) {
    int active_layer_index = zmk_keymap_highest_layer_active();
    const char *layer_label = zmk_keymap_layer_label(active_layer_index);

    // LOG_DBG("Layer changed to %i", active_layer_index);

    if (layer_label == NULL) {
        lv_img_set_src(icon, &layer_key_img);
    } else if (strcmp(layer_label, "adj") == 0) {
        lv_img_set_src(icon, &layer_adj_img);
    } else if (strcmp(layer_label, "base") == 0) {
        lv_img_set_src(icon, &layer_adj_img);
    } else if (strcmp(layer_label, "fun") == 0) {
        lv_img_set_src(icon, &layer_fun_img);
    } else if (strcmp(layer_label, "num") == 0) {
        lv_img_set_src(icon, &layer_num_img);
    } else if (strcmp(layer_label, "sym") == 0) {
        lv_img_set_src(icon, &layer_sym_img);
    } else {
        lv_img_set_src(icon, &layer_key_img);
    }
}

int zmk_widget_layer_status_init(struct zmk_widget_layer_status *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent, NULL);
    lv_img_set_auto_size(widget->obj, true);
    set_layer_symbol(widget->obj);
    sys_slist_append(&widgets, &widget->node);
    return 0;
}

lv_obj_t *zmk_widget_layer_status_obj(struct zmk_widget_layer_status *widget) {
    return widget->obj;
}

int layer_status_listener(const zmk_event_t *eh) {
    struct zmk_widget_layer_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_layer_symbol(widget->obj); }
    return 0;
}

ZMK_LISTENER(widget_layer_status, layer_status_listener)
ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);