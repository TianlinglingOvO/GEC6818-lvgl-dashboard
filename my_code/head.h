#ifndef __HEAD_H
#define __HEAD_H

#include "../lvgl/lvgl.h"

lv_style_t style;
/**
 * @brief FreeType字体示例：传入字体路径和字号，创建自定义字体样式
 * @param Font_Path ttf字体文件路径字符串
 * @param Font_Size 字体字号，单位像素
 * @retval 无
 * @note  内部生成style样式，可外部给lv_label使用；注意多次调用会内存泄漏，记得释放font
 */
void lv_Chinese_Freetype_Font_TTF_Func(char * Font_Path, int Font_Size);

#endif