# 00 — 速查表

> 启动项目时最先看这个。详细内容见对应编号的 guide 文件。

## ⚠️ 三大铁律

1. **USB 和 XL4005 绝对不能同时供电！** 烧录时拔 XL4005，运行时拔 USB
2. **排查外设先查供电拓扑！** "板子有电" ≠ "所有模块有电"
3. **GPIO 输出三步走！** `initDigitalOutput` → `setPins/clearPins` → `enableOutput`

## 引脚分配速查

> 完整清单见 CLAUDE.md 或 `E:\Desktop\File\MSPM0G3507_完整接线方案.md`

### TB6612 #1（左侧电机）

| 引脚 | 功能 | LaunchPad |
|------|------|-----------|
| PB4 | PWMA (TIMA1-C0) | J1.18 |
| PB7/PB6 | AIN1/AIN2 | J2.12/J2.11 |
| PB1/PB8 | BIN1/BIN2 | J1.19/J2.13 |
| PB12 | PWMB (TIMA0-C2) | J2.19 |
| PB0 | STBY | J1.16 |

### TB6612 #2（右侧电机）

| 引脚 | 功能 | LaunchPad |
|------|------|-----------|
| PB13 | PWMA (TIMA0-C3) | J2.20 |
| PA8/PB21 | AIN1/AIN2 | J1.4/J2.15 |
| PB5/PB9 | BIN1/BIN2 | J1.12/J1.7 |
| PB20 | PWMB (TIMA0-C2) | J1.37 |
| PB2 | STBY | J1.9 |

### 舵机/通信/传感器

| 引脚 | 功能 | 备注 |
|------|------|------|
| PB10 | 舵机1 PWM (TIMG0-C0) | PCB底部，需焊排针 |
| PB11 | 舵机2 PWM (TIMG0-C1) | PCB底部，需焊排针 |
| PA10/PA11 | 蓝牙 UART0 TX/RX | ⚠️ 需改 J21/J22→2-3 |
| PB2/PB3 | MPU6050 I2C1 SCL/SDA | ⚠️ 需 4.7kΩ 上拉 3.3V |
| PB26 | RGB LED RED | 板载诊断 LED |
| PA19/PA20 | SWDIO/SWCLK | ⛔ 不可占用 |

## 硬件测试状态

| 模块 | 状态 | 日期 |
|------|------|------|
| TB6612 #1 (左) | ✅ 通过 | 2026-07-21 |
| TB6612 #2 (右) | ✅ 通过 | 2026-07-21 |
| 舵机1 (PB10/TIMG0-C0) | ✅ 通过 | 2026-07-21 |
| 舵机2 (PB11/TIMG0-C1) | ⏳ 未测 | — |
| HC-05 蓝牙 | ✅ 通过 | 2026-07-21 |
| MPU6050 陀螺仪 | ⏳ 未测 | — |
| 灰度传感器 ×8 | ⏳ 未测 | — |
| 编码器 MC310 ×4 | ⏳ 未测 | — |

## 调试检查清单

```
□ 烧录了没？ → 改完代码没 Download = MCU 里还是旧固件
□ 绿灯亮？ → 板子有电（不代表代码在跑）
□ RGB LED 闪？ → 代码在跑
□ 万用表 DC 档测信号 → ~0.25V = 舵机 PWM 正常，~3.1V = 反相/异常
□ 外设不响应 → 先查供电域（USB vs 电池 vs XL4005）
```

## 文件索引

| 你想做什么 | 看哪个 |
|-----------|--------|
| 接电源 | [01-power-architecture.md](01-power-architecture.md) |
| 改跳线 | [02-board-jumpers.md](02-board-jumpers.md) |
| 配 GPIO | [03-gpio-guide.md](03-gpio-guide.md) |
| 配 PWM/舵机 | [04-pwm-guide.md](04-pwm-guide.md) + [05-servo-guide.md](05-servo-guide.md) |
| 排查故障 | [06-debugging-guide.md](06-debugging-guide.md) |
| 查数据手册 | [07-bom-datasheets.md](07-bom-datasheets.md) |
