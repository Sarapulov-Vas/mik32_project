#ifndef __RADAR
#define __RADAR

#include "HC-SR04.h"
#include "mik32_hal_adc.h"
#include "mik32_hal_spi.h"
#include "mik32_hal_ssd1306.h"
#include "servo.h"
#include "settings.h"
#include <math.h>

#define DEBOUNCE_DELAY_MS 100

#define SYSTEM_FREQ_HZ 32000000UL

#define SYSTEM_FREQ_KHZ 32000

#define HOLD_DELAY_MS 1000

#define RADAR_CENTER 63
#define RADAR_DISTANCE 61

#define NUMBER_OF_MODES 2

typedef enum { Scan = 0, Manual = 1, Settings } Modes;

typedef struct {
    SPI_HandleTypeDef spi;
    HAL_SSD1306_HandleTypeDef scr;
    Servo_HandleTypeDef servo;
    HC_SR04_HandleTypeDef HC_SR04;
    ADC_HandleTypeDef hadc;
    uint32_t potentiometr_chanel;
    GPIO_TypeDef *Button_Port;
    HAL_PinsTypeDef Button_Pin;
    HAL_GPIO_Line_Config Button_Mux_Line;
    HAL_GPIO_Line Button_Line;
    HAL_SSD1306_Vertex radar_map[180];
    int16_t angle;
    uint8_t direction;
    Modes mode;
    uint64_t last_button_press_time;
    uint64_t button_press_time;
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];
    uint8_t setting_number;
    uint8_t change_setting;
} Radar_HandleTypeDef;

extern Radar_HandleTypeDef radar;

void Init_And_Run();

#endif
