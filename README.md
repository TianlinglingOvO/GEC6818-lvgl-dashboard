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
- 图片：`LV_USE_FS_STDIO` 盘符 `A:`，PNG / JPG / GIF；资源在 `image/`

## 当前进度

- [x] 工程搭建与交叉编译
- [x] 显示 / 触摸驱动接入
- [x] 基础控件、中文字库、图片/GIF 练习
- [x] 驱动层拆分到 `drives/`（字库、图片、表盘 GUI）
- [x] 仪表 UI 资源完善（背景、转向灯、燃油/水温、驾驶模式等图标）
- [x] 速度表盘：`lv_scale` + 指针 + 背景图
- [x] 转速表盘初版：`Car_tachometer_Show_Gui()`
- [x] LED 控制相关练习代码（`my_code/`，部分为测试）
- [ ] 油量水温、里程、胎压、驾驶模式、指示灯等完整联动
- [ ] 多界面切换与数据交互协议完善

当前入口：`main.c` 加载背景图后调用 `Car_Speed_Ometer_Dial_Show_Gui()`。

## 目录说明

| 路径 | 说明 |
|------|------|
| `main.c` | 程序入口：初始化显示/触摸，启动界面 |
| `drives/` | **当前参与编译的业务层**（Makefile 收集 `drives/*.c`） |
| `drives/head.h` | 对外接口声明 |
| `drives/dashboard_gui.c` | 速度表 / 转速表 GUI |
| `drives/chinese_font_lib.c` | FreeType 中文字体 |
| `drives/image.c` | PNG / JPG / GIF 加载 |
| `my_code/` | 练习与试验代码（如 LED 按钮）；默认不进当前 Makefile 通配 |
| `image/` | 仪表盘图片资源（背景、指示灯、模式图标等） |
| `freetype_tmp/` | FreeType 头文件与 ARM 库 |
| `simfang.ttf` | 字体文件（上板需一并拷贝） |
| `lvgl/` | LVGL 源码 |
| `lv_conf.h` | LVGL 配置 |
| `Makefile` | `arm-linux-gcc` + FreeType；编译 `main.c` 与 `drives/*.c` |

> 本地设计文档目录 `design_docs/` 不纳入本仓库。

## 编译与烧录

```bash
make clean && make -j16
scp ./build/bin/main ./simfang.ttf root@<板子IP>:/Mydata
# 界面图片拷到板子运行目录（与代码中 A: 路径对应）
scp -r ./image root@<板子IP>:/Mydata/
ssh root@<板子IP>
cd /Mydata && chmod 777 main && ./main
```

说明：

- 链接参数：`-L ./freetype_tmp/lib -lfreetype`
- 板子 IP、触摸节点、校准参数按实际环境修改
- 中文显示需保证运行目录能读到 `simfang.ttf`

## 说明

- 基于官方 [`lvgl/lv_port_linux`](https://github.com/lvgl/lv_port_linux)（`release/v9.2`）二次开发
- 已忽略：`build/`、`lvgl/tests|docs|scripts`、Eclipse 文件、`design_docs/`、个人作业草稿等
- 保留：`freetype_tmp/lib`、`simfang.ttf`、`image/` 界面资源
- 项目持续迭代中
