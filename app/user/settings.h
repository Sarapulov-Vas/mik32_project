#ifndef __SETTINGS
#define __SETTINGS

#include "mik32_hal_adc.h"
#include "mik32_hal_ssd1306.h"
#include <inttypes.h>

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

/**
  * @brief Тип указателя на функцию применения настройки
  */
typedef void (*ApplySettingFunc)(Setting_HandleTypeDef *, HAL_SSD1306_HandleTypeDef *);

struct __Setting {
    char *Name;                   ///< Название настройки
    uint8_t Value;               ///< Текущее значение настройки
    uint8_t Max_Value;           ///< Максимальное допустимое значение
    uint8_t Min_Value;          ///< Минимальное допустимое значение
    ApplySettingFunc Apply_Setting; ///< Функция для применения настройки
};

/**
  * @brief Перечисление идентификаторов настроек
  */
enum {
    Distance = 0,               ///< Настройка расстояния
    Step = 1,                   ///< Настройка шага угла
    Min_Angle = 2,              ///< Настройка минимального угла
    Max_Angle = 3,              ///< Настройка максимального угла
    Radar_Viewing_Angle = 4,    ///< Настройка угла обзора радара
    Brightness = 5              ///< Настройка яркости дисплея
};

/**
  * @brief Инициализация настроек значениями по умолчанию
  * @param settings Указатель на массив настроек
  */
void Init_Default_Setting(Setting_HandleTypeDef *settings);

/**
  * @brief Отображение всех настроек на экране
  * @param scr Указатель на структуру управления дисплеем
  * @param settings Указатель на массив настроек
  */
void Display_Settings(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings);

/**
  * @brief Выделение выбранной настройки на экране
  * @param scr Указатель на структуру управления дисплеем
  * @param settings Указатель на массив настроек
  * @param num Номер настройки (индекс в массиве)
  */
void Select_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num);

/**
  * @brief Снятие выделения с настройки на экране
  * @param scr Указатель на структуру управления дисплеем
  * @param settings Указатель на массив настроек
  * @param num Номер настройки (индекс в массиве)
  */
void Deselect_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num);

/**
  * @brief Изменение значения настройки с помощью потенциометра
  * @param scr Указатель на структуру управления дисплеем
  * @param settings Указатель на массив настроек
  * @param hadc Указатель на структуру АЦП
  * @param potentiometr_chanel Номер канала АЦП для потенциометра
  * @param num Номер изменяемой настройки (индекс в массиве)
  */
void Change_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings,
                    ADC_HandleTypeDef *hadc, uint32_t potentiometr_chanel, int num);

/**
  * @brief Применение изменённой настройки
  * @param settings Указатель на массив настроек
  * @param scr Указатель на структуру управления дисплеем
  * @param num Номер применяемой настройки (индекс в массиве)
  */
void Apply_Setting(Setting_HandleTypeDef *settings, HAL_SSD1306_HandleTypeDef *scr, uint8_t num);

#endif