<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 2.9″ LCD 168×384 (ST7305 · SPI)</h1>

<p align="center"><b>Reflective LCD · SPI · ST7305</b></p>

<p align="center"><a href="./README.md">简体中文</a> | English · <a href="../../README_EN.md">Family index</a></p>

<p align="center">
  <img alt="Size: 2.9 inch" src="https://img.shields.io/badge/Size-2.9%22-3498DB?style=flat-square" />
  <img alt="Resolution: 168x384" src="https://img.shields.io/badge/Resolution-168%C3%97384-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7305" src="https://img.shields.io/badge/Driver-ST7305-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 2.9 inch 168×384 reflective LCD SPI module (ST7305) product image" src="./images/product.png" width="640" /></p>

## Contents

- [Overview](#overview)
- [Specifications](#specifications)
- [Sample projects](#sample-projects)
- [Repository layout](#repository-layout)
- [Resources](#resources)
- [Buy](#buy)
- [Support](#support)

---

## Overview

OSPTEK **2.9″ 168×384 reflective LCD** is a **SPI** monochrome display module driven by **ST7305**. Suited to low-power instruments, labels, and outdoor-readable UIs.

Spec ID (repository name): `2.9-lcd-168x384-spi-st7305`

Current module version: **YDP290H001-V3**. Electrical and mechanical details follow [`docs/YDP_290_H001_V3_1dcc5ea9e3.pdf`](./docs/YDP_290_H001_V3_1dcc5ea9e3.pdf).

## Specifications

| Item | Spec |
| ---- | ---- |
| Size | 2.9 inch |
| Type | Reflective LCD (monochrome) |
| Resolution | 168×384 |
| Interface | SPI |
| Driver IC | ST7305 |

> Full outline, FPC definition, power, and timing follow the product datasheet / driver IC datasheet.

## Sample projects

| Description | Path |
| ---- | ---- |
| ESP32-S3 · ST7305 SPI bring-up (stripes / checker / scan) | [`examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/`](./examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/) |
| ESP32-S3 · ST7305 SPI + LVGL8 | [`examples/esp32s3-idf5_st7305-spi_lvgl8/`](./examples/esp32s3-idf5_st7305-spi_lvgl8/) |

Bring-up demo GIF: [`assets/video_1.gif`](./assets/video_1.gif).

## Repository layout

```text
2.9-lcd-168x384-spi-st7305/                                # repo root (nav: ../../README_EN.md)
└── versions/
    └── YDP290H001-V3/                                # full materials for this part number
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## Resources

### Product files

| Resource | Link |
| ---- | ---- |
| Product datasheet (YDP290H001-V3) | [`docs/YDP_290_H001_V3_1dcc5ea9e3.pdf`](./docs/YDP_290_H001_V3_1dcc5ea9e3.pdf) |
| Driver IC datasheet (ST7305) | [`docs/ST_7305_V0_2_d0b99d9cdb.pdf`](./docs/ST_7305_V0_2_d0b99d9cdb.pdf) |
| 2.9″ reflective adapter schematic (DEMO) | [`docs/SCH_2.9寸全反转接板_DEMO.pdf`](./docs/SCH_2.9%E5%AF%B8%E5%85%A8%E5%8F%8D%E8%BD%AC%E6%8E%A5%E6%9D%BF_DEMO.pdf) |
| 2.9″ reflective adapter board (24pin) | [`docs/2.9寸全反转接板-24pin.pdf`](./docs/2.9%E5%AF%B8%E5%85%A8%E5%8F%8D%E8%BD%AC%E6%8E%A5%E6%9D%BF-24pin.pdf) |

### Samples

- [ESP32-S3 ST7305 SPI bring-up](./examples/esp32s3-2.9-tft-168x384-spi-st7305-bringup/)
- [ESP32-S3 ST7305 SPI + LVGL8](./examples/esp32s3-idf5_st7305-spi_lvgl8/)

## Buy

<p align="center">
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="AliExpress store" src="https://img.shields.io/badge/AliExpress-Official_Store-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://shop110742373.taobao.com/"><img alt="Taobao store" src="https://img.shields.io/badge/Taobao-Official_Store-FF6A00?style=for-the-badge" /></a>
</p>

**Overseas (AliExpress)**

- Store: [OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

**China (Taobao)**

- Store: [鱼鹰光电工厂店](https://shop110742373.taobao.com/)

## Support

- Technical support / product inquiry: <luyu@osptek.com>
- QQ group: **985881096**
- Website: <https://osptek.com/>
- Feel free to open an Issue in this repository with any questions

---

<p align="center"><sub>© 2026 OSPTEK · Materials in this repository are licensed under CC BY 4.0</sub></p>
