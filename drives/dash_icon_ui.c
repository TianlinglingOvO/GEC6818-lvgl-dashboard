#include "head.h"

/* 图标结构体 */
typedef struct
{
    lv_obj_t * obj; // obj
    bool on;        // status
    /* image src */
    const char * img_off;
    const char * img_on;
    int led;        // LED number(1~4)
} dash_icon_t;

/* 多变量结构体*/
typedef struct 
{
    lv_obj_t* obj;
    int idx;
    const char** imgs;
    int n;
}dash_cycle_t;

static dash_icon_t turn_right = {
    .obj = NULL,
    .on = false,
    .img_off = "A:turn_right_off.png",
    .img_on = "A:turn_right_on.png",
    .led = 2
};

static dash_icon_t turn_left = {
    .obj = NULL,
    .on = false,
    .img_off = "A:turn_left_off.png",
    .img_on = "A:turn_left_on.png",
    .led = 2
};

static dash_icon_t seat_belt = {
    .obj = NULL,
    .on = false,
    .img_off = "A:seat_belt_off.png",
    .img_on = NULL,
    .led = 4
};

static const char * mode_imgs[] = {"A:eco_on.png", "A:comfort_on.png", "A:sport_on.png"};

static dash_cycle_t car_modes = {
    .idx = 0,
    .imgs = mode_imgs,
    .n = 3,
    .obj = NULL
};

static const char * light_imgs[] = {"A:light_low_off.png", "A:light_low_on.png", "A:light_high_on.png"};

static dash_cycle_t light_modes = {
    .idx = 0,
    .imgs = light_imgs,
    .n = 3,
    .obj = NULL
};

/**
 * @brief 图标应用函数
 * 
 * @param ic 图标结构体
 */
static void dash_icon_apply(dash_icon_t* ic)
{
    // 根据当前状态选择状态图片
    const char* src = ic->on ? ic->img_on : ic->img_off;
    // 选择背景
    lv_obj_set_style_bg_image_src(ic->obj, src, 0);

    int lamp = ic->on; 
    // 处理安全带灯光
    if (ic->img_on == NULL) lamp = !ic->on;

    switch(ic->led) {
        case 1: LED_One(lamp); break;
        case 2: LED_Two(lamp); break;
        case 3: LED_Thr(lamp); break;
        case 4: LED_Four(lamp); break;
        default: break;
    }
}

/**
 * @brief 多状态切换函数
 * 
 * @param dc 多状态图标结构体
 */
static void dash_cycle_apply(dash_cycle_t* dc)
{
    // 使用下标获取图标
    const char* src = dc->imgs[dc->idx];
    // 设置图标
    lv_obj_set_style_bg_img_src(dc->obj, src, 0);
}

/**
 * @brief 图标点击更换
 * 
 * @param e lv事件结构体函数
 */
static void dash_icon_click(lv_event_t* e)
{
    // 如果不是点击，直接return
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    // 获取自己的变量类型
    dash_icon_t* ic = lv_event_get_user_data(e);
    // 状态反转
    ic->on = !ic->on;
    // 切花图标状态
    dash_icon_apply(ic);
}

/**
 * @brief 多状态图标切换(轮播)
 * 
 * @param e lvs事件结构体函数
 */
static void dash_cycle_click(lv_event_t* e)
{
    // 非点击return
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    // 回去用户数据
    dash_cycle_t* dc = lv_event_get_user_data(e);
    // 下标轮播
    dc->idx = (dc->idx + 1) % dc->n;
    // 更滑图标
    dash_cycle_apply(dc);
}

/**
 * @brief 汽车状态图标创建函数
 * 
 * @param parent 父对象
 * @param ic    图标结构体
 * @param x     x轴偏移
 * @param y     y轴偏移
 */
static void dash_icon_create(lv_obj_t* parent, dash_icon_t* ic, int x, int y)
{
    // 创建对象
    ic->obj = lv_obj_create(parent);
    // 大小设置
    lv_obj_set_size(ic->obj, 50, 50);
    // 图标背景透明
    lv_obj_set_style_bg_opa(ic->obj, LV_OPA_TRANSP, NULL);
    lv_obj_set_style_border_width(ic->obj, 0, NULL);
    lv_obj_set_style_shadow_width(ic->obj, 0, NULL);
    // 设置偏移
    lv_obj_align(ic->obj, LV_ALIGN_CENTER, x, y);
    // 回调函数，刷新图标状态
    lv_obj_add_event_cb(ic->obj, dash_icon_click, LV_EVENT_CLICKED, ic);
    // 状态初始化
    dash_icon_apply(ic);
}

/**
 * @brief 多状态图标创建函数
 * 
 * @param parent 父对象
 * @param dc 多图标结构体
 * @param x  x轴偏移
 * @param y  y轴偏移
 */
static void dash_cycle_create(lv_obj_t* parent, dash_cycle_t* dc, int x, int y)
{
    // 同dash_icon_create()函数
    dc->obj = lv_obj_create(parent);
    lv_obj_set_size(dc->obj, 50, 50);
    lv_obj_set_style_bg_opa(dc->obj, LV_OPA_TRANSP, NULL);
    lv_obj_set_style_border_width(dc->obj, 0, NULL);
    lv_obj_set_style_shadow_width(dc->obj, 0, NULL);
    lv_obj_align(dc->obj, LV_ALIGN_CENTER, x, y);
    lv_obj_add_event_cb(dc->obj, dash_cycle_click, LV_EVENT_CLICKED, dc);
    dash_cycle_apply(dc);
}

void Dash_Icon_Show_UI(void)
{
    dash_icon_create(lv_screen_active(),&turn_right, -200, -170);
    dash_icon_create(lv_screen_active(),&turn_left, -280, -170);
    dash_icon_create(lv_screen_active(),&seat_belt, -300, 0);
    dash_cycle_create(lv_screen_active(),&car_modes, 200, -170);
    dash_cycle_create(lv_screen_active(),&light_modes, 280, -170); 
}

bool dash_seatbelt_fastened(void)
{
    return seat_belt.on;
}

int dash_drive_mod(void)
{
    return car_modes.idx;
}