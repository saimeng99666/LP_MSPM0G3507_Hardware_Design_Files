# LP_MSPM0G3507 硬件设计项目

> 详细内容在 `.claude/guide/` 目录，按编号查阅。

## ⚠️ 核心工作规则

1. **缓存优先** — 先查 `cache/` 目录的 `.vision_cache.json`，命中再读
2. **开源优先** — 需要新工具/方案先搜 GitHub、CSDN
3. **社区例程优先** — 写代码先搜 GitHub 实战代码，SDK 通用例程不保证外设能跑
4. **主动讲原理** — 先答案再解释，不等用户追问
5. **洞洞板原则** — 板子只当安装底板，不焊走线；连线用杜邦线+排线在空中走；插模块焊排母、插线焊排针；不要剪排母(用单排母座)；I2C 必须上拉，UART 不需要
6. **降压模块不上板** — XL4005 太大(5.5×2.5cm)，放车架上，只引线到洞洞板

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
4. **MC310 电机接口注意！** → 1=电机- 2=VCC 3=A 4=B 5=GND 6=电机+，接反烧编码器！

## 🔧 当前引脚分配

> 完整 34 pin 分配 + 电源接线见 `E:\Desktop\File\MSPM0G3507_完整接线方案.md`

### 电机驱动 — TB6612 #1（左侧：左前+左后）

| 信号 | MCU 引脚 | 功能 | LaunchPad |
|------|---------|------|-----------|
| PWMA | **PB4** | TIMA1-C0 | J1.18 |
| AIN1 | **PB7** | GPIO 输出 | J2.12 |
| AIN2 | **PB6** | GPIO 输出 | J2.11 |
| STBY | **PB0** | GPIO 输出 | J1.16 |
| BIN1 | **PB1** | GPIO 输出 | J1.19 |
| BIN2 | **PB8** | GPIO 输出 | J2.13 |
| PWMB | **PB12** | TIMA0-C2 | J2.19 |

### 电机驱动 — TB6612 #2（右侧：右前+右后）

| 信号 | MCU 引脚 | 功能 | LaunchPad |
|------|---------|------|-----------|
| PWMA | **PB13** | TIMA0-C3 | J2.20 |
| AIN1 | **PA8** | GPIO 输出 | J1.4 |
| AIN2 | **PB21** | GPIO 输出 | J2.15 附近 |
| STBY | **PB2** | GPIO 输出 | J1.9 |
| BIN1 | **PB5** | GPIO 输出 | J1.12 附近 |
| BIN2 | **PB9** | GPIO 输出 | J1.7 |
| PWMB | **PB20** | TIMA0-C2 | J1.37 |

### 舵机 MG996R ×2

| 舵机 | MCU 引脚 | 功能 | LaunchPad |
|------|---------|------|-----------|
| 舵机1 (俯仰) | **PB10** | TIMG0-C0 | J1.38 (PCB底部) |
| 舵机2 (偏航) | **PB11** | TIMG0-C1 | J1.39 (PCB底部) |

### 编码器 MC310 ×4

| 电机 | A 相 | B 相 |
|------|------|------|
| 左前 | **PB15** (TIMG1-C0) | **PB16** (TIMG1-C1) |
| 左后 | **PB14** (TIMG10-IDX) | **PB23** (TIMA0-FAL) |
| 右前 | **PA28** (TIMA2-C0) | **PA31** (TIMA2-C1) |
| 右后 | **PA29** (TIMG10-C0) | **PA30** (TIMG10-C1) |

### 通信 & 传感器

| 模块 | MCU 引脚 | 协议 | 备注 |
|------|---------|------|------|
| HC-05 蓝牙 | PA10(TX), PA11(RX) | UART0 | 4线即可(VCC/GND/TX/RX)，需改 J21/J22→2-3 |
| MPU6050 陀螺仪 | PB2(SCL), PB3(SDA) | I2C1 | ⚠️ **需 4.7kΩ 上拉到 3.3V** |
| 灰度传感器 ×8 | PA24~27,PB17~19,PA22 | ADC | 5V 供电 |

### 保留/占用引脚

| 引脚 | 用途 | 说明 |
|------|------|------|
| PA19, PA20 | SWDIO, SWCLK | ⛔ 调试口，不可占用 |
| PA10, PA11 | UART0 | ⛔ 被蓝牙占用，勿复用 |
| PA0, PA1 | 预留 | 可配 I2C 上拉 |
| PB22, PB26, PB27 | RGB LED | 板载

## 📁 关键路径

| 什么 | 哪里 |
|------|------|
| 项目代码 | `e:\Desktop\MSPM0G3507_Robot\` — Git 仅本地回档，**不推远程** |
| MSPM0 SDK | `e:\Desktop\mspm0_sdk_2_10_00_04__all\mspm0_sdk_2_10_00_04\` |
| 数据手册缓存 | `cache/` |
| 完整接线方案 | `E:\Desktop\File\MSPM0G3507_完整接线方案.md` |
| 洞洞板布线指南 | `E:\Desktop\File\洞洞板布线指南.md` |
| 用户桌面 | `E:\Desktop\`（注意不是C盘） |
| 用户记忆 | `C:\Users\hp\.claude\projects\e--Desktop-LP-MSPM0G3507-Hardware-Design-Files\memory\` |
