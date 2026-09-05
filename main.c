#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "my_code/head.h" /* Define Head File */

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ? : dflt;
}

#if LV_USE_LINUX_FBDEV
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t * disp = lv_linux_fbdev_create();

    lv_linux_fbdev_set_file(disp, device);
}
#elif LV_USE_LINUX_DRM
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_DRM_CARD", "/dev/dri/card0");
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}
#elif LV_USE_SDL
static void lv_linux_disp_init(void)
{
    const int width = atoi(getenv("LV_SDL_VIDEO_WIDTH") ? : "800");
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ? : "480");

    lv_sdl_window_create(width, height);
}
#else
#error Unsupported configuration
#endif

#if LV_USE_EVDEV
 static void lv_linux_touch_init(void)
 {
     lv_indev_t * touch_indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
     if(touch_indev == NULL)
     {
         printf("[Touch ERROR] open /dev/input/event6 failed !\n");
         return;
     }
     lv_indev_set_display(touch_indev, lv_display_get_default());
     lv_evdev_set_calibration(touch_indev, 0, 0, 1024, 480);
    //  lv_evdev_set_swap_axes(touch_indev, true);
     printf("[Touch OK] /dev/input/event6 init success\n");
     printf(" GUI!\n");
 }
#endif



int main(void)
{
    lv_init();  /* LVGL初始化函数 */


#if LV_USE_LINUX_FBDEV  /* 显示设备初始化函数 */
    /*Linux display device init*/
    lv_linux_disp_init();
#endif

#if LV_USE_EVDEV
    lv_linux_touch_init();
#endif

    Car_Speed_Ometer_Dial_Show_Gui();
    
    /*Handle LVGL tasks*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
