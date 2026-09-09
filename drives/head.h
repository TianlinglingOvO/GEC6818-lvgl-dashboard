#ifndef __HEAD_H
#define __HEAD_H

#include "../lvgl/lvgl.h"

extern lv_style_t style;
/**
 * @brief FreeType字体示例：传入字体路径和字号，创建自定义字体样式
 * @param Font_Path ttf字体文件路径字符串
 * @param Font_Size 字体字号，单位像素
 * @retval 无
 * @note  内部生成style样式，可外部给lv_label使用；注意多次调用会内存泄漏，记得释放font
 */
void lv_Chinese_Freetype_Font_TTF_Func(char * Font_Path, int Font_Size);

/**
 * @brief show speed dashboard GUI 
 */
void Car_Speed_Ometer_Dial_Show_Gui(void);

/**
 * @brief Show tachometer dashboaed GUI
 * 
 */
void Car_tachometer_Show_Gui(void);

/**
 * @brief Show Dashboard icon UI
 * 
 */
void Dash_Icon_Show_UI(void);

/**
 * @brief Show Dashboard pedal icon and control the speed 
 * 
 */
void Dash_Pedal_Show_UI(void);

/**
 * @brief Show ODO Test
 * 
 */
void Dash_Odo_Show_UI(void);

/**
 * @brief Show Clock UI
 * 
 */
void Dash_Clock_Show_UI(void);

/**
 * @brief Show Fuel and Temp UI
 * 
 */
void Dash_Fuel_Temp_Show_UI(void);

void png_func(void);
void jpg_func(void);
void gif_func(void);

int LED_Mode(char* LED_DRV_PATH, int status, int LED_IO_Number);
void LED_ALL(int status);
void LED_One(int status);
void LED_Two(int status);
void LED_Thr(int status);
void LED_Four(int status);

int beep_init(void);
void beep_on(void);
void beep_off(void);
void beep_deinit(void);

bool dash_seatbelt_fastened(void);
int dash_drive_mod(void);

/*================= Test Func ================*/
void Btn_Show_GUI_Test(void);
void Turn_Left_Control_Func_Test(void);

#endif