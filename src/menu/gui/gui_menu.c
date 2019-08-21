/*  
 * Copyright (c) 2018 Guillem96
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"
#include "menu/gui/gui_menu_controllers.h"

#include "power/battery.h"

#include "core/custom-gui.h"

#include "utils/util.h"
#include "gfx/gfx.h"

#include <string.h>

#define MINOR_VERSION 3
#define MAJOR_VERSION 0

// static void gui_menu_draw_background(gui_menu_t* menu)
// {
//     if(!render_custom_background(menu->custom_gui))
//         gfx_clear_color(&g_gfx_ctxt, 0xFF191414);

//     /* Render title */
//     if (!render_custom_title(menu->custom_gui))
//     {
//         g_gfx_con.scale = 4;
//         gfx_con_setpos(&g_gfx_con, 480, 20);
//         gfx_printf(&g_gfx_con, "ArgonNX v%d.%d", MAJOR_VERSION, MINOR_VERSION);
//     }
// }

// static u32 get_battery_color(battery_status_t battery_status)
// {
//     if (battery_status.charge_status == FAST_CHARGING) {
//         return 0xFF6ED0F4;
//     } else if (battery_status.charge_status == CHARGING) {
//         return 0xFF74CC78;
//     } else if (battery_status.percent > 50) {
//         return 0xFF4EAD55;
//     } else if (battery_status.percent < 50) {
//         return 0xFFF4C153;
//     } else {
//         return 0xFFFF7C7C;
//     }
// }

// static void gui_menu_draw_battery()
// {
//     battery_status_t battery_status;
//     battery_get_status(&battery_status);
//     u32 color = get_battery_color(battery_status);

//     u32 x = 30;
//     u32 y = 1130;
//     u32 padding = 6;
//     u32 battery_width = 70;
//     u32 battery_height = 30;

//     gfx_draw_color_rect(&g_gfx_ctxt, 0xFFFFFFFF, x - padding / (float)2, y - padding / (float)2,
//                             battery_height + padding / (float)2,
//                             battery_width + padding / (float)2);
//     gfx_draw_color_rect(&g_gfx_ctxt, 0xFFFFFFFF,
//                             x + battery_height / (float)2 - 6,
//                             y - padding - 3,
//                             10, 10);

//     u32 y_offset = battery_width - (battery_width * battery_status.percent / (float)100);
//     gfx_draw_color_rect(&g_gfx_ctxt, color, x, y + y_offset,
//                             battery_height - padding / (float)2 ,
//                             (battery_width * battery_status.percent / (float)100) - padding / (float)2);

//     g_gfx_con.scale = 2;
//     gfx_con_setpos(&g_gfx_con, 20, 35);
//     gfx_printf(&g_gfx_con, "%d%%", battery_status.percent);
// }

/* Render functions */
static void render_tabs(argon_ctxt_t*);
static void render_payloads_entries(lv_obj_t*, argon_ctxt_t*);

void gui_menu_draw(argon_ctxt_t *argon_ctxt)
{
    render_tabs(argon_ctxt);

}

void gui_menu_open(argon_ctxt_t *argon_ctxt)
{
    while (true)
    {
        lv_tick_inc(1);
        lv_task_handler();
        msleep(1);
    }
}

void gui_menu_destroy(argon_ctxt_t *argon_ctxt)
{
    argon_ctxt_destroy(argon_ctxt);
}

static void render_tabs(argon_ctxt_t* argon_ctxt) {
    custom_gui_t* cg = custom_gui_load();


    lv_obj_t *base_tabs = lv_tabview_create(lv_scr_act(), NULL);
    lv_tabview_set_btns_pos(base_tabs, LV_TABVIEW_BTNS_POS_BOTTOM);
    
    /* Disable animations */
    lv_tabview_set_anim_time(base_tabs, 0);
    lv_tabview_set_sliding(base_tabs, false);

    lv_obj_t* payloads_tab = lv_tabview_add_tab(base_tabs, 
                                                LV_SYMBOL_DIRECTORY" Payloads");
    lv_obj_t* settings_tab = lv_tabview_add_tab(base_tabs, 
                                                LV_SYMBOL_SETTINGS" Settings");
    
    if(!render_custom_background(cg, payloads_tab)) 
    {
        gfx_printf("\nFail drawing bg\n");
        return;
    }

    render_payloads_entries(payloads_tab, argon_ctxt);

    /* Push created items to the pool */
    gui_menu_pool_push(argon_ctxt->pool, payloads_tab);
    gui_menu_pool_push(argon_ctxt->pool, settings_tab);
}

static void render_payloads_entries(lv_obj_t* par_tabview, argon_ctxt_t* argon_ctxt)
{
    lv_obj_t *btn ;
    lv_obj_t *label;

    for (u32 i = 0; i < 5; i++) {
        btn = lv_btn_create(par_tabview, NULL);
        lv_obj_set_size(btn, 350, 350);
        lv_obj_set_pos(btn, i * 400 + 100, 200);
        lv_obj_set_event_cb(btn, ctrl_reboot_rcm);
        label = lv_label_create(btn, NULL);
        lv_label_set_text(label, LV_SYMBOL_PLAY);
        gui_menu_pool_push(argon_ctxt->pool, label);
        gui_menu_pool_push(argon_ctxt->pool, btn);   
    }
}
