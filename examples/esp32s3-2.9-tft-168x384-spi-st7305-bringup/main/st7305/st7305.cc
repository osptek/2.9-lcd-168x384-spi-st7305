#include "st7305.h"

#define LCD_H   168  
#define LCD_W   384 

static const char* TAG = "ST7305";

ST7305_LCD::ST7305_LCD(gpio_num_t dc, gpio_num_t rst, gpio_num_t cs,
    gpio_num_t sclk, gpio_num_t mosi,
    spi_host_device_t host, int spi_clock_hz)
    : DC_PIN(dc), RES_PIN(rst), CS_PIN(cs), SCLK_PIN(sclk), SDIN_PIN(mosi),
      spi_host_(host), spi_clock_hz_(spi_clock_hz) 
{
    // 显存使用 DMA 可用内存，便于一次性刷屏
    display_buffer = (uint8_t*)heap_caps_malloc(DISPLAY_BUFFER_LENGTH, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    if (display_buffer) {
        memset(display_buffer, 0x00, DISPLAY_BUFFER_LENGTH);
    }
}

ST7305_LCD::~ST7305_LCD() {
    if (spi_dev_) {
        spi_bus_remove_device(spi_dev_);
        spi_dev_ = nullptr;
    }
    spi_bus_free(spi_host_);
    if (display_buffer) { free(display_buffer); display_buffer = nullptr; }
}

void ST7305_LCD::initialize() {
    gpio_config_t io = {};
    io.mode = GPIO_MODE_OUTPUT;
    io.pin_bit_mask = (1ULL << DC_PIN) | (1ULL << RES_PIN);
    io.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io.pull_up_en   = GPIO_PULLUP_DISABLE;
    io.intr_type    = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io));

    spi_bus_config_t buscfg = {};
    buscfg.sclk_io_num = SCLK_PIN;
    buscfg.mosi_io_num = SDIN_PIN;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    // 一次整屏写入需要较大 TX buffer
    buscfg.max_transfer_sz = DISPLAY_BUFFER_LENGTH + 16;   // 照 reference: 8064 整屏发送
    ESP_ERROR_CHECK(spi_bus_initialize(spi_host_, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t dev{};
    dev.clock_speed_hz = spi_clock_hz_;
    dev.mode = 0;
    dev.spics_io_num = CS_PIN;           // 交给主机拉 CS
    dev.queue_size = 4;
    dev.flags = SPI_DEVICE_NO_DUMMY;
    ESP_ERROR_CHECK(spi_bus_add_device(spi_host_, &dev, &spi_dev_));

    gpio_set_level(RES_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RES_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RES_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    initial_st7305();
    fill(0x00);
}

void ST7305_LCD::fill(uint8_t data) {
    memset(display_buffer, data, DISPLAY_BUFFER_LENGTH);
    ESP_LOGI(TAG, "fill data = 0x%02x", data);
}

void ST7305_LCD::clear_display() {
    memset(display_buffer, 0x00, DISPLAY_BUFFER_LENGTH);
}

void ST7305_LCD::write_point(uint16_t x, uint16_t y, bool enabled) {
    if (x >= LCD_WIDTH || y >= LCD_HIGH) {
        return;
    }

    // 计算目标字节与位位置
    uint16_t real_x = x / 4;     // 每 4 个像素占一个 byte
    uint16_t real_y = y / 2;     // 上下两行共用一行数据
    uint32_t write_byte_index = real_y * LCD_DATA_WIDTH + real_x;

    uint8_t one_two = (y % 2 == 0) ? 0 : 1;  // 上下行标志
    uint8_t line_bit_4 = x % 4;               // 一字节中的像素组编号
    uint8_t write_bit = 7 - (line_bit_4 * 2 + one_two);

    if (enabled) {
        display_buffer[write_byte_index] |= (1 << write_bit);   // 置 1
    } else {
        display_buffer[write_byte_index] &= ~(1 << write_bit);  // 置 0
    }
}

void ST7305_LCD::write_point(uint16_t x, uint16_t y, uint16_t data) {
    write_point(x, y, data != 0);
}

esp_err_t ST7305_LCD::write_cmd(uint8_t cmd) {
    gpio_set_level(DC_PIN, 0); // 命令
    spi_transaction_t t{};
    t.length = 8;
    t.tx_buffer = &cmd;
    return spi_device_polling_transmit(spi_dev_, &t);
}

esp_err_t ST7305_LCD::write_param(uint8_t p) {
    gpio_set_level(DC_PIN, 1); // 数据/参数
    spi_transaction_t t{};
    t.length = 8;
    t.tx_buffer = &p;
    return spi_device_polling_transmit(spi_dev_, &t);
}


esp_err_t ST7305_LCD::write_data(const uint8_t* data, size_t len) {
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t t{};
    t.length = len * 8;
    t.tx_buffer = data;
    return spi_device_polling_transmit(spi_dev_, &t);
}

void ST7305_LCD::initial_st7305() {
    // 照 reference/ST7305-main 的初始化（带数据手册 YDP290H001）
    write_cmd(0xD6); write_param(0x13); write_param(0x02); // NVM Load Control
    write_cmd(0xD1); write_param(0x01);                    // Booster Enable
    write_cmd(0xC0); write_param(0x08); write_param(0x06); // Gate Voltage Setting

    write_cmd(0xC1); write_param(0x3C); write_param(0x3E); write_param(0x3C); write_param(0x3C); // VSHP
    write_cmd(0xC2); write_param(0x23); write_param(0x21); write_param(0x23); write_param(0x23); // VSLP
    write_cmd(0xC4); write_param(0x5A); write_param(0x5C); write_param(0x5A); write_param(0x5A); // VSHN
    write_cmd(0xC5); write_param(0x37); write_param(0x35); write_param(0x37); write_param(0x37); // VSLN

    write_cmd(0xB2); write_param(0x05); // Frame Rate Control

    write_cmd(0xB3);
    uint8_t b3[] = {0xE5,0xF6,0x17,0x77,0x77,0x77,0x77,0x77,0x77,0x71};
    write_data(b3, sizeof(b3));

    write_cmd(0xB4);
    uint8_t b4[] = {0x05,0x46,0x77,0x77,0x77,0x77,0x76,0x45};
    write_data(b4, sizeof(b4));

    write_cmd(0x62); uint8_t gtim[] = {0x32,0x03,0x1F}; write_data(gtim, sizeof(gtim));
    write_cmd(0xB7); write_param(0x13); // Source EQ Enable
    write_cmd(0xB0); write_param(0x60); // Gate Line Setting: 384 line

    write_cmd(0x11); vTaskDelay(pdMS_TO_TICKS(100)); // Sleep out

    write_cmd(0xC9); write_param(0x00); // Source Voltage Select
    write_cmd(0x36); write_param(0x00); // Memory Data Access Control
    write_cmd(0x3A); write_param(0x11); // Data Format Select: 3write/24bit
    write_cmd(0xB9); write_param(0x20); // Gamma Mode: Mono
    write_cmd(0xB8); write_param(0x29); // Panel Setting

    write_cmd(0x2A); write_param(0x17); write_param(0x24); // Column Address
    write_cmd(0x2B); write_param(0x00); write_param(0xBF); // Row Address

    write_cmd(0x35); write_param(0x00); // TE

    write_cmd(0xD0); write_param(0xFF); // Auto power down
    write_cmd(0x38);                    // Frame rate mode (HPM 高刷，扫描不易被看到)
    write_cmd(0x29);                    // Display on
    vTaskDelay(pdMS_TO_TICKS(100));

    HPM_MODE = true; LPM_MODE = false;
}

void ST7305_LCD::low_power_mode() {
    if(LPM_MODE){
        HPM_MODE = false;
        LPM_MODE = true;
    } else {
        HPM_MODE = false; 
        LPM_MODE = true;

        write_cmd(0xC1); write_param(115); write_param(0x3E); write_param(0x3C); write_param(0x3C);
        write_cmd(0xC2); write_param(0x00); write_param(0x21); write_param(0x23); write_param(0x23);
        write_cmd(0xC4); write_param(50);  write_param(0x5C); write_param(0x5A); write_param(0x5A);
        write_cmd(0xC5); write_param(50);  write_param(0x35); write_param(0x37); write_param(0x37);
        write_cmd(0xC9); write_param(0x00);

        vTaskDelay(pdMS_TO_TICKS(20));
        write_cmd(0x39); // LPM ON
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ST7305_LCD::high_power_mode() {
    if(HPM_MODE){
        HPM_MODE = true;
        LPM_MODE = false;
    } else {
        HPM_MODE = true; 
        LPM_MODE = false;

        write_cmd(0x38); // HPM ON
        vTaskDelay(pdMS_TO_TICKS(300));

        write_cmd(0xC1); write_param(115); write_param(0x3E); write_param(0x3C); write_param(0x3C);
        write_cmd(0xC2); write_param(0x00); write_param(0x21); write_param(0x23); write_param(0x23);
        write_cmd(0xC4); write_param(50);  write_param(0x5C); write_param(0x5A); write_param(0x5A);
        write_cmd(0xC5); write_param(50);  write_param(0x35); write_param(0x37); write_param(0x37);
        write_cmd(0xC9); write_param(0x00);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void ST7305_LCD::display_on(bool enabled) {
    write_cmd(enabled ? 0x29 : 0x28);
}

void ST7305_LCD::display_sleep(bool enabled){
    if (enabled){
        if (LPM_MODE){
            write_cmd(0x38); // back to HPM before sleep
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        write_cmd(0x10);
        vTaskDelay(pdMS_TO_TICKS(100));
    } else {
        write_cmd(0x11);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ST7305_LCD::display_inversion(bool enabled){
    write_cmd(enabled ? 0x21 : 0x20);
}

void ST7305_LCD::address() {
    write_cmd(0x2A); write_param(0x17); write_param(0x24); // 168 宽映射
    write_cmd(0x2B); write_param(0x00); write_param(0xBF); // 384 高映射
    write_cmd(0x2C); // memory write
}

void ST7305_LCD::display() {
    address();
    // 先拉 DC=1 表示后续是数据流
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t t = {};
    t.flags = 0;
    t.length = DISPLAY_BUFFER_LENGTH * 8;
    t.tx_buffer = display_buffer;
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi_dev_, &t));
    ESP_LOGI(TAG, "刷屏字节: 168x384, 每行%d字节, 共%d行, 整屏 %d 字节 (168*384/8=%d)",
             LCD_DATA_WIDTH, LCD_DATA_HIGH, DISPLAY_BUFFER_LENGTH, (168 * 384) / 8);
}

void ST7305_LCD::display_full_screen(const uint8_t* image) {
    if (!image) {
        return;
    }
    memcpy(display_buffer, image, DISPLAY_BUFFER_LENGTH); // 8064 字节
}

void ST7305_LCD::display_image_at(const uint8_t* src, int imgW, int imgH, int x, int y) {
    if (!src) { 
        return;
    }
    int src_stride_blocks = imgW / 4;
    int dst_block_x = x / 4;
    int dst_pair_y  = y / 2;

    int copy_blocks = std::min(src_stride_blocks, LCD_DATA_WIDTH - dst_block_x);
    int copy_pairs  = std::min(imgH/2, (LCD_HIGH/2) - dst_pair_y);
    if (copy_blocks <= 0 || copy_pairs <= 0) {
        return;
    }
    
    const uint8_t* src_row = src;
    for (int rp = 0; rp < copy_pairs; ++rp) {
        uint32_t dst_idx = (uint32_t)(dst_pair_y + rp) * LCD_DATA_WIDTH + dst_block_x;
        memcpy(&display_buffer[dst_idx], src_row, (size_t)copy_blocks);
        src_row += src_stride_blocks;
    }
}

void ST7305_LCD::fill_rect_at(int imgW, int imgH, int x, int y, bool is_black) {
    // 基本健壮性
    if (imgW <= 0 || imgH <= 0) {
        return;
    }

    int dst_block_x = x / 4;
    int dst_pair_y  = y / 2;

    // 期望填充的块数/行对数（来源是 imgW/imgH）
    int want_blocks = imgW / 4;
    int want_pairs  = imgH / 2;

    // 边界裁剪（与 display_image_at 同步的规则）
    int fill_blocks = std::min(want_blocks, LCD_DATA_WIDTH - dst_block_x);
    int fill_pairs  = std::min(want_pairs, (LCD_HIGH / 2) - dst_pair_y);

    if (fill_blocks <= 0 || fill_pairs <= 0) {
        return;
    }

    const uint8_t byte_val = is_black ? 0xFF : 0x00;

    for (int rp = 0; rp < fill_pairs; ++rp) {
        uint32_t dst_idx = (uint32_t)(dst_pair_y + rp) * LCD_DATA_WIDTH + dst_block_x;
        memset(&display_buffer[dst_idx], byte_val, (size_t)fill_blocks);
    }
}