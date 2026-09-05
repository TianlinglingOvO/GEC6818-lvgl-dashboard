#include "head.h"

void png_func(void)
{
    lv_obj_t* png_image = lv_image_create(lv_screen_active());
    lv_image_set_src(png_image,"A:./1.png");
    lv_obj_align(png_image,LV_ALIGN_CENTER,0,0);
}

void jpg_func(void)
{
    lv_obj_t* jpg_image = lv_image_create(lv_screen_active());
    lv_image_set_src(jpg_image, "A:/kawaii.jpg");
    lv_obj_align(jpg_image,LV_ALIGN_CENTER,0,0);
}

void gif_func(void)
{
    lv_obj_t* gif_image = lv_gif_create(lv_screen_active());
    lv_gif_set_src(gif_image,"A:./biubiu.gif");
    lv_obj_center(gif_image);
}