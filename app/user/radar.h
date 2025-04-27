#ifndef __RADAR
#define __RADAR

#include <math.h>
#include "mik32_hal_spi.h"
#include "mik32_hal_ssd1306.h"
#include "mik32_hal_adc.h"
#include "servo.h"
#include "HC-SR04.h"

#define DEBOUNCE_DELAY_MS 100

#define SYSTEM_FREQ_HZ 32000000UL


#define SYSTEM_FREQ_KHZ 32000

#define RADAR_CENTER 63
#define RADAR_DISTANCE 61
#define MAX_DISTANCE 50
#define STEP 2
#define RADAR_VIEWING_ANGLE 60

#define MIN_POTENCIOMETR_VALUE 200
#define MAX_POTENCIOMETR_VALUE 4000

#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)
#define SCALE(dist) ((dist) * RADAR_DISTANCE / MAX_DISTANCE)

typedef struct
{
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
    HAL_SSD1306_Vertex radar_map[180 / STEP + 1];
    int16_t angle;
    uint8_t direction;
    uint8_t mode;
    uint64_t last_button_press_time;
} Radar_HandleTypeDef;

extern Radar_HandleTypeDef radar;

void Init_And_Run();

#endif
