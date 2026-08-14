<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 2.9″ LCD 168×384（ST7305 · SPI）</h1>

<p align="center"><b>全反射 LCD · SPI · ST7305</b></p>

<p align="center"><a href="./README_EN.md">English</a> | 简体中文 · <a href="../../README.md">规格族索引</a></p>

<p align="center">
  <img alt="Size: 2.9 inch" src="https://img.shields.io/badge/Size-2.9%22-3498DB?style=flat-square" />
  <img alt="Resolution: 168x384" src="https://img.shields.io/badge/Resolution-168%C3%97384-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7305" src="https://img.shields.io/badge/Driver-ST7305-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 2.9 寸 168×384 全反射 LCD SPI 模组（ST7305）宣传图" src="./images/product.png" width="640" /></p>

## 目录

- [产品简介](#产品简介)
- [规格参数](#规格参数)
- [示例工程](#示例工程)
- [仓库结构](#仓库结构)
- [相关资料](#相关资料)
- [购买链接](#购买链接)
- [技术支持](#技术支持)

---

## 产品简介

OSPTEK **2.9 寸 168×384 全反射 LCD** 是一款 **SPI** 接口黑白显示模组，显示驱动为 **ST7305**。适合低功耗仪表、标签与户外可读场景。

规格标识（仓库名）：`2.9-lcd-168x384-spi-st7305`

当前模组版本：**YDP290H001-V3**。电气与外形细节以 [`docs/YDP_290_H001_V3_1dcc5ea9e3.pdf`](./docs/YDP_290_H001_V3_1dcc5ea9e3.pdf) 为准。

## 规格参数

| 项目 | 规格 |
| ---- | ---- |
| 尺寸 | 2.9 英寸 |
| 类型 | 全反射 LCD（黑白） |
| 分辨率 | 168×384 |
| 接口 | SPI |
| 驱动 IC | ST7305 |

> 完整外形尺寸、FPC 定义、供电与时序以产品规格书 / 驱动手册为准。

## 示例工程

| 说明 | 路径 |
| ---- | ---- |
| ESP32-S3 · ST7305 SPI bring-up（条纹 / 棋盘 / 逐行刷新） | [`examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/`](./examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/) |
| ESP32-S3 · ST7305 SPI + LVGL8 | [`examples/esp32s3-idf5_st7305-spi_lvgl8/`](./examples/esp32s3-idf5_st7305-spi_lvgl8/) |

bring-up 演示动图见 [`assets/video_1.gif`](./assets/video_1.gif)。

## 仓库结构

```text
2.9-lcd-168x384-spi-st7305/                                # 仓库根（导航见 ../../README.md）
└── versions/
    └── YDP290H001-V3/                                # 本料号完整资料
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## 相关资料

### 本产品资料

| 资料 | 链接 |
| ---- | ---- |
| 产品规格书（YDP290H001-V3） | [`docs/YDP_290_H001_V3_1dcc5ea9e3.pdf`](./docs/YDP_290_H001_V3_1dcc5ea9e3.pdf) |
| 驱动 IC 数据手册（ST7305） | [`docs/ST_7305_V0_2_d0b99d9cdb.pdf`](./docs/ST_7305_V0_2_d0b99d9cdb.pdf) |
| 2.9 寸全反转接板原理图（DEMO） | [`docs/SCH_2.9寸全反转接板_DEMO.pdf`](./docs/SCH_2.9%E5%AF%B8%E5%85%A8%E5%8F%8D%E8%BD%AC%E6%8E%A5%E6%9D%BF_DEMO.pdf) |
| 2.9 寸全反转接板（24pin） | [`docs/2.9寸全反转接板-24pin.pdf`](./docs/2.9%E5%AF%B8%E5%85%A8%E5%8F%8D%E8%BD%AC%E6%8E%A5%E6%9D%BF-24pin.pdf) |

### 示例工程

- [ESP32-S3 ST7305 SPI bring-up](./examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/)
- [ESP32-S3 ST7305 SPI + LVGL8](./examples/esp32s3-idf5_st7305-spi_lvgl8/)

## 购买链接

<p align="center">
  <a href="https://shop110742373.taobao.com/"><img alt="淘宝官方店铺" src="https://img.shields.io/badge/淘宝-官方店铺-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="速卖通官方店铺" src="https://img.shields.io/badge/速卖通-官方店铺-FF6A00?style=for-the-badge" /></a>
</p>

**国内（淘宝）**

- 店铺：[鱼鹰光电工厂店](https://shop110742373.taobao.com/)

**海外（AliExpress）**

- 店铺：[OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

## 技术支持

- 技术支持 / 产品咨询：<luyu@osptek.com>
- QQ 技术交流群：**985881096**
- 公司官网：<https://osptek.com/>
- 有任何问题，都可以在本仓库 Issues 中提问

---

<p align="center"><sub>© 2026 OSPTEK 鱼鹰光电 · 本仓库资料采用 CC BY 4.0 许可</sub></p>
