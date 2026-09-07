#include "head.h"

static bool led_on[4];
static lv_obj_t* led_btn[4];
static lv_obj_t* all_led_btn;

static void Led_Btn_Func(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    int i;
    for (i = 0; i < 4; i++)
    {
        if (btn == led_btn[i])
        {
            led_on[i] = !led_on[i];
            LED_Mode("/dev/led_drv", led_on[i] ? 1 : 0, 7 + i);
            lv_obj_set_style_bg_color(led_btn[i], led_on[i] ? lv_color_hex(0xff0000) : lv_color_hex(0x888888), 0);
        }
    }
}

static void Led_All_Btn_Func(lv_event_t* e)
{
    static bool all_on = false;

    all_on = !all_on;

    LED_ALL(all_on ? 1 : 0);

    int i;

    for (i = 0; i < 4 ; ++i)
    {
        led_on[i] = all_on;
        lv_obj_set_style_bg_color(led_btn[i], all_on ? lv_color_hex(0xff0000) : lv_color_hex(0x888888), 0);
    }
    lv_obj_set_style_bg_color(all_led_btn, all_on ? lv_color_hex(0xff0000) : lv_color_hex(0x888888), 0);
}

void Btn_Show_GUI(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        led_btn[i] = lv_button_create(lv_screen_active());
        lv_obj_set_size(led_btn[i], 80, 80);
        lv_obj_set_style_bg_color(led_btn[i], lv_color_hex(0x888888), 0);
        lv_obj_align(led_btn[i], LV_ALIGN_CENTER, (i - 1) * 90, 0);
        lv_obj_add_event_cb(led_btn[i], Led_Btn_Func, LV_EVENT_CLICKED, NULL);
        led_on[i] = false;
    }

    all_led_btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(all_led_btn, 80, 80);
    lv_obj_set_style_bg_color(all_led_btn, lv_color_hex(0x888888), 0);
    lv_obj_align(all_led_btn, LV_ALIGN_CENTER, 4 * 90, 0);
    lv_obj_add_event_cb(all_led_btn, Led_All_Btn_Func, LV_EVENT_CLICKED, NULL);
}
