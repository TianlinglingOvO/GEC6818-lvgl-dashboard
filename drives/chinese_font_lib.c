#include "head.h"

/* ========================= 中文字库函数 ==============================*/
lv_style_t style;
/**
 * @brief FreeType字体示例：传入字体路径和字号，创建自定义字体样式
 * @param Font_Path ttf字体文件路径字符串
 * @param Font_Size 字体字号，单位像素
 * @retval 无
 * @note  内部生成style样式，可外部给lv_label使用；注意多次调用会内存泄漏，记得释放font
 */
void lv_Chinese_Freetype_Font_TTF_Func(char * Font_Path, int Font_Size)
{
    /*入参合法性检查：字体路径为空直接返回*/
    if(Font_Path == NULL || Font_Size <= 7)
    {
        LV_LOG_ERROR("Font path is NULL or font size invalid!");
        return;
    }

    /* 创建FreeType字体对象
     * 参数1：字体文件路径，外部传入
     * 参数2：渲染模式，LV_FREETYPE_FONT_RENDER_MODE_BITMAP 位图模式渲染
     * 参数3：字体大小，字号由入参Font_Size指定(像素)
     * 参数4：字体样式，LV_FREETYPE_FONT_STYLE_NORMAL 常规字体（非粗体、非斜体）
     */
    lv_font_t * font = lv_freetype_font_create(Font_Path,
                                               LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                               Font_Size,
                                               LV_FREETYPE_FONT_STYLE_NORMAL);

    /* 判断字体是否创建成功，文件不存在/路径错误/内存不足都会创建失败 */
    if(!font)
    {
        LV_LOG_ERROR("freetype font create failed.");   // 打印错误日志
        return;                                         // 创建失败直接退出，防止空指针访问
    }

    lv_style_init(&style);                              // 初始化样式对象
    lv_style_set_text_font(&style, font);              // 设置样式的文本字体为刚才创建的FreeType字体
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER); // 设置文本对齐方式：居中对齐
}
/* =================================================================================*/
