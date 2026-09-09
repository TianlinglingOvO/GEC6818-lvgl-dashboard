#include "head.h"

typedef struct
{
    lv_obj_t * obj; // obj
    bool on;        // status
    /* image src */
    const char * img_off;
    const char * img_on;
    int led;        // LED number(1~4)
} dash_icon_t;

static dash_icon_t turn_right = {
    .obj = NULL,
    .on = false,
    .img_off = "A:turn_right_off1.png",
    .img_on = "A:turn_right_on1.png",
    .led = 2
};


static void dash_icon_apply(dash_icon_t* ic)
{
    const char* src = ic->on ? ic->img_on : ic->img_off;
    if (src == NULL)
    {
        lv_obj_add_flag(ic->obj, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_remove_flag(ic->obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_image_src(ic->obj, src, 0);

    switch(ic->led) {
        case 1: LED_One(ic->on); break;
        case 2: LED_Two(ic->on); break;
        case 3: LED_Thr(ic->on); break;
        case 4: LED_Four(ic->on); break;
        default: break;
    }
}

static void dash_icon_click(lv_event_t* e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    dash_icon_t* ic = lv_event_get_user_data(e);
    ic->on = !ic->on;
    dash_icon_apply(ic);
}

void dash_icon_create(lv_obj_t* parent, dash_icon_t* ic, int x, int y)
{
    ic->obj = lv_obj_create(parent);
    lv_obj_set_size(ic->obj, 50, 50);
    lv_obj_set_style_bg_opa(ic->obj, LV_OPA_TRANSP, NULL);
    lv_obj_set_style_border_width(ic->obj, 0, NULL);
    lv_obj_set_style_shadow_width(ic->obj, 0, NULL);
    lv_obj_align(ic->obj, LV_ALIGN_CENTER, x, y);
    lv_obj_add_event_cb(ic->obj, dash_icon_click, LV_EVENT_CLICKED, ic);
    dash_icon_apply(ic);
}

void Dash_Icon_Show_UI(void)
{
    dash_icon_create(lv_screen_active(),&turn_right, 50, 50);
}
