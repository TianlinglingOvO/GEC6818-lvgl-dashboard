# GEC6818 LVGL 汽车仪表盘课设

基于粤嵌 **GEC6818** 开发板、**LVGL v9.2** 的嵌入式课程设计作品。

目标：在开发板上实现图形化汽车仪表盘 UI，并联动部分板载外设（LED / 蜂鸣器）。

## UI展示

![UI](/Show_UI.jpg)

## 功能一览

- **速度表 / 转速表**：`lv_scale` 环形刻度 + 指针 + 背景图
- **油门 / 刹车踏板**：按住加速或减速，速度与转速联动更新
- **安全带联锁**：未系安全带时加速触发蜂鸣器告警，并限制加速逻辑
- **驾驶模式**：经济 / 舒适 / 运动（影响加减速手感）
- **指示灯图标**：左右转向、远近光、安全带等状态切换
- **燃油 / 水温**：按油量与温度区间切换 low / media / high 图标
- **里程 (ODO) / 时钟**：仪表信息区显示
- **外设**：蜂鸣器 (`/dev/beep`)、LED (`/dev/led_drv`) 接口

入口：`main.c` 初始化显示、触摸与蜂鸣器后，依次创建上述界面模块。

## 硬件与环境

- 开发板：GEC6818（framebuffer `/dev/fb0` + 触摸 `/dev/input/event0`）
- GUI：LVGL `release/v9.2`（基于官方 `lv_port_linux`）
- 开发环境：Ubuntu（WSL）+ VS Code
- 交叉编译：`arm-linux-gcc`
- 中文字库：FreeType（`freetype_tmp/`）+ `simfang.ttf`
- 图片：`LV_USE_FS_STDIO` 盘符 `A:`，资源位于 `image/`

## 目录结构

| 路径 | 说明 |
|------|------|
| `main.c` | 程序入口 |
| `drives/` | 业务代码（Makefile 编译 `drives/*.c`） |
| `drives/dashboard_gui.c` | 速度/转速表、踏板、ODO、时钟、油量水温逻辑 |
| `drives/dash_icon_ui.c` | 指示灯与驾驶模式等图标 UI |
| `drives/chinese_font_lib.c` | FreeType 中文字体 |
| `drives/image.c` | 图片/GIF 加载 |
| `drives/buzzer.c` | 蜂鸣器驱动封装 |
| `drives/led.c` | LED 控制 |
| `drives/head.h` | 对外接口声明 |
| `image/` | 仪表盘图片资源 |
| `freetype_tmp/` | FreeType 头文件与 ARM 库 |
| `simfang.ttf` | 字体（上板需一并拷贝） |
| `lvgl/` | LVGL 源码 |
| `lv_conf.h` | LVGL 配置 |
| `Makefile` | 交叉编译与 FreeType 链接 |

本地设计文档 `design_docs/`、调试图 `image_debug/` 不纳入仓库。

## 编译与烧录

```bash
make clean && make -j16
scp ./build/bin/main ./simfang.ttf root@<板子IP>:/Mydata
scp -r ./image root@<板子IP>:/Mydata/
ssh root@<板子IP>
cd /Mydata && chmod 777 main && ./main
```

说明：

- `Makefile` 使用 `arm-linux-gcc`，链接 `-L ./freetype_tmp/lib -lfreetype`
- 板子 IP、触摸节点、校准参数按实际环境修改
- 运行目录需能读到 `simfang.ttf` 与 `image/` 中对应资源（路径与代码里 `A:` 一致）
- 蜂鸣器 / LED 依赖板载驱动节点：`/dev/beep`、`/dev/led_drv`

## 说明

- 基于官方 [`lvgl/lv_port_linux`](https://github.com/lvgl/lv_port_linux)（`release/v9.2`）二次开发
- 已忽略：`build/`、`lvgl/tests|docs|scripts`、Eclipse 工程文件、`design_docs/`、`image_debug/`、个人作业草稿等
- 保留：`freetype_tmp/lib`、`simfang.ttf`、`image/` 正式界面资源
