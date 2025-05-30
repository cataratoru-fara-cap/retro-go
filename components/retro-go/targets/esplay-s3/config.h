// Target definition
#define RG_TARGET_NAME             "ESPLAY-S3"

// Storage
#define RG_STORAGE_ROOT             "/sd"
// #define RG_STORAGE_SDSPI_HOST       SPI2_HOST
// #define RG_STORAGE_SDSPI_SPEED      SDMMC_FREQ_DEFAULT
#define RG_STORAGE_SDMMC_HOST       SDMMC_HOST_SLOT_1
#define RG_STORAGE_SDMMC_SPEED      SDMMC_FREQ_DEFAULT
// #define RG_STORAGE_FLASH_PARTITION  "vfs"

// Audio
#define RG_AUDIO_USE_INT_DAC        0   // 0 = Disable, 1 = GPIO25, 2 = GPIO26, 3 = Both
#define RG_AUDIO_USE_EXT_DAC        0  // 0 = Disable, 1 = Enable

// Video
#define RG_SCREEN_DRIVER            0   // 0 = ILI9341
#define RG_SCREEN_HOST              SPI2_HOST
#define RG_SCREEN_SPEED             SPI_MASTER_FREQ_80M
#define RG_SCREEN_BACKLIGHT         1
#define RG_SCREEN_WIDTH             128
#define RG_SCREEN_HEIGHT            160
#define RG_SCREEN_ROTATE            0
#define RG_SCREEN_MARGIN_TOP        0
#define RG_SCREEN_MARGIN_BOTTOM     0
#define RG_SCREEN_MARGIN_LEFT       0
#define RG_SCREEN_MARGIN_RIGHT      0
#define RG_SCREEN_INIT() { \
    ILI9341_CMD(0x01); /* Software reset */ \
    ILI9341_DELAY(150); \
    ILI9341_CMD(0x11); /* Sleep out */ \
    ILI9341_DELAY(150); \
    ILI9341_CMD(0xB1, 0x05, 0x3A, 0x3A); /* Frame rate control 1 */ \
    ILI9341_CMD(0xB2, 0x05, 0x3A, 0x3A); /* Frame rate control 2 */ \
    ILI9341_CMD(0xB3, 0x05, 0x3A, 0x3A, 0x05, 0x3A, 0x3A); /* Frame rate control 3 */ \
    ILI9341_CMD(0xB4, 0x03); /* Display inversion control */ \
    ILI9341_CMD(0xC0, 0x28, 0x08, 0x04); /* Power control 1 */ \
    ILI9341_CMD(0xC1, 0xC0); /* Power control 2 */ \
    ILI9341_CMD(0xC5, 0x1E); /* VCOM control 1 */ \
    ILI9341_CMD(0x36, 0xC8); /* Memory Access Control (BGR, MY, MX) */ \
    ILI9341_CMD(0x3A, 0x05); /* Pixel Format Set (16-bit) */ \
    ILI9341_CMD(0xE0, 0x04, 0x22, 0x07, 0x0A, 0x2E, 0x30, 0x25, 0x2A, 0x28, 0x26, 0x2E, 0x3A, 0x00, 0x01, 0x03, 0x13); /* Positive Gamma Correction */ \
    ILI9341_CMD(0xE1, 0x04, 0x16, 0x06, 0x0D, 0x2D, 0x26, 0x23, 0x27, 0x27, 0x25, 0x2D, 0x3B, 0x00, 0x01, 0x04, 0x13); /* Negative Gamma Correction */ \
    ILI9341_CMD(0x29); /* Display ON */ \
}
// Input
// Refer to rg_input.h to see all available RG_KEY_* and RG_GAMEPAD_*_MAP types
#define RG_GAMEPAD_I2C_MAP {\
    {RG_KEY_UP,     (1<<2)},\
    {RG_KEY_RIGHT,  (1<<5)},\
    {RG_KEY_DOWN,   (1<<3)},\
    {RG_KEY_LEFT,   (1<<4)},\
    {RG_KEY_SELECT, (1<<1)},\
    {RG_KEY_START,  (1<<0)},\
    {RG_KEY_A,      (1<<6)},\
    {RG_KEY_B,      (1<<7)},\
}
#define RG_GAMEPAD_ADC_MAP {\
    {RG_KEY_UP,    ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_11, 3072, 4096},\
    {RG_KEY_DOWN,  ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_11, 0, 1024},\
    {RG_KEY_RIGHT, ADC_UNIT_1, ADC_CHANNEL_1, ADC_ATTEN_DB_11, 3072, 4096},\
    {RG_KEY_LEFT,  ADC_UNIT_1, ADC_CHANNEL_1, ADC_ATTEN_DB_11, 0, 1024},\
}

// GPIO buttons mapping
#define RG_GAMEPAD_GPIO_MAP {\
    {RG_KEY_SELECT, GPIO_NUM_7, GPIO_PULLUP_ONLY, 0},  /* Joystick switch (D4)  */ \
    {RG_KEY_A,      GPIO_NUM_8, GPIO_PULLUP_ONLY, 0},  /* Button 1 (D5) */ \
    {RG_KEY_B,      GPIO_NUM_9, GPIO_PULLUP_ONLY, 0},  /* Button 2 (D6) */ \
}

// Battery
#define RG_BATTERY_DRIVER           1
#define RG_BATTERY_ADC_UNIT         ADC_UNIT_1
#define RG_BATTERY_ADC_CHANNEL      ADC_CHANNEL_3
#define RG_BATTERY_CALC_PERCENT(raw) (((raw) * 2.f - 3500.f) / (4200.f - 3500.f) * 100.f)
#define RG_BATTERY_CALC_VOLTAGE(raw) ((raw) * 2.f * 0.001f)

// Status LED
// #define RG_GPIO_LED                 GPIO_NUM_2

// I2C BUS
// #define RG_GPIO_I2C_SDA             GPIO_NUM_10
// #define RG_GPIO_I2C_SCL             GPIO_NUM_11

// Status LED - Could be set to GPIO_NUM_NC if no LED is connected
#define RG_GPIO_LED                 GPIO_NUM_NC

// SPI Display
#define RG_GPIO_LCD_MISO            GPIO_NUM_NC  // Often not used for many displays
#define RG_GPIO_LCD_MOSI            GPIO_NUM_38  // D11 on Arduino
#define RG_GPIO_LCD_CLK             GPIO_NUM_48  // D13 on Arduino
#define RG_GPIO_LCD_CS              GPIO_NUM_21  // D10 on Arduino
#define RG_GPIO_LCD_DC              GPIO_NUM_17   // D8 on Arduino (RS/DC)
#define RG_GPIO_LCD_RST             GPIO_NUM_18   // D9 on Arduino

// SPI SD Card - Share bus with display
#define RG_GPIO_SDSPI_MISO          GPIO_NUM_47  // D12 on Arduino
#define RG_GPIO_SDSPI_MOSI          GPIO_NUM_38  // Shared with LCD
#define RG_GPIO_SDSPI_CS            GPIO_NUM_10   // D7 on Arduino
#define RG_GPIO_SDSPI_CLK           GPIO_NUM_48  // Shared with LCD

// External I2S DAC
// #define RG_GPIO_SND_I2S_BCK         38
// #define RG_GPIO_SND_I2S_WS          13
// #define RG_GPIO_SND_I2S_DATA        9
// #define RG_GPIO_SND_AMP_ENABLE      18
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define ILI9341_DELAY for millisecond-level delays
#define ILI9341_DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))