#pragma once
#include <cstdint>
#include <cstring>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include <algorithm>


class ST7305_LCD {
public:
    ST7305_LCD(
        gpio_num_t dc, 
        gpio_num_t rst, 
        gpio_num_t cs,
        gpio_num_t sclk, 
        gpio_num_t mosi,
        spi_host_device_t 
        host = SPI2_HOST,
        int spi_clock_hz = 40 * 1000 * 1000
    );
    ~ST7305_LCD();
    void initialize();
    void fill(uint8_t data);
    void clear_display();
    void write_point(uint16_t x, uint16_t y, bool enabled);
    void write_point(uint16_t x, uint16_t y, uint16_t data);
    void display_image_at(const uint8_t* src, int imgW, int imgH, int x, int y);
    void fill_rect_at(int imgW, int imgH, int x, int y, bool is_black);
    void display_full_screen(const uint8_t* image);
    void display();

    void low_power_mode();
    void high_power_mode();
    void display_on(bool enabled);
    void display_sleep(bool enabled);
    void display_inversion(bool enabled);

private:
    // registers/data helpers
    esp_err_t write_cmd(uint8_t cmd);
    esp_err_t write_data(const uint8_t* data, size_t len);
    esp_err_t write_param(uint8_t p);
    void address();
    void initial_st7305();

private:
    const gpio_num_t DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN;
    const int LCD_WIDTH = 168;
    const int LCD_HIGH  = 384;
    const int LCD_DATA_WIDTH  = 42;         // 168/4 (可见有效字节/行对)
    const int LCD_DATA_HIGH   = 192;        // 384/2
    const int DISPLAY_BUFFER_LENGTH = 8064; // 192*42

    bool HPM_MODE = true;
    bool LPM_MODE = false;

    spi_host_device_t spi_host_;
    int spi_clock_hz_;
    spi_device_handle_t spi_dev_ = nullptr;

    uint8_t* display_buffer = nullptr;
};