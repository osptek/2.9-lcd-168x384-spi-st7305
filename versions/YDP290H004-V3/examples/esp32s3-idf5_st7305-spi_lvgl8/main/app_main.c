
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_st7305.h"
#include "esp_log.h"
#include "math.h"
#include "lvgl.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "esp_system.h"

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
LV_FONT_DECLARE(font_dingding);
lv_obj_t *time_label;
lv_obj_t *status_label_;
void ui_init(void)
{
    // 创建时钟标签
    time_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(time_label, &font_dingding, 0); // 设置大字体
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 60);         // 上半部分居中

    status_label_ = lv_label_create(lv_scr_act());
    lv_obj_set_size(status_label_, LV_HOR_RES - 10, 60);

    lv_obj_set_style_text_font(status_label_, &font_dingding, 0); // 设置大字体
    lv_label_set_text(status_label_, "2.9全反屏开发板，欢迎使用鱼鹰科技的产品");
    lv_obj_align(status_label_, LV_ALIGN_TOP_MID, 0, 5); // 上半部分居中
    lv_label_set_long_mode(status_label_, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // 创建公司名称标签
    lv_obj_t *company_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(company_label, &font_dingding, 0); // 设置小字体
    lv_label_set_text(company_label, "鱼鹰科技\nKevincoooool");
    lv_obj_align(company_label, LV_ALIGN_BOTTOM_MID, 0, -30); // 下半部分居中
}
static void lv_tick_task(void *arg)
{
    lv_tick_inc(10);
}

// LVGL任务处理函数
static void guiTask(void *pvParameter)
{
    while (1)
    {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

#define LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define LCD_SPI_HOST SPI2_HOST

// 引脚定义

#define PIN_NUM_LCD_CS 11
#define PIN_NUM_LCD_DC 12
#define PIN_NUM_MOSI 9
#define PIN_NUM_MISO -1
#define PIN_NUM_SCLK 10
#define PIN_NUM_LCD_RST 13
#define PIN_NUM_LCD_TE 14

static const char *TAG = "example";
esp_lcd_panel_handle_t panel_handle = NULL;
// uint8_t *lcd_buffer = NULL;
uint8_t lcd_buffer[384 * 21] = {0};

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    ESP_LOGI(TAG, "Flush area: %d, %d, %ld, %ld\n", area->x1, area->y1, w, h);
    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            // 获取LVGL的颜色值
            lv_color_t color = color_p[(y - area->y1) * w + (x - area->x1)];

            // 计算缓冲区位置和位偏移
            uint16_t byte_idx = (y >> 3) * ST7305_WIDTH + x;
            if (byte_idx >= 384 * 21)
            {
                continue;
            }

            uint8_t bit_pos = y & 0x07;
            if (color.full)
            {
                lcd_buffer[byte_idx] |= (1 << bit_pos);
            }
            else
            {
                lcd_buffer[byte_idx] &= ~(1 << bit_pos);
            }
        }
    }
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, w, h, lcd_buffer);

    // esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(disp_drv);
}

// 绘制测试图案
// 画两条边缘线和一个中心点
bool is_rotated = false; // 跟踪旋转状态

