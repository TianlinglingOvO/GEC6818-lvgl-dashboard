#include "head.h"

static lv_obj_t * needle_line;    /* 速度表指针 */
static lv_obj_t * scale_line;     /* 速度表 */
static lv_obj_t * tach_needle;    /* 转速表指针 */
static lv_obj_t * tach_scale;     /* 转速表 */
static int32_t speed_kmh = 0;     /* 速度 */



static void set_needle_line_value(void* obj, int32_t v)
{
    /* obj:scale刻度对象； needle_line指针对象； 150:指针长度； v:当前刻度值 */
    lv_scale_set_line_needle_value(obj, needle_line, 150, v);
}

void Car_Speed_Ometer_Dial_Show_Gui(void)
{
    /* 创建环形刻度标对象，父对象为屏幕 */
    scale_line = lv_scale_create(lv_screen_active());
    /* 设置刻度表大小 宽、高 */
    lv_obj_set_size(scale_line, 300, 300);
    /* 设置刻度标模式：环形， 刻度线向内 */
    lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);

    /* 设置对象背景完全不透明 */
    // lv_obj_set_style_bg_opa(scale_line, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(scale_line, LV_OPA_TRANSP, 0);
    // 防止刻度尺被图片覆盖
    lv_scale_set_post_draw(scale_line, true);
    /* Black BG */
    lv_obj_set_style_bg_color(scale_line,lv_color_hex(0x000000),0);
    /* My bg photo */
    lv_obj_t* speed_bg = lv_image_create(scale_line);
    lv_image_set_src(speed_bg, "A:speed_bg_test.jpg");
    lv_obj_set_size(speed_bg, 300, 300);
    lv_image_set_inner_align(speed_bg, LV_IMAGE_ALIGN_STRETCH);
    lv_obj_align(speed_bg,LV_ALIGN_CENTER, 0, 0);
    lv_obj_move_to_index(speed_bg, 0);
    

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


    set_needle_line_value(scale_line, 0);
}

void Car_tachometer_Show_Gui(void)
{
    /* 创建环形刻度标对象，父对象为屏幕 */
    tach_scale = lv_scale_create(lv_screen_active());
    /* 设置刻度表大小 宽、高 */
    lv_obj_set_size(tach_scale, 300, 300);
    /* 设置刻度标模式：环形， 刻度线向内 */
    lv_scale_set_mode(tach_scale, LV_SCALE_MODE_ROUND_INNER);

    /* 设置对象背景完全不透明 */
    // lv_obj_set_style_bg_opa(tach_scale, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(tach_scale, LV_OPA_TRANSP, 0);
    lv_scale_set_post_draw(tach_scale, true);
    /* Black BG */
    lv_obj_set_style_bg_color(tach_scale, lv_color_hex(0x000000), 0);
    /* My bg photo */
    lv_obj_t* tach_bg = lv_image_create(tach_scale);
    lv_image_set_src(tach_bg, "A:tachometer_bg_test.jpg");
    lv_obj_set_size(tach_bg, 300, 300);
    lv_image_set_inner_align(tach_bg, LV_IMAGE_ALIGN_STRETCH);
    lv_obj_align(tach_bg,LV_ALIGN_CENTER, 0, 0);
    lv_obj_move_to_index(tach_bg, 0);

    /* 设置圆角为圆心 */
    lv_obj_set_style_radius(tach_scale, LV_RADIUS_CIRCLE, 0);
    /* 开启边角裁剪，圆形背景防止内存溢出 */
    lv_obj_set_style_clip_corner(tach_scale, true, 0);

    /* 对齐方式：右侧垂直居中（示意图：左速度、右转速 x1000rpm） */
    lv_obj_align(tach_scale, LV_ALIGN_RIGHT_MID, LV_PCT(-2), 0);

    /* 显示刻度数字标签：0~8，表示 ×1000 rpm */
    lv_scale_set_label_show(tach_scale, true);
    /* 8 段 × 每段 5 格 + 终点 = 41；主刻度落在 0,1,...,8 */
    lv_scale_set_total_tick_count(tach_scale, 41);
    lv_scale_set_major_tick_every(tach_scale, 5);

    /* LV_PART_ITEMS: 小刻度部分设置小刻度线长度为5像素 */
    lv_obj_set_style_length(tach_scale, 5, LV_PART_ITEMS);
    /* LV_PART_INDICATOR：主刻度部分设置主刻度线长度为20像素 */
    lv_obj_set_style_length(tach_scale, 20, LV_PART_INDICATOR);

    /* 转速范围 0~8（×1000 rpm），对应 0~8000 rpm */
    lv_scale_set_range(tach_scale, 0, 8);
    /* 设置环形刻度总张开角度 270度 */
    lv_scale_set_angle_range(tach_scale, 270);
    /* 设置刻度整体旋转偏移 135度，决定缺口位置 */
    lv_scale_set_rotation(tach_scale, 135);

    /* 创建指针线条，父对象为转速表 */
    tach_needle = lv_line_create(tach_scale);
    /* 设置指针线条宽度6像素 */
    lv_obj_set_style_line_width(tach_needle, 6, LV_PART_MAIN);
    /* 开启线条端点圆角 */
    lv_obj_set_style_line_rounded(tach_needle, true, LV_PART_MAIN);

    lv_obj_t * rpm_label = lv_label_create(tach_scale);
    lv_label_set_text(rpm_label, "x1000rpm");
    lv_obj_set_style_text_color(rpm_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_color(rpm_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(rpm_label, &lv_font_montserrat_18, 0);
    lv_obj_align(rpm_label, LV_ALIGN_CENTER, 0, LV_PCT(18));

    /* 将仪表盘刻度线整体设置为黑色 */
    lv_obj_set_style_line_color(tach_scale, lv_color_black(), LV_PART_ITEMS);
    lv_obj_set_style_line_color(tach_scale, lv_color_black(), LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tach_scale, lv_color_black(), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(tach_scale, lv_color_black(), LV_PART_MAIN);

    /* 将指针设置为黑色 */
    lv_obj_set_style_line_color(tach_needle, lv_color_black(), LV_PART_MAIN);

    lv_scale_set_line_needle_value(tach_scale, tach_needle, 150, 0);
}
