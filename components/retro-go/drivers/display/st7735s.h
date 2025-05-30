#pragma once

#include "rg_display.h"
#include "rg_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

// ST7735S display dimensions
#define ST7735S_TFTWIDTH   160
#define ST7735S_TFTHEIGHT  128

// ST7735S Commands
#define ST7735_NOP     0x00 // No Operation
#define ST7735_SWRESET 0x01 // Software reset
#define ST7735_RDDID   0x04 // Read Display ID
#define ST7735_RDDST   0x09 // Read Display Status
#define ST7735_SLPIN   0x10 // Sleep in
#define ST7735_SLPOUT  0x11 // Sleep out
#define ST7735_PTLON   0x12 // Partial mode on
#define ST7735_NORON   0x13 // Partial off (normal)
#define ST7735_INVOFF  0x20 // Display inversion off
#define ST7735_INVON   0x21 // Display inversion on
#define ST7735_GAMSET  0x26 // Gamma curve select
#define ST7735_DISPOFF 0x28 // Display off
#define ST7735_DISPON  0x29 // Display on
#define ST7735_CASET   0x2A // Column address set
#define ST7735_RASET   0x2B // Row address set
#define ST7735_RAMWR   0x2C // Memory write
#define ST7735_RAMRD   0x2E // Memory read
#define ST7735_PTLAR   0x30 // Partial start/end address set
#define ST7735_TEOFF   0x34 // Tearing effect line off
#define ST7735_TEON    0x35 // Tearing effect line on
#define ST7735_MADCTL  0x36 // Memory data access control
#define ST7735_COLMOD  0x3A // Interface pixel format

#define MADCTL_MY  0x80 // Bottom to top
#define MADCTL_MX  0x40 // Right to left
#define MADCTL_MV  0x20 // Row/column exchange
#define MADCTL_ML  0x10 // LCD refresh Bottom to top
#define MADCTL_RGB 0x00 // Red-Green-Blue
#define MADCTL_BGR 0x08 // Blue-Green-Red
#define MADCTL_MH  0x04 // LCD refresh right to left

static spi_device_handle_t spi;
static SemaphoreHandle_t lcd_mutex;
static uint16_t *lcd_buffer;

// Forward declarations to fix implicit declaration warnings
void lcd_set_backlight(int percent);

static const unsigned char lcd_init_data[] = {
    2, ST7735_SLPOUT, 0x80,       // Exit sleep mode, delay 120ms
    1, ST7735_INVOFF,
    1, ST7735_MADCTL, MADCTL_RGB, // Set MADCTL: RGB color filter (default), no mirroring
    1, ST7735_COLMOD, 0x05,       // Set 16-bit color
    2, ST7735_DISPON, 0x80,       // Display on, delay 120ms
    0                             // End of list
};

static void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(RG_GPIO_LCD_DC, dc);
}

static void lcd_cmd(const uint8_t cmd)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    t.user = (void*)0;
    spi_device_polling_transmit(spi, &t);
}

static void lcd_data(const uint8_t *data, int len)
{
    if (len == 0) return;

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    t.user = (void*)1;
    spi_device_polling_transmit(spi, &t);
}

static void lcd_init_cmd_list(const unsigned char *list)
{
    xSemaphoreTake(lcd_mutex, portMAX_DELAY);

    while (*list) {
        uint8_t cmd = *list++;
        uint8_t len = *list++;
        uint8_t delay = len & 0x80;
        len &= 0x7F;

        lcd_cmd(cmd);
        if (len) {
            lcd_data((uint8_t*)list, len);
            list += len;
        }

        if (delay) {
            // Use FreeRTOS delay instead of rg_system_sleep
            vTaskDelay(pdMS_TO_TICKS(120));
        }
    }

    xSemaphoreGive(lcd_mutex);
}

static void lcd_set_window_internal(int left, int top, int width, int height)
{
    uint8_t data[4];

    // Column address set
    lcd_cmd(ST7735_CASET);
    data[0] = 0x00;
    data[1] = left;
    data[2] = 0x00;
    data[3] = left + width - 1;
    lcd_data(data, 4);

    // Row address set
    lcd_cmd(ST7735_RASET);
    data[0] = 0x00;
    data[1] = top;
    data[2] = 0x00;
    data[3] = top + height - 1;
    lcd_data(data, 4);

    // Memory write
    lcd_cmd(ST7735_RAMWR);
}

void lcd_init(void)
{
    lcd_mutex = xSemaphoreCreateMutex();

    spi_bus_config_t buscfg = {
        .miso_io_num = RG_GPIO_LCD_MISO,
        .mosi_io_num = RG_GPIO_LCD_MOSI,
        .sclk_io_num = RG_GPIO_LCD_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = RG_SCREEN_SPEED,
        .mode = 0,
        .spics_io_num = RG_GPIO_LCD_CS,
        .queue_size = 7,
        .pre_cb = lcd_spi_pre_transfer_callback,
    };

    // Initialize non-SPI GPIOs
    gpio_set_direction(RG_GPIO_LCD_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(RG_GPIO_LCD_RST, GPIO_MODE_OUTPUT);

    // Reset the display
    gpio_set_level(RG_GPIO_LCD_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100)); // Use FreeRTOS delay
    gpio_set_level(RG_GPIO_LCD_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100)); // Use FreeRTOS delay

    // Attach to the SPI bus
    spi_bus_initialize(RG_SCREEN_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(RG_SCREEN_HOST, &devcfg, &spi);

    // Initialize the LCD
    lcd_init_cmd_list(lcd_init_data);

    // Allocate memory for the pixel buffer
    lcd_buffer = heap_caps_malloc(16384, MALLOC_CAP_DMA);

    // Initial display settings
    lcd_set_backlight(RG_DISPLAY_BACKLIGHT_MAX);
}

void lcd_deinit(void)
{
    if (!spi) return;
    spi_bus_remove_device(spi);
    spi_bus_free(RG_SCREEN_HOST);
    vSemaphoreDelete(lcd_mutex);
    free(lcd_buffer);
    lcd_buffer = NULL;
    spi = NULL;
}

void lcd_set_window(int left, int top, int width, int height)
{
    xSemaphoreTake(lcd_mutex, portMAX_DELAY);
    lcd_set_window_internal(left, top, width, height);
    xSemaphoreGive(lcd_mutex);
}

void lcd_send_buffer(uint16_t *buffer, int count)
{
    if (count == 0) {
        // Buffer not used
        return;
    }

    xSemaphoreTake(lcd_mutex, portMAX_DELAY);
    lcd_data((uint8_t*)buffer, count * 2);
    xSemaphoreGive(lcd_mutex);
}

void lcd_sync(void)
{
    // Nothing to do
}

uint16_t *lcd_get_buffer(size_t size)
{
    return lcd_buffer;
}

void lcd_set_backlight(int percent)
{
    // For many ST7735S displays without dedicated backlight control
    // this function might be empty or implement GPIO control if available

    // If using a GPIO for backlight control (typical setup):
    // gpio_set_level(BACKLIGHT_GPIO_PIN, percent > 0 ? 1 : 0);
}
