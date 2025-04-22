#ifndef __RADAR
#define __RADAR

#include <math.h>
#include "mik32_hal_spi.h"
#include "mik32_hal_ssd1306.h"
#include "mik32_hal_adc.h"
#include "servo.h"
#include "HC-SR04.h"

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
    HAL_SSD1306_Vertex radar_map[180 / STEP];
} Radar_HandleTypeDef;


void Init_Radar(Radar_HandleTypeDef *radar);
void Scan(Radar_HandleTypeDef *radar);
void Manual_Mode(Radar_HandleTypeDef *radar);

#endif
