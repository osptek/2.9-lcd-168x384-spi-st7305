#include "stdio.h"
#include "st7305.h"
#include "pins.h"

#define LCD_W 168
#define LCD_H 384

ST7305_LCD lcd(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN, SPI2_HOST, 8 * 1000 * 1000);

// 1) 横条纹：每 stripe_w 行黑白交替
static void show_stripes(int stripe_w, int hold_ms) {
    lcd.clear_display();
    for (int y = 0; y < LCD_H; ++y) {
        bool black = ((y / stripe_w) % 2 == 0);
        for (int x = 0; x < LCD_W; ++x) {
            lcd.write_point((uint16_t)x, (uint16_t)y, black);
        }
    }
    lcd.display();
    vTaskDelay(pdMS_TO_TICKS(hold_ms));
}

// 2) 棋盘方格：cell x cell 的黑白方格交替
static void show_checkerboard(int cell, int hold_ms) {
    lcd.clear_display();
    for (int y = 0; y < LCD_H; ++y) {
        for (int x = 0; x < LCD_W; ++x) {
            bool black = (((x / cell) + (y / cell)) % 2 == 0);
            lcd.write_point((uint16_t)x, (uint16_t)y, black);
        }
    }
    lcd.display();
    vTaskDelay(pdMS_TO_TICKS(hold_ms));
}

// 3) 从上到下，一行像素一行像素地刷黑
static void show_row_by_row(int delay_ms) {
    lcd.clear_display();
    lcd.display();
    for (int y = 0; y < LCD_H; ++y) {
        for (int x = 0; x < LCD_W; ++x) {
            lcd.write_point((uint16_t)x, (uint16_t)y, true);
        }
        lcd.display();
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

// 4) 从左到右，一列像素一列像素地刷黑
static void show_col_by_col(int delay_ms) {
    lcd.clear_display();
    lcd.display();
    for (int x = 0; x < LCD_W; ++x) {
        for (int y = 0; y < LCD_H; ++y) {
            lcd.write_point((uint16_t)x, (uint16_t)y, true);
        }
        lcd.display();
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

extern "C" void app_main(void) {
    lcd.initialize();
    lcd.display_on(true);
    lcd.display_inversion(false);

    lcd.clear_display();
    lcd.display();

    while (true) {
        show_stripes(16, 2000);        // 条纹，保持 2 秒
        show_checkerboard(24, 2000);   // 棋盘，保持 2 秒
        show_row_by_row(10);           // 逐行刷黑
        vTaskDelay(pdMS_TO_TICKS(500));
        show_col_by_col(10);           // 逐列刷黑
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
