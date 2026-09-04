# GEC6818 LVGL 汽车仪表盘课设

基于粤嵌 **GEC6818** 开发板、**LVGL v9.2** 的嵌入式课程设计。

目标是做一个图形化汽车仪表盘（速度、转速、油量水温、里程、胎压、驾驶模式、指示灯等）。当前还在练习阶段：环境搭建、控件、中文字库、图片/GIF 显示；仪表盘界面尚未开始正式开发。

仓库：[`TianlinglingOvO/GEC6818-lvgl-dashboard`](https://github.com/TianlinglingOvO/GEC6818-lvgl-dashboard)

## 硬件与环境

- 开发板：GEC6818
- GUI：LVGL `release/v9.2`
- 移植工程：`lv_port_linux`（framebuffer + evdev 触摸）
- 开发环境：Ubuntu（WSL）+ VS Code + `arm-linux-gcc` 交叉编译
- 中文字库：FreeType + `simfang.ttf`

## 目录说明（简要）

| 路径 | 说明 |
|------|------|
| `main.c` | 程序入口：显示/触摸初始化，示例调用 |
| `my_code/` | 自己写的练习与功能代码 |
| `image/` | 图片、GIF 等资源 |
| `lvgl/` | LVGL 源码 |
| `freetype_tmp/` | FreeType 头文件与库 |
| `lv_conf.h` | LVGL 配置（驱动、字库、图片解码等宏） |
| `Makefile` | 交叉编译与链接 FreeType |

## 当前进度

- [x] 工程与交叉编译环境
- [x] 显示 / 触摸驱动接入
- [x] 基础控件（对象、标签、按钮、拖拽）
- [x] 中文 FreeType 字库
- [x] PNG / JPG / GIF 显示练习
- [ ] 仪表盘界面与数据交互（待开始）

## 编译与烧录（板子）

```bash
make clean && make -j16
scp ./build/bin/main root@<板子IP>:/Mydata
# 如需中文/图片，一并拷贝字体与资源文件
ssh root@<板子IP>
cd /Mydata && chmod 777 main && ./main
```

> `Makefile` 需使用 `arm-linux-gcc`；板子 IP、触摸节点（如 `/dev/input/event0`）按实际环境修改。

## 说明

- 本仓库基于官方 [`lvgl/lv_port_linux`](https://github.com/lvgl/lv_port_linux)（`release/v9.2`）二次开发。
- 大体积字体包、`build/` 产物、个人作业草稿等不纳入版本管理。
- 项目仍在迭代，文档会随仪表盘开发补全。
