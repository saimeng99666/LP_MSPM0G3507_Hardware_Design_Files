# 00 — 速查表

> 启动项目时最先看这个。详细内容见对应编号的 guide 文件。

## ⚠️ 三大铁律

1. **USB 和 XL4005 绝对不能同时供电！** 烧录时拔 XL4005，运行时拔 USB
2. **排查外设先查供电拓扑！** "板子有电" ≠ "所有模块有电"
3. **GPIO 输出三步走！** `initDigitalOutput` → `setPins/clearPins` → `enableOutput`

## 引脚分配速查

| 引脚 | 功能 | LaunchPad | 备注 |
|------|------|-----------|------|
| PB4 | 电机 PWM (TIMA1-C0) | J1.18 | function 4 |
| PB0 | STBY | J1.16 | GPIO 输出 |
| PB6 | AIN2 | J2.11 | GPIO 输出 |
| PB7 | AIN1 | J2.12 | GPIO 输出 |
| PA12 | 舵机 PWM (TIMG0-C0) | J4.8 | function 4 |
| PB26 | RGB LED RED | 板载 | 诊断 LED |
| PA19 | SWDIO | 板载 | ⛔ 不可占用 |
| PA20 | SWCLK | 板载 | ⛔ 不可占用 |

## 调试检查清单

```
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
