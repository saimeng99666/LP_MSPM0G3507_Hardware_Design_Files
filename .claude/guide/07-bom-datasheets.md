# 07 — 物料清单 & 数据手册

## 硬件清单

| 模块 | 型号 | 数量 | 关键参数 |
|------|------|------|---------|
| MCU 开发板 | MSPM0G3507 LaunchPad | ×1 | 3.3V GPIO, 可 5V 供电 |
| 电机驱动 | TB6612FNG breakout | ×2 | **排针接口**，VM=2.5~13.5V, VCC=2.7~5.5V, Iout=1.0A/3.2A峰值 |
| 降压模块 | XL4005 (5A/300KHz) | ×2 | Vout=0.8×(1+R2/R1), 蓝色电位器调压 |
| 降压模块 | LM2596S (3A/150KHz) | ×1 | 备用 |
| 直流电机 | MC310 编码器电机 | ×4 | 7.4V, 0.5A额定/2.0A堵转 |
| 舵机 | MG996R | ×2 | 4.8~7.2V, 170mA空载/2.5A堵转, 6V时≈10kg·cm |
| 灰度传感器 | 八路灰度 | ×1 | 5V, 33mA |
| 蓝牙模块 | HC-05 (带底板) | ×1 | 3.6~6V, UART 透传, 默认 9600bps |
| 电池 | 2S LiPo | ×1 | 7.4V标称 (满电8.4V, 截止≈6.4V) |

## 数据手册位置

- `cache/TB6612FNG_Datasheet.pdf.vision_cache.json` — TB6612 数据手册
- `cache/XL4005_datasheet.pdf.vision_cache.json` — XL4005 数据手册
- `cache/HC-05_Datasheet.vision_cache.json` — HC-05 蓝牙模块数据手册
- MC310/灰度传感器: `e:\Desktop\电子设计\星坤杯寻迹小车资料\`

## 相关资源

| 资源 | 路径 |
|------|------|
| MSPM0 SDK | `e:\Desktop\mspm0_sdk_2_10_00_04__all\mspm0_sdk_2_10_00_04\` |
| 星坤杯资料 | `e:\Desktop\电子设计\星坤杯寻迹小车资料` |
| MCP Server | `D:\code\MCP Server\DeepSeek Vision\server.py` |
| 项目代码 | `e:\Desktop\MSPM0G3507_Robot\` |

## 项目背景

- **竞赛**: 2026 广东省电子设计竞赛 D 题 — 可自动行驶的瞄准装置
- **方案**: 四驱小车底盘 + 二轴云台 + 视觉瞄准
- **用户**: 大一信息工程，会 C/C++/数学/电路基础，电子设计新手
