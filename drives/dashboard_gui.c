#include "head.h"
#include <stdio.h>
#include <time.h>

static lv_obj_t * needle_line;    /* 速度表指针 */
static lv_obj_t * scale_line;     /* 速度表 */
static lv_obj_t * tach_needle;    /* 转速表指针 */
static lv_obj_t * tach_scale;     /* 转速表 */
static bool accel_held = false;   /* 加速状态*/
static bool brake_held = false;   /* 减速状态 */
static int32_t speed_kmh = 0;     /* 速度 */
static lv_obj_t* odo_label;       /* 里程表标签*/
static float odo_km = 0;          /* 里程表总数*/
static float trip_km = 0;           /* 本次里程数 */
static lv_obj_t* clk_label;         /* 时钟标签 */
static float fuel_pct = 100;        /* 油箱油量 */
static float coolant_c = 80;        /* 水温计温度 */
static lv_obj_t* fuel_obj;          /* 油箱*/
static lv_obj_t* temp_obj;          /* 水温计 */


// 加速挡位
static const int accel_step[] = {3,6,9};
// 最高限速
static const int speed_cap[] = {60, 130, 260};

/**
 * @brief 踏板事件函数
 * 
 * @param e lv事件结构体
 */
static void pedal_event(lv_event_t* e)
{
    // 获取事件对象信息
    lv_event_code_t code = lv_event_get_code(e);
    // 按住状态获取
    bool* held = lv_event_get_user_data(e);

    // 按住
    if(code == LV_EVENT_PRESSED) {
        *held = true;
        printf("PRESSED\n");
    }
    // 松开
    if(code == LV_EVENT_RELEASED) {
        *held = false;
        printf("RELEASED\n");
    }
}

/**
 * @brief 踏板创建
 * 
 * @param parent 创建绑定的父对象 
 * @param src 图标路径
 * @param x   x偏移
 * @param y   y偏移
 * @param held  用户自定义变量
 */
