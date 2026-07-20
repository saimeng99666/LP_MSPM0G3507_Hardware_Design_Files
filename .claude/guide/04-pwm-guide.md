# 04 — 定时器 PWM 配置指南

> ⚠️ **最重要的经验写在最前面**：自己写不出来的配置，去 GitHub/Gitee 找社区例程对比。
> SDK 通用例程 ≠ 实际能用。本节所有坑都是靠对比 LCKFB 社区例程才发现的。

## 定时器选型

| 定时器 | 类型 | CC 数量 | 适合 | 注意 |
|--------|------|---------|------|------|
| TIMA | 高级定时器 | 4 CC | 电机 PWM | `initPWMMode` 正常 |
| TIMG | 通用定时器 | 2 CC | 舵机 PWM / 定时中断 | ⚠️ 只用 Timer 模式或 `EDGE_ALIGN_UP` |

> **LCKFB 社区例程从未对 TIMG 使用 `DL_TimerG_initPWMMode`**。所有 PWM 用 TIMA，
> TIMG 只用于定时中断。本项目用 TIMG0 舵机是"特殊需求"，必须严格按以下模板。

## ⚠️ 边缘对齐 PWM 的极性陷阱

`DL_TIMER_CC_OCTL_INIT_VAL_LOW` + `EDGE_ALIGN`（向下数）时：
```
计数器: LOAD → … → CC → … → 0 → LOAD → …
输出:   LOW ────────┘ HIGH ────────┘ LOW

HIGH 时间 = PERIOD - CC  （不是 CC！→ 反相）
```

**解决方案**：用 `EDGE_ALIGN_UP`（向上数），自然产生正向脉冲：
```
计数器: 0 → … → CC → … → LOAD → 0 → …
输出:   ┌─HIGH─┐________________┌─HIGH─┐

HIGH 时间 = CC （正确！正向脉冲）
```

| 模式 | 需要 OCTL | 风险 |
|------|----------|------|
| `EDGE_ALIGN` + `INIT_VAL_HIGH` | 覆盖 OCTL | 复杂，与其他配置冲突 |
| **`EDGE_ALIGN_UP` + `INIT_VAL_LOW`** | 默认值即可 | ✅ 社区验证，简单可靠 |

## 舵机 PWM 标准模板（TIMG0，经 LCKFB 例程验证）

### ti_msp_dl_config.c

```c
static const DL_TimerG_ClockConfig gSERVO_ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U   // BUSCLK 32MHz / 256 = 125kHz
};

static const DL_TimerG_PWMConfig gSERVO_Config = {
    .pwmMode           = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,  // ← 向上数!
    .period            = 2500,   // 2500/125kHz = 20ms = 50Hz
    .isTimerWithFourCC = false,  // ← TIMG 只有 2 CC，必须 false
    .startTimer        = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_SERVO_init(void)
{
    DL_TimerG_setClockConfig(SERVO_PWM_INST,
        (DL_TimerG_ClockConfig *)&gSERVO_ClockConfig);
    DL_TimerG_initPWMMode(SERVO_PWM_INST,
        (DL_TimerG_PWMConfig *)&gSERVO_Config);

    DL_TimerG_setCaptureCompareValue(SERVO_PWM_INST, 0, DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareOutCtl(SERVO_PWM_INST,
        DL_TIMER_CC_OCTL_INIT_VAL_LOW,
        DL_TIMER_CC_OCTL_INV_OUT_DISABLED,
        DL_TIMER_CC_OCTL_SRC_FUNCVAL,
        DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(SERVO_PWM_INST,
        DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE,
        DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_enableClock(SERVO_PWM_INST);
    DL_TimerG_setCCPDirection(SERVO_PWM_INST,
        DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT);
}
```

### main.c

```c
int main(void) {
    SYSCFG_DL_init();
    DL_TimerG_startCounter(SERVO_PWM_INST);  // ← 必须在 main() 里启动!
    DL_TimerG_setCaptureCompareValue(SERVO_PWM_INST, 188, DL_TIMER_CC_0_INDEX); // 90°
}
```

## ⚠️ 几个关键坑（踩一次就够了）

### 坑 1：不要在 init 里调 startCounter
`DL_TimerG_enableClock`（给时钟）和 `DL_TimerG_startCounter`（启动计数）是两个独立操作。
社区例程统一在 `main()` 里调 `startCounter`，不要自作主张移到 init。

### 坑 2：不要调 setCounterControl
`initPWMMode` 已经正确设置了 CTRCTL 的所有位（CM、REPEAT、CZC、CAC、CLC、CVAE）。
再调 `setCounterControl` 是多余的，可能覆盖 PWM 模式的设置。

### 坑 3：不要自己发明 Timer Mode 版 PWM
`initTimerMode` 有三重陷阱：
- 设 CC0 为 CAPTURE 模式（`INTERM_INT_DISABLED = CAPTURE`）
- 不设 CM/REPEAT（计数器方向、周期模式）
- 不设 CVAE（计数器起始值）

用 PWM 模式就对了。

### 坑 4：EDGE_ALIGN ≠ EDGE_ALIGN_UP
`EDGE_ALIGN`（向下）在 `INIT_VAL_LOW` 下输出反相脉冲（HIGH=PERIOD-CC, 92.5%占空比→3.1V）。
`EDGE_ALIGN_UP`（向上）在 `INIT_VAL_LOW` 下是正向脉冲（HIGH=CC, 7.5%占空比→0.25V）。

### 坑 5：宏名幻觉
以下宏**不存在**于 SDK：`DL_TIMER_CC_ACTION_SET_CCP_ON_ZERO`、`DL_TIMER_CC_ACTION_CLEAR_CCP_ON_CMP_UP`
正确宏：`DL_TIMER_CC_LACT_CCP_HIGH`、`DL_TIMER_CC_CDACT_CCP_LOW`、`DL_TIMER_CC_ZACT_CCP_HIGH`、`DL_TIMER_CC_CUACT_CCP_LOW`

## 万用表快速验证

| 电压 | 原因 |
|------|------|
| ~0.25V | ✅ 正常（7.5% 占空比 @ 90°） |
| ~3.1V | 反相脉冲 / 输出卡 HIGH |
| 3.3V 恒压 | 输出卡 HIGH（CAPTURE 模式/极性反了） |
| 0V | 计数器没启动 / 代码没跑 |

## 调试方法论（血的教训）

1. **先找社区例程对照，再自己写** — LCKFB Gitee 仓库是首选参考
2. **改代码后确认 Build 了** — "万用表 3.1V" 可能是旧固件，根本没下载新代码
3. **对比 SysConfig 生成的代码** — 手动配寄存器最容易出错，生成代码是权威参考
4. **TIMG 做 PWM 要小心** — 社区全用 TIMA 做 PWM，TIMG 只做定时中断
