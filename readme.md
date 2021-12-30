# 五子棋对战程序



**Github 项目地址**: https://github.com/TimeSea05/Gobang-Work

## 1. 开发环境

本程序使用VSCode for Linux开发。

**开发环境**：`Ubuntu 21.04`

**编译环境**：`gcc 10.3.0`

**调试环境**：`gdb 10.1.9`

**如何编译本程序：**

将本程序克隆到`Ubuntu桌面版`，在文件路径打开终端，输入`make`即可开始编译。

如果你想使用VSCode对本程序进行二次开发，且你已经使用VSCode搭建好了C/C++开发环境，那么可以直接使用VSCode打开程序文件夹，按`F5`(或`Fn+F5`)即可开始调试。

## 2. 程序说明

此程序有两个模式：人人对战与人机对战模式。运行程序时，根据程序提示即可进行游戏。

本程序的输入输出为`UTF-8`编码。如果您发现在运行时程序乱码，大概率是因为您的终端的编码不是**UTF-8**。

建议运行程序时将终端的背景颜色调整为白色。

如果输出时发现棋盘变窄，可以考虑在终端中打开`Preference`选项，找到`Profiles`，点击`Compatibility`,将`Ambiguous-width characters`选项设置为`Wide`。

## 3. 程序运行快照

![程序运行快照](images/ex.png)