static void pedal_create(lv_obj_t* parent, const char* src, int x, int y, bool* held)
{
    // 同dash_icon_crate()
    lv_obj_t* pedal = lv_obj_create(parent);
    lv_obj_set_size(pedal, 80, 80);
    lv_obj_set_style_bg_opa(pedal, LV_OPA_TRANSP, NULL);
    lv_obj_set_style_border_width(pedal, 0, NULL);
    lv_obj_set_style_shadow_width(pedal, 0, NULL);
    lv_obj_align(pedal, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_bg_image_src(pedal, src, 0);
    // lv_obj_remove_flag(pedal, LV_OBJ_FLAG_SCROLLABLE);
    // lv_obj_remove_flag(pedal, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_add_event_cb(pedal, pedal_event, LV_EVENT_ALL, held);
}

/**
 * @brief 警示图标创建函数
 * 
 * @param parent 父对象
 * @param x x偏移
 * @param y y偏移
 * @return lv_obj_t* lv对象结构体
 */
static lv_obj_t* warn_icon_create(lv_obj_t* parent, int x, int y) 
{
    // 同dash_icon_create()
    lv_obj_t* o = lv_obj_create(parent);
    lv_obj_set_size(o, 50, 50);
    lv_obj_set_style_bg_opa(o, LV_OPA_TRANSP, NULL);
    lv_obj_set_style_border_width(o, 0, NULL);
    lv_obj_set_style_shadow_width(o, 0, NULL);
    lv_obj_align(o, LV_ALIGN_CENTER, x, y);
    return o;
}

/**
 * @brief Set the speed needle value object
 * 
 * @param obj 控制对象
 * @param v  速度数值大小
 */
static void set_speed_needle_value(void* obj, int32_t v)
{
    /* obj:scale刻度对象； needle_line指针对象； 120:指针长度； v:当前刻度值 */
    lv_scale_set_line_needle_value(obj, needle_line, 120, v);
}

/**
 * @brief Set the tach neddle value object
 * 
 * @param obj 控制对象
 * @param v 转速大小
 */
static void set_tach_neddle_value(void* obj, int32_t v)
{
    /* obj:scale刻度对象； tach_line指针对象； 120:指针长度； v:当前刻度值 */
    lv_scale_set_line_needle_value(obj, tach_needle, 120, v);
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
    lv_image_set_src(speed_bg, "A:speed_bg.jpg");
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

    // 设置文本标签
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


    // 初始化指针位置
    set_speed_needle_value(scale_line, 0);
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
    lv_image_set_src(tach_bg, "A:tachometer_bg.jpg");
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

    // 设置转速文本标签
    lv_obj_t * rpm_label = lv_label_create(tach_scale);
    lv_label_set_text(rpm_label, "x1000rpm");
    // lv_obj_set_style_text_color(rpm_label, lv_color_hex(0xFFFFFF), 0);
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

    // 初始化转速指针
    set_tach_neddle_value(tach_scale, 0);
}

/**
 * @brief 读取加载里程表
 * 
 */
static void odo_load(void)
{
    // 打开文件
    FILE* fp = fopen("/Mydata/odometer.txt", "r");
    // 不存在return
    if(!fp) return;
    // 读取数据
    if (fscanf(fp, "%f", &odo_km) != 1) odo_km = 0;
    fclose(fp);
}

/**
 * @brief 里程数保存函数
 * 
 */
static void odo_save(void)
{
    //打开
    FILE* fp = fopen("/Mydata/odometer.txt", "w");
    if(!fp) return;
    // 写入
    fprintf(fp, "%.3f\n", odo_km);
    // 关闭
    fclose(fp);
}

/**
 * @brief 时钟回调函数
 * 
 * @param t lv时间结构体
 */
static void clk_timer_cb(lv_timer_t* t)
{
    // 用不到，直接放生
    (void)t;

   // 获取当前时间戳
   time_t now = time(NULL);
   struct tm* tm = localtime(&now);
   if (!tm) return;
   // 格式化输出当前时间
   lv_label_set_text_fmt(clk_label, "%02d:%02d:%02d", tm->tm_hour,tm->tm_min,tm->tm_sec);
}

/**
 * @brief 速度定时回调函数
 * 
 * @param t 
 */
static void speed_timer_cb(lv_timer_t* t)
{
    (void)t;
    static bool alarm_on = false;   // 闹钟状态
    static int blink_div = 0;       // 闪烁计时器 
    static int save_div = 0;        // 里程保存计时器

    save_div++;
    // 到时间保存
    if (save_div >= 20) 
    {
        save_div = 0;
        odo_save();
    }

    // 想要加速要同时满足 按压 && 安全带系上 && 油量 > 0
    bool want_accel = accel_held && dash_seatbelt_fastened() && (fuel_pct > 0);
    // 报警要求: 按压 && 没有系上安全带
    bool alarm = accel_held && !dash_seatbelt_fastened();

    // 获取当前运动模式的下标
    int m = dash_drive_mod();
    if (m < 0) m = 0;
    if (m > 2) m = 2;

    // 刹车-4
    if (brake_held) speed_kmh -= 4;
    // 超过模式限速时不断 -1
    else if (speed_kmh > speed_cap[m]) speed_kmh -= 1;
    // 加速
    else if (want_accel) {
        // 按住模式加速
        speed_kmh += accel_step[m];
        // 最高速保持不变
        if (speed_kmh > speed_cap[m]) 
        {
            speed_kmh = speed_cap[m];
        }
    }
    // 默认减速
    else speed_kmh -= 1;

    // 防止速度小于0
    if (speed_kmh < 0) speed_kmh = 0;

    // 速度大于0 油量下降 温度上升
    if (speed_kmh > 0) 
    {
        fuel_pct -= 0.80f;
        coolant_c += 0.20f;
    }
    // 停止时温度下降
    else
    {
        coolant_c -= 0.30f;
    }

    // 边界保护
    if(fuel_pct < 0)    fuel_pct = 0;
    if(fuel_pct > 100)  fuel_pct = 100;
    if(coolant_c < 40)  coolant_c = 40;
    if(coolant_c > 120) coolant_c = 120;

    // 油箱状态图标切换
    if(fuel_pct <= 20)
        lv_obj_set_style_bg_image_src(fuel_obj, "A:fuel_low.png", 0);
    else if(fuel_pct <= 60)
        lv_obj_set_style_bg_image_src(fuel_obj, "A:fuel_media.png", 0);
    else
        lv_obj_set_style_bg_image_src(fuel_obj, "A:fuel_high.png", 0);

    // 水温计图标切换
    if(coolant_c < 70)
        lv_obj_set_style_bg_image_src(temp_obj, "A:temp_low.png", 0);
    else if(coolant_c < 105)
        lv_obj_set_style_bg_image_src(temp_obj, "A:temp_media.png", 0);
    else
        lv_obj_set_style_bg_image_src(temp_obj, "A:temp_high.png", 0);

    // 报警模式
    if (alarm) 
    {
        // 闪烁
        blink_div++;
        if (blink_div >= 6)
        {
            blink_div = 0;
            alarm_on = !alarm_on;
            // LED3 闪烁 蜂鸣器启动
            LED_Thr(alarm_on);
            if (alarm_on)
            {
                beep_on();
            }
            else
            {
                beep_off();
            }
        }
    }
    else    // 不闪烁时关闭蜂鸣器，初始化定时器
    {
        blink_div = 0;
        if (alarm_on)
        {
            alarm_on = false;
            beep_off();
            LED_Thr(dash_seatbelt_fastened());
        }
    }

    // 里程计算 显示
    odo_km += (float)speed_kmh / 72000.0f;
    trip_km += (float)speed_kmh / 72000.0f;
    lv_label_set_text_fmt(odo_label, "ODO  %06d km\nTRIP %06.1f km", (int)odo_km, trip_km);

    // 优化：最大和最小速度的时候不刷新指针
    static int32_t last_speed = -1;
    if (speed_kmh == last_speed) return;
    last_speed = speed_kmh;

    set_speed_needle_value(scale_line, speed_kmh);
    set_tach_neddle_value(tach_scale, speed_kmh * 8 / 260);

}

void Dash_Pedal_Show_UI(void)
{
    // lv_obj_remove_flag(lv_screen_active(), LV_OBJ_FLAG_SCROLLABLE);
    pedal_create(lv_screen_active(), "A:acc.png", -210, 200, &accel_held);
    pedal_create(lv_screen_active(), "A:brake.png", -290, 200, &brake_held);
    lv_timer_create(speed_timer_cb, 50, NULL);
}

void Dash_Odo_Show_UI(void)
{
    odo_load();
    odo_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_color(odo_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(odo_label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_align(odo_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text_fmt(odo_label, "ODO  %06d km\nTRIP %06.1f km", (int)odo_km, trip_km);
    lv_obj_align(odo_label, LV_ALIGN_CENTER, 0, 200);
}

void Dash_Clock_Show_UI(void)
{
    clk_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_color(clk_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(clk_label, &lv_font_montserrat_30, 0);
    lv_obj_align(clk_label, LV_ALIGN_CENTER, 0, -180);

    clk_timer_cb(NULL);
    lv_timer_create(clk_timer_cb, 1000, NULL);
}

void Dash_Fuel_Temp_Show_UI(void)
{
    fuel_obj = warn_icon_create(lv_screen_active(), -250, 120);
    temp_obj = warn_icon_create(lv_screen_active(), 250, 120);
}