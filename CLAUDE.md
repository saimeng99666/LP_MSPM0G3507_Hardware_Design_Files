# LP_MSPM0G3507 硬件设计项目

> 详细内容在 `.claude/guide/` 目录，按编号查阅。

## ⚠️ 核心工作规则

1. **缓存优先** — 先查 `cache/` 目录的 `.vision_cache.json`，命中再读
2. **开源优先** — 需要新工具/方案先搜 GitHub、CSDN
3. **社区例程优先** — 写代码先搜 GitHub 实战代码，SDK 通用例程不保证外设能跑
4. **主动讲原理** — 先答案再解释，不等用户追问 — 先答案再解释，不等用户追问

## 📖 手册目录

| 编号 | 文件 | 内容 |
|------|------|------|
| 00 | [速查表](.claude/guide/00-quick-reference.md) | 引脚分配、检查清单、铁律 |
| 01 | [电源架构](.claude/guide/01-power-architecture.md) | 供电拓扑、线材、面包板坑 |
| 02 | [跳线配置](.claude/guide/02-board-jumpers.md) | J101、J19/J20、其他改动 |
| 03 | [GPIO 指南](.claude/guide/03-gpio-guide.md) | IOMUX、三步走、头文件 |
| 04 | [PWM 指南](.claude/guide/04-pwm-guide.md) | 极性陷阱、TIMG0 勘误、TIMA vs TIMG |
| 05 | [舵机指南](.claude/guide/05-servo-guide.md) | 信号标准、接线、调试流程 |
| 06 | [调试方法论](.claude/guide/06-debugging-guide.md) | 供电拓扑优先、电机/舵机排查 |
| 07 | [BOM & 数据手册](.claude/guide/07-bom-datasheets.md) | 硬件清单、手册位置、资源 |

## 🔴 三大铁律（违反必出问题）

1. **USB 和 XL4005 绝对不能同时供电！** → 烧录拔 XL4005，运行拔 USB
2. **GPIO 输出必须三步走！** → `initDigitalOutput` → `setPins/clearPins` → `enableOutput`
3. **排查外设先查供电！** → "板子有电" ≠ "所有模块有电"，USB 只管 MCU

## 🔧 当前引脚分配

| 引脚 | 功能 | 位置 |
|------|------|------|
| PB4 | 电机 PWM (TIMA1-C0) | J1.18 |
| PB0 | STBY | J1.16 |
| PB6 | AIN2 | J2.11 |
| PB7 | AIN1 | J2.12 |
| PA12 | 舵机 PWM (TIMG0-C0) | J4.8 |
| PB26 | RGB LED RED | 板载 |

## 📁 关键路径

| 什么 | 哪里 |
|------|------|
| 项目代码 | `e:\Desktop\MSPM0G3507_Robot\` |
| MSPM0 SDK | `e:\Desktop\mspm0_sdk_2_10_00_04__all\mspm0_sdk_2_10_00_04\` |
| 数据手册缓存 | `cache/` |
| 完整接线方案 | `E:\Desktop\File\MSPM0G3507_完整接线方案.md` |
| 用户记忆 | `C:\Users\hp\.claude\projects\e--Desktop-LP-MSPM0G3507-Hardware-Design-Files\memory\` |
