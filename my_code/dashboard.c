#include "head.h"

/* 表盘指针 */
static lv_obj_t* needle_line;

static void set_needle_line_value(void* obj, int32_t v)
{
    /* obj:scale刻度对象； needle_line指针对象； 150:指针长度； v:当前刻度值 */
    lv_scale_set_line_needle_value(obj, needle_line, 150, v);
}

void Car_Speed_Ometer_Dial_Show_Gui(void)
{
    /* 创建环形刻度标对象，父对象为屏幕 */
    lv_obj_t* scale_line = lv_scale_create(lv_screen_active());
    /* 设置刻度表大小 宽、高 */
    lv_obj_set_size(scale_line, 350, 350);
    /* 设置刻度标模式：环形， 刻度线向内 */
    lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);

    /* 设置对象背景完全不透明 */
    lv_obj_set_style_bg_opa(scale_line, LV_OPA_COVER, 0);
    /* Black BG */
    lv_obj_set_style_bg_color(scale_line,lv_color_hex(0xFFFFFF),0);

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

    /* 定义动画结构体 */
    lv_anim_t anim_scale_line;
    /* 初始化动画变量 */
    lv_anim_init(&anim_scale_line);
    /* 设置动画操作对象为环形刻度scale_line */
    lv_anim_set_var(&anim_scale_line, scale_line);
    /* 设置动画每帧执行回调函数 set_needle_line_value，用来更新指针位置 */
    lv_anim_set_exec_cb(&anim_scale_line, set_needle_line_value);
    /* 单词动画持续时间3000ms */
    lv_anim_set_duration(&anim_scale_line, 3000);
    /* 设置无线循环播放 */
    lv_anim_set_repeat_count(&anim_scale_line, LV_ANIM_REPEAT_INFINITE);
    /* 回退动画时间3000ms，来回摆动 */
    lv_anim_set_playback_duration(&anim_scale_line, 3000);
    /* 动画数值区间：从0运动到260 */
    lv_anim_set_values(&anim_scale_line, 0, 260);
    /* 启动动画 */
    lv_anim_start(&anim_scale_line);
}