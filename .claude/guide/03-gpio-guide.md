# 03 — GPIO / IOMUX 配置指南

## 怎么选引脚 — IOMUX

每个引脚通过 IOMUX 连接芯片内部，同一引脚可以切到不同功能：

```
物理引脚 → [IOMUX] → GPIO / Timer PWM / UART / SPI / I2C ...
```

| 用途 | 选法 | 去哪查 |
|------|------|--------|
| GPIO 输出 | 任意引脚 | 避开调试口(SWDIO/SWDCLK)就行 |
| 定时器 PWM | 引脚必须有 `TIMAx_CCPx` / `TIMGx_CCPx` 功能 | `mspm0g350x.h` 搜对应宏 |
| UART/I2C/SPI | 引脚必须有对应外设功能 | 同上 |

本项目选了：
```
PB4 → TIMA1_CCP0 (function 4) → PWM 输出   ← 有这个脚有定时器功能
PB0 → GPIO → STBY                           ← 普通 GPIO，选哪个都行
PB6 → GPIO → AIN2                           ← 同上
PB7 → GPIO → AIN1                           ← 同上
```

## ⚠️ GPIO 输出三步走（最容易踩的坑！）

`DL_GPIO_initDigitalOutput(pin)` **只做半件事**——设 IOMUX 引脚功能为 GPIO，**不使能输出驱动（DOE）**。

```c
// ❌ 错误：电机不动，引脚其实是输入/高阻态
DL_GPIO_initDigitalOutput(IOMUX_PINCM12);

// ✅ 正确：TI 官方例程的三步模式
DL_GPIO_initDigitalOutput(IOMUX_PINCM12);           // ① 设 GPIO 功能
DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_0);              // ② 设输出高/低
DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_0);         // ③ 使能输出驱动 ← 关键!
```

| 函数 | 干了什么 | 没干什么 |
|------|---------|---------|
| `DL_GPIO_initDigitalOutput` | IOMUX: PC=CONNECT, PF=1(GPIO) | **不设 DOE** — 引脚仍是输入 |
| `DL_GPIO_setPins/clearPins` | 改 DOUT 内部寄存器 | **不设 DOE** — 物理引脚电压不变 |
| `DL_GPIO_enableOutput` | 设 DOE=1 | 必须先调前两步 |

## 外设输出配置 (PWM 等)：两步走

```c
DL_GPIO_initPeripheralOutputFunction(pin, function);  // ① 设 IOMUX 为外设功能
DL_GPIO_enableOutput(port, pin);                      // ② 使能输出驱动 ← 也要调!
```

## 器件头文件注意事项

- 本项目用的是 `mspm0g350x.h`（通过 `msp.h` 的 `DeviceFamily_ID_MSPM0G350X` 宏选中）
- **不同头文件的 IOMUX 函数编号不同**：如 `TIMA1_CCP0` 在 `g350x.h` 是 function 4，在 `g351x.h` 是 function 8
- 用宏名别用硬编码数字，宏会自动适配头文件

## LaunchPad 指示灯

- **绿色 LED（LED101，靠近 USB）**: 电源指示灯，**有电就亮，不代表代码在跑**
- **RGB LED（LED102，板子中部）**: 用户可编程
  - RED: PB26 (IOMUX_PINCM57)
  - GREEN: PB27 (IOMUX_PINCM58)
  - BLUE: PB22 (IOMUX_PINCM50)

## 调试方法论

查找 MSPM0 配置 bug 的最高效路径：
1. 在 SDK `examples/nortos/LP_MSPM0G3507/` 下找官方例程
2. 逐行对比 `ti_msp_dl_config.c` 和 `ti_msp_dl_config.h`
3. SDK 路径: `e:\Desktop\mspm0_sdk_2_10_00_04__all\mspm0_sdk_2_10_00_04\`

不要逐行读 SDK 源码——对比官方例程找差异快得多。
