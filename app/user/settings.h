#ifndef __SETTINGS
#define __SETTINGS

#include <inttypes.h>
#include "mik32_hal_ssd1306.h"
#include "mik32_hal_adc.h"

#define NUMBER_OF_SETTINGS 6

#define MIN_DISTANCE 10
#define MAX_DISTANCE 250
#define MIN_STEP 1
#define MAX_STEP 45
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MIN_RADAR_VIEWING_ANGLE 0
#define MAX_RADAR_VIEWING_ANGLE 180
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 0xFF

#define DEFAULT_DISTANCE 50
#define DEFAULT_STEP 2
#define DEFAULT_MIN_ANGLE 0
#define DEFAULT_MAX_ANGLE 180
#define DEFAULT_RADAR_VIEWING_ANGLE 60
#define DEFAULT_BRIGHTNESS 0xFF

typedef struct __Setting Setting_HandleTypeDef;

typedef void (*ApplySettingFunc)(Setting_HandleTypeDef*, HAL_SSD1306_HandleTypeDef*);

struct __Setting
{
    char *Name;
    uint8_t Value;
    uint8_t Max_Value;
    uint8_t Min_Value;
    ApplySettingFunc Apply_Setting;
};

enum
{
    Distance = 0,
    Step = 1,
    Min_Angle = 2,
    Max_Angle = 3,
    Radar_Viewing_Angle = 4,
    Brightness = 5
};

void Init_Default_Setting(Setting_HandleTypeDef *settings);
void Display_Settings(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings);
void Seclect_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num);
void Deseclect_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num);
void Change_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, ADC_HandleTypeDef *hadc,
    uint32_t potentiometr_chanel, int num);
void Apply_Setting(Setting_HandleTypeDef *settings, HAL_SSD1306_HandleTypeDef *scr, uint8_t num);

#endif