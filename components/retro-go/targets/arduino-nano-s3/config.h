// Target definition
#define RG_TARGET_NAME             "ARDUINO-NANO-ESP32"

// Storage
#define RG_STORAGE_DRIVER           1  // 0 = Host, 1 = SDSPI, 2 = SDMMC, 3 = USB, 4 = Flash
#define RG_STORAGE_SDSPI_HOST       SPI2_HOST
#define RG_STORAGE_SDSPI_SPEED      SDMMC_FREQ_DEFAULT
#define RG_STORAGE_ROOT             "/sd"
// #define RG_STORAGE_FLASH_PARTITION  "vfs"

// Audio
#define RG_AUDIO_USE_INT_DAC        0   // 0 = Disable, 1 = GPIO25, 2 = GPIO26, 3 = Both
#define RG_AUDIO_USE_EXT_DAC        1   // 0 = Disable, 1 = Enable

// Video - Adjusting for ST7735S (smaller display than ILI9341)
#define RG_SCREEN_DRIVER            1  // 0 = ILI9341, 1 = ST7735S
#define RG_SCREEN_HOST              SPI2_HOST
#define RG_SCREEN_SPEED             SPI_MASTER_FREQ_40M
#define RG_SCREEN_TYPE              0
// Adjust these values for your ST7735S display (likely 160x128 or similar)
#define RG_SCREEN_WIDTH             160
#define RG_SCREEN_HEIGHT            128
#define RG_SCREEN_ROTATE            0
#define RG_SCREEN_MARGIN_TOP        0
#define RG_SCREEN_MARGIN_BOTTOM     0
#define RG_SCREEN_MARGIN_LEFT       0
#define RG_SCREEN_MARGIN_RIGHT      0

// Input
// Refer to rg_input.h to see all available RG_KEY_* and RG_GAMEPAD_*_MAP types
#define RG_GAMEPAD_DRIVER           1  // 1 = GPIO, 2 = Serial, 3 = I2C
#define RG_GAMEPAD_HAS_MENU_BTN     1
#define RG_GAMEPAD_HAS_OPTION_BTN   0

// ADC joystick mapping
#define RG_GAMEPAD_ADC_MAP {\
    {RG_KEY_UP,    ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_11, 3072, 4096},\
    {RG_KEY_DOWN,  ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_11, 0, 1024},\
    {RG_KEY_RIGHT, ADC_UNIT_1, ADC_CHANNEL_1, ADC_ATTEN_DB_11, 3072, 4096},\
    {RG_KEY_LEFT,  ADC_UNIT_1, ADC_CHANNEL_1, ADC_ATTEN_DB_11, 0, 1024},\
}

// GPIO buttons mapping
#define RG_GAMEPAD_GPIO_MAP {\
    {RG_KEY_SELECT, GPIO_NUM_4, GPIO_PULLUP_ONLY, 0},  /* Joystick switch (D4)  */ \
    {RG_KEY_A,      GPIO_NUM_5, GPIO_PULLUP_ONLY, 0},  /* Button 1 (D5) */ \
    {RG_KEY_B,      GPIO_NUM_6, GPIO_PULLUP_ONLY, 0},  /* Button 2 (D6) */ \
}

// Battery
#define RG_BATTERY_DRIVER           1
#define RG_BATTERY_ADC_UNIT         ADC_UNIT_1
#define RG_BATTERY_ADC_CHANNEL      ADC_CHANNEL_3
#define RG_BATTERY_CALC_PERCENT(raw) (((raw) * 2.f - 3500.f) / (4200.f - 3500.f) * 100.f)
#define RG_BATTERY_CALC_VOLTAGE(raw) ((raw) * 2.f * 0.001f)

// Status LED - Could be set to GPIO_NUM_NC if no LED is connected
#define RG_GPIO_LED                 GPIO_NUM_NC

// SPI Display
#define RG_GPIO_LCD_MISO            GPIO_NUM_NC  // Often not used for many displays
#define RG_GPIO_LCD_MOSI            GPIO_NUM_11  // D11 on Arduino
#define RG_GPIO_LCD_CLK             GPIO_NUM_13  // D13 on Arduino
#define RG_GPIO_LCD_CS              GPIO_NUM_10  // D10 on Arduino
#define RG_GPIO_LCD_DC              GPIO_NUM_8   // D8 on Arduino (RS/DC)
#define RG_GPIO_LCD_RST             GPIO_NUM_9   // D9 on Arduino

// SPI SD Card - Share bus with display
#define RG_GPIO_SDSPI_MISO          GPIO_NUM_12  // D12 on Arduino
#define RG_GPIO_SDSPI_MOSI          GPIO_NUM_11  // Shared with LCD
#define RG_GPIO_SDSPI_CS            GPIO_NUM_7   // D7 on Arduino
#define RG_GPIO_SDSPI_CLK           GPIO_NUM_13  // Shared with LCD

// External I2S DAC
#define RG_GPIO_SND_I2S_BCK         38
#define RG_GPIO_SND_I2S_WS          13
#define RG_GPIO_SND_I2S_DATA        9
#define RG_GPIO_SND_AMP_ENABLE      18