// 绘制测试图案函数
void draw_test_pattern(uint8_t *buffer, bool rotated)
{
    memset(buffer, 0, ST7305_RESOLUTION_HOR * ST7305_RESOLUTION_VER / 8);

    int width = rotated ? ST7305_RESOLUTION_VER : ST7305_RESOLUTION_HOR;
    int height = rotated ? ST7305_RESOLUTION_HOR : ST7305_RESOLUTION_VER;

    // 画左边竖线
    for (int y = 0; y < height; y++)
    {
        int x = 10; // 距离左边10像素
        uint16_t byte_idx = (y >> 3) * width + x;
        uint8_t bit_pos = y & 0x07;
        if (byte_idx < width * ((height + 7) >> 3))
        {
            buffer[byte_idx] |= (1 << bit_pos);
        }
    }

    // 画右边竖线
    for (int y = 0; y < height; y++)
    {
        int x = width - 10; // 距离右边10像素
        uint16_t byte_idx = (y >> 3) * width + x;
        uint8_t bit_pos = y & 0x07;
        if (byte_idx < width * ((height + 7) >> 3))
        {
            buffer[byte_idx] |= (1 << bit_pos);
        }
    }

    // 在中心画一个5x5的方块
    int center_x = width / 2;
    int center_y = height / 2;
    for (int y = center_y - 2; y <= center_y + 2; y++)
    {
        for (int x = center_x - 2; x <= center_x + 2; x++)
        {
            uint16_t byte_idx = (y >> 3) * width + x;
            uint8_t bit_pos = y & 0x07;
            if (byte_idx < width * ((height + 7) >> 3))
            {
                buffer[byte_idx] |= (1 << bit_pos);
            }
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_SCLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = ST7305_RESOLUTION_HOR * ST7305_RESOLUTION_VER / 8,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Initialize LCD panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_LCD_DC,
        .cs_gpio_num = PIN_NUM_LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_config, &io_handle));

    ESP_LOGI(TAG, "Initialize LCD panel driver");
    // esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 1,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7305(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, is_rotated));
    // ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Display simple pattern");

    // 准备测试数据
    uint8_t *test_pattern = heap_caps_malloc(ST7305_RESOLUTION_HOR * ST7305_RESOLUTION_VER / 8, MALLOC_CAP_DMA);
    assert(test_pattern);
    memset(test_pattern, 0, ST7305_RESOLUTION_HOR * ST7305_RESOLUTION_VER / 8);
// 初始绘制
    draw_test_pattern(test_pattern, is_rotated);
    // 显示测试图案
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, ST7305_RESOLUTION_HOR, ST7305_RESOLUTION_VER, test_pattern);

    // 定期切换显示内容
    // bool invert = false;
    // while (1)
    // {
    //     ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, invert));
    //     invert = !invert;
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }

    // 释放资源
    free(test_pattern);

    /*接入lvgl*/
    lv_init();
    static lv_disp_draw_buf_t disp_buf;

    // 分配显示缓存
    lv_color_t *buf1 = heap_caps_malloc(ST7305_WIDTH * 20 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(ST7305_WIDTH * 20 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, ST7305_WIDTH * 20);
    // lcd_buffer = heap_caps_malloc(384 * 21, MALLOC_CAP_DMA);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    // 设置原始分辨率
    disp_drv.hor_res = ST7305_WIDTH;    // 384
    disp_drv.ver_res = ST7305_HEIGHT;   // 168

    // 启用LVGL软件旋转
    // disp_drv.full_refresh = 1;
    // disp_drv.direct_mode = 1;
    disp_drv.sw_rotate = 0;
    // 设置旋转方向(0/90/180/270度)
    disp_drv.rotated = LV_DISP_ROT_NONE; // 可以改为LV_DISP_ROT_NONE/LV_DISP_ROT_90/LV_DISP_ROT_180/LV_DISP_ROT_270

    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &disp_buf;

    // 注册驱动
    lv_disp_drv_register(&disp_drv);

    // 创建LVGL定时器任务
    esp_timer_handle_t periodic_timer;
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10000)); // 1ms周期

    // 创建LVGL处理任务
    xTaskCreate(guiTask, "gui", 4096 * 2, NULL, 8, NULL);

    ui_init();
    // 时间变量
    uint8_t hours = 12;
    uint8_t minutes = 8;
    char time_buf[32];
    while (1)
    {
        // 更新时钟显示
        snprintf(time_buf, sizeof(time_buf), "%02d:%02d", hours, minutes);
        lv_label_set_text(time_label, time_buf);

        // 更新时间(这里仅作演示)
        minutes++;
        if (minutes >= 60) {
            minutes = 0;
            hours++;
            if (hours >= 24) {
                hours = 0;
            }
        }

        // 延时1分钟
        vTaskDelay(pdMS_TO_TICKS(100));  // 实际使用时可改为RTC时钟
    }
}