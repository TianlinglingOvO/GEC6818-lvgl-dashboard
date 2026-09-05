# GEC6818 LVGL 汽车仪表盘课设

基于粤嵌 **GEC6818** 开发板、**LVGL v9.2** 的嵌入式课程设计。

目标：图形化汽车仪表盘（速度、转速、油量水温、里程、胎压、驾驶模式、指示灯等）。

仓库：[`TianlinglingOvO/GEC6818-lvgl-dashboard`](https://github.com/TianlinglingOvO/GEC6818-lvgl-dashboard)

## 硬件与环境

- 开发板：GEC6818（framebuffer `/dev/fb0` + 触摸 `/dev/input/event0`）
- GUI：LVGL `release/v9.2`（基于官方 `lv_port_linux` 移植）
- 开发环境：Ubuntu（WSL）+ VS Code
- 交叉编译：`arm-linux-gcc`
- 中文字库：FreeType（`freetype_tmp/`）+ `simfang.ttf`
- 图片：`LV_USE_FS_STDIO` 盘符 `A:`，支持 PNG / JPG / GIF

## 当前进度

- [x] 工程搭建与交叉编译
- [x] 显示 / 触摸驱动接入
- [x] 基础控件练习（对象、标签、按钮、拖拽）
- [x] 中文 FreeType 字库
- [x] PNG / JPG / GIF 显示
- [x] 代码拆分到 `my_code/`
- [x] 速度表盘初版：`lv_scale` 环形刻度 + 指针 + 加油/刹车按钮 + 定时器加减速度
- [ ] 转速、油量水温、里程、胎压、驾驶模式、指示灯等完整仪表
- [ ] 多界面切换与数据交互协议完善

当前入口：`main.c` 调用 `Car_Speed_Ometer_Dial_Show_Gui()`。

## 目录说明

| 路径 | 说明 |
|------|------|
| `main.c` | 程序入口：初始化显示/触摸，启动仪表界面 |
| `my_code/` | 业务代码（字库、图片、表盘） |
| `my_code/dashboard.c` | 速度表盘 GUI 与按键/定时器逻辑 |
| `my_code/chinese_font_lib.c` | FreeType 中文字体样式 |
| `my_code/image.c` | PNG / JPG / GIF 示例 |
| `my_code/head.h` | 对外函数声明 |
| `image/` | 界面图片、GIF 资源 |
| `freetype_tmp/` | FreeType 头文件与 ARM 库 |
| `simfang.ttf` | 宋体字库（板子运行中文时需一并拷贝） |
| `lvgl/` | LVGL 源码 |
| `lv_conf.h` | LVGL 配置宏 |
| `Makefile` | 交叉编译；已加入 `my_code/*.c` 与 FreeType 链接 |

## 编译与烧录

```bash
make clean && make -j16
scp ./build/bin/main ./simfang.ttf root@<板子IP>:/Mydata
# 若界面用到图片，再拷贝 image/ 里对应文件到运行目录
ssh root@<板子IP>
cd /Mydata && chmod 777 main && ./main
```

说明：

- `Makefile` 使用 `arm-linux-gcc`，并链接 `-L ./freetype_tmp/lib -lfreetype`
- 触摸校准、板子 IP、`event` 节点按实际环境修改
- 中文显示需要板子上能读到 `simfang.ttf`（路径与代码里一致）

## 说明

- 基于官方 [`lvgl/lv_port_linux`](https://github.com/lvgl/lv_port_linux)（`release/v9.2`）二次开发
- 已忽略：`build/`、`lvgl/tests|docs|scripts`、Eclipse 工程文件、个人作业草稿等
- 保留：`freetype_tmp/lib`（ARM 库）与 `simfang.ttf`，方便直接编译与上板
- 项目持续迭代中
