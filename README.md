# 纸牌消除小游戏 CardGame

这是一个使用 **C++ + Cocos2d-x 3.17.2** 开发的纸牌消除小游戏，也是我的一次应聘测试项目。

项目目标是按照给定 PDF 需求，完成一个可运行、可回退、具备基础动画表现的纸牌程序，并整理为可以直接运行和可以从源码构建的 GitHub 仓库。

## 项目简介

游戏核心规则如下：

- 主牌区中只有未被遮挡且已翻开的牌可以点击
- 手牌区显示当前顶部牌
- 若主牌区纸牌与手牌区顶部牌的点数差为 `1`，则可以消除并移动到手牌区顶部
- 备用牌堆可以点击抽牌，替换手牌区顶部牌
- 支持连续回退，并带有对应的移动动画

这个仓库同时提供两种使用方式：

- 直接运行：仓库内已包含 Windows 可执行版本
- 源码构建：仓库内包含完整 Win32 工程和源码说明

## 直接运行

仓库已经自带可运行的 Windows 版本，路径在 `Release/` 目录下。

1. 克隆或下载本仓库
2. 进入 `Release/`
3. 运行 `CardGame.exe`

如果在全新 Windows 环境中无法启动，可安装 Microsoft Visual C++ Redistributable：
https://aka.ms/vs/17/release/vc_redist.x86.exe

## 功能说明

- `Playfield`：主牌区，负责摆放可消除/被遮挡的纸牌
- `Tray`：手牌区，显示当前匹配基准牌
- `Stack`：备用牌堆，点击后抽取新牌
- `Undo`：回退按钮，支持连续撤销

匹配规则：主牌区纸牌与手牌区顶部牌点数差为 `1` 即可匹配，花色不作要求。

## 仓库结构

```text
CardGame/
├── Classes/               C++ 源码，按 MVC 思路组织
├── Resources/             卡牌图片和关卡 JSON 配置
├── Release/               已编译好的 Windows 可运行版本
├── proj.win32/            Win32 Visual Studio 工程入口文件
├── README.md              项目说明与环境配置说明
└── 程序设计文档.html        详细设计文档（中文）
```

## 源码构建说明

仓库中包含完整源码，但 **不直接提交 cocos2d 引擎本体**，因此从源码构建时需要额外准备本地引擎目录。

本机已验证通过的构建环境：

- Cocos2d-x `3.17.2`
- Visual Studio Build Tools `2022/2025`
- MSVC Toolset `v143` 或 `v145`
- Windows SDK `10.0.26100.0`
- Target Platform `Win32`

构建步骤：

1. 下载并解压 `Cocos2d-x 3.17.2`
2. 假设引擎路径为 `D:\cocos2d-x-3.17.2`
3. 在项目根目录创建名为 `cocos2d` 的 junction：

```cmd
cd CardGame
mklink /J cocos2d D:\cocos2d-x-3.17.2
```

4. 使用 MSBuild 编译：

```cmd
MSBuild proj.win32\CardGame.vcxproj /p:Configuration=Debug /p:Platform=Win32 /m
```

5. 运行输出文件：

```text
proj.win32\Debug.win32\CardGame.exe
```

补充说明：

- `proj.win32/CardGame.vcxproj` 已补全 Win32 入口文件引用
- 工程已修正链接路径，可以在干净仓库下重新完成源码构建
- 如果只是查看效果，直接运行 `Release/CardGame.exe` 最方便

## 已实现内容

对照测试需求，当前项目已实现：

- 初始底牌与主牌区匹配消除
- 主牌区遮挡/翻开逻辑
- 备用牌堆抽牌替换
- 点数差 `1` 的匹配规则
- 连续回退与移动动画
- `1080 x 2080` 分辨率与 `FIXED_WIDTH` 适配
- 基于 JSON 的关卡配置
- MVC 风格的代码结构整理

## 设计与文档

可直接打开 `程序设计文档.html` 查看完整说明，内容包括：

- 对 PDF 需求的逐项对照
- MVC 架构划分
- Bug 修复记录
- 动画系统说明
- 扩展指南

## English Note

This repository is intentionally documented in Chinese-first form because it is used as an interview test submission. The project itself remains source-complete for Win32 development and runnable out of the box through the bundled `Release/` package.

## License

For interview / educational demonstration use.
