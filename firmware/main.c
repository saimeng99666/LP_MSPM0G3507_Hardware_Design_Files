/**
 * MSPM0G3507 Robot — Servo Sweep Test
 * 2026 广东省电子设计竞赛 D 题
 *
 * 接线:
 *   PA12 (TIMG0-C0, J4.8) →  舵机 黄线 (SIG)
 *   舵机 红线 (VCC)   →  XL4005#1 OUT+ (6.0V)
 *   舵机 棕线 (GND)   →  星形GND
 *
 * 测试流程: 0°→180°→0° 往复扫描，同时 RGB 红灯闪烁表示程序在跑
 */

#include "ti_msp_dl_config.h"

static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        delay_cycles(32000);
    }
}

int main(void)
{
    SYSCFG_DL_init();

    /* 启动舵机 PWM 计数器 */
    DL_TimerG_startCounter(SERVO_PWM_INST);

    /* LED 闪3下: 启动信号 */
    for (int i = 0; i < 3; i++) {
        DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_26);
        delay_ms(100);
        DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_26);
        delay_ms(100);
    }

    /* 先到 90° (中间位置，安全启动) */
    servo_set(SERVO_ANGLE_90);
    delay_ms(1000);

    while (1) {
        /* ── 0° → 180° 慢扫 (step=2, 每步15ms) ── */
        DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_26);    /* LED on = 正扫 */
        for (uint16_t cc = SERVO_ANGLE_0; cc <= SERVO_ANGLE_180; cc += 2) {
            servo_set(cc);
            delay_ms(15);
        }

        /* ── 180° → 0° 回扫 ── */
        DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_26);  /* LED off = 回扫 */
        for (uint16_t cc = SERVO_ANGLE_180; cc >= SERVO_ANGLE_0; cc -= 2) {
            servo_set(cc);
            delay_ms(15);
        }

        /* 到 0° 后回中等待 1 秒，开始下一轮 */
        servo_set(SERVO_ANGLE_90);
        delay_ms(1000);
    }
}
