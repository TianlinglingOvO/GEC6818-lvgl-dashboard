#include "head.h"

static lv_obj_t * needle_line;    /* 表盘指针 */
static lv_obj_t * scale_line;     /* 表盘 */
static int32_t speed_kmh = 0;     /* 速度 */
static bool accel_held   = false; /* 加速按钮判断 */
static bool brake_held   = false; /* 减速按钮判断 */
static lv_obj_t * accel_btn;      /* 加速按钮 */
static lv_obj_t * brake_btn;      /* 减速按钮 */
static lv_obj_t * accel_lab;      /* 加速按钮标签 */
static lv_obj_t * brake_lab;      /* 减速按钮标签 */

static void Btn_Even_Func(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (btn == accel_btn)
    {
        if (code == LV_EVENT_PRESSED) accel_held = true;
        if (code == LV_EVENT_RELEASED) accel_held = false;
    }

    if (btn == brake_btn)
    {
        if (code == LV_EVENT_PRESSED) brake_held = true;
        if (code == LV_EVENT_RELEASED) brake_held = false;
    }
}

static void set_needle_line_value(void* obj, int32_t v)
{
    /* obj:scale刻度对象； needle_line指针对象； 150:指针长度； v:当前刻度值 */
    lv_scale_set_line_needle_value(obj, needle_line, 150, v);
}

static void speed_timer_cb(lv_timer_t* t)
{
    (void)t;
    if (accel_held) speed_kmh +=2;
    else if (brake_held) speed_kmh -=3;
    else speed_kmh -= 1;

    if (speed_kmh < 0) speed_kmh = 0;
    if (speed_kmh > 260) speed_kmh = 260;

    set_needle_line_value(scale_line, speed_kmh);
}

void Car_Speed_Ometer_Dial_Show_Gui(void)
{
    /* Add git BG */
    jpg_func();
    /* 创建环形刻度标对象，父对象为屏幕 */
    scale_line = lv_scale_create(lv_screen_active());
    /* 设置刻度表大小 宽、高 */
    lv_obj_set_size(scale_line, 350, 350);
    /* 设置刻度标模式：环形， 刻度线向内 */
    lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);

    /* 设置对象背景完全不透明 */
    lv_obj_set_style_bg_opa(scale_line, LV_OPA_COVER, 0);
    /* Black BG */
    lv_obj_set_style_bg_color(scale_line,lv_color_hex(0x000000),0);
    

    /* 设置圆角为圆心 */
    lv_obj_set_style_radius(scale_line, LV_RADIUS_CIRCLE, 0);
    /* 开启边角裁剪，圆形背景防止内存溢出 */
    lv_obj_set_style_clip_corner(scale_line, true, 0);

    /* 对齐方式：左侧垂直居中，X偏移屏幕2%， Y偏移屏幕0% */
    lv_obj_align(scale_line, LV_ALIGN_LEFT_MID, LV_PCT(2), 0);

    /* 显示刻度数字标签 */
    lv_scale_set_label_show(scale_line, true);
    /* 设置总刻度数量 66格 */
    lv_scale_set_total_tick_count(scale_line, 66);
    /* 每5格小刻度显示一个主刻度 */
    lv_scale_set_major_tick_every(scale_line, 5);

    /* LV_PART_ITEMS: 小刻度部分设置小刻度线长度为5像素 */
    lv_obj_set_style_length(scale_line, 5, LV_PART_ITEMS);
    /* LV_PART_INDICATOR：主刻度部分设置主刻度线长度为20像素 */
    lv_obj_set_style_length(scale_line, 20, LV_PART_INDICATOR);


    /* 设置刻度数据范围：最小值0，最大值260 */
    lv_scale_set_range(scale_line, 0, 260);
    /* 设置环形刻度总张开角度 270度 */
    lv_scale_set_angle_range(scale_line, 270);
    /* 设置刻度整体旋转偏移 135度，决定缺口位置 */
    lv_scale_set_rotation(scale_line, 135);

    /* 创建指针线条，父对象时scale_line刻度控件 */
    needle_line = lv_line_create(scale_line);
    /* 设置指针线条宽度6像素 */
    lv_obj_set_style_line_width(needle_line, 6, LV_PART_MAIN);
    /* 开启线条端点圆角 */
    lv_obj_set_style_line_rounded(needle_line, true, LV_PART_MAIN);

    lv_obj_t* speed_label = lv_label_create(scale_line);
    lv_label_set_text(speed_label, "km/h");
    lv_obj_set_style_text_color(speed_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(speed_label,&lv_font_montserrat_20, 0);
    lv_obj_align(speed_label, LV_ALIGN_LEFT_MID, LV_PCT(43), LV_PCT(10));
    
    /* 将仪表盘刻度线整体设置为白色 */
    lv_obj_set_style_line_color(scale_line,lv_color_white(),LV_PART_ITEMS);
    lv_obj_set_style_line_color(scale_line,lv_color_white(),LV_PART_INDICATOR);
    lv_obj_set_style_text_color(scale_line,lv_color_white(),LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(scale_line, lv_color_white(), LV_PART_MAIN);
    
    /* 将指针设置为白色 */
    lv_obj_set_style_line_color(needle_line,lv_color_white(),LV_PART_MAIN);

    /* 再屏幕创建按钮 */
    accel_btn = lv_button_create(lv_screen_active());
    brake_btn = lv_button_create(lv_screen_active());

    /* 设置按钮大小 */
    lv_obj_set_size(accel_btn, 100, 100);
    lv_obj_set_size(brake_btn, 100, 100);

    /* 设置按钮颜色 */
    lv_obj_set_style_bg_color(accel_btn, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_bg_color(brake_btn, lv_color_hex(0x000000), 0);

    /* 设置按钮位置 */
    lv_obj_align(accel_btn, LV_ALIGN_CENTER, 200, -190);
    lv_obj_align(brake_btn, LV_ALIGN_CENTER, 340, -190);
    
    /* 创建标签，父对象->按钮 */
    accel_lab = lv_label_create(accel_btn);
    brake_lab = lv_label_create(brake_btn);

    /* 创建标签文本 */
    lv_label_set_text(accel_lab,"ACC");
    lv_label_set_text(brake_lab,"BRK");

    /* 设置标签文本大小 */
    lv_obj_set_style_text_font(accel_lab, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_font(brake_lab, &lv_font_montserrat_20, 0);

    /*设置标签位置*/
    lv_obj_center(accel_lab);
    lv_obj_center(brake_lab);

    lv_obj_add_event_cb(accel_btn, Btn_Even_Func, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(brake_btn, Btn_Even_Func, LV_EVENT_ALL, NULL);

    lv_timer_create(speed_timer_cb, 50, NULL);
    set_needle_line_value(scale_line, 0);
}
