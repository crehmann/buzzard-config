/*
 * Copyright (c) 2021 Pete Johanson
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once


#include <lvgl.h>
#include <kernel.h>

struct bongo_cat_widget {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_anim_t anim;
};

int bongo_cat_widget_init(struct bongo_cat_widget *widget, lv_obj_t *parent);
lv_obj_t *bongo_cat_widget_obj(struct bongo_cat_widget *widget);