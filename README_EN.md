# 2.9" 168×384 reflective SPI module (ST7305) — documentation & samples

**简体中文：** [`README.md`](README.md)

---

> This repository provides **sample projects** for this module, together with datasheets, specifications, and interface / bring-up documentation for selection reference and integration.

## Product overview

| Item | Description |
|:--|:--|
| Module | 2.9-inch **reflective LCD** (monochrome), **168×384** resolution |
| Interface | **SPI** |
| Driver IC | **ST7305** |
| Spec ID | **`2.9-lcd-168x384-spi-st7305`** is the common product designation in documentation |

---

## Repository layout

### Top-level

| Path | Contents |
|:--|:--|
| `docs/` | Datasheets, specifications, adapter schematics |
| `examples/` | **Sample projects** |

### `examples/` layout

| Location | Description (internal package folder) |
|:--|:--|
| `examples/` root | **ESP-IDF代码** (ST7305 SPI + LVGL8) |

### Sample project paths

| Description | Path |
|:--|:--|
| ST7305 SPI + LVGL8 | `examples/esp32s3-idf5_st7305-spi_lvgl8/` |
