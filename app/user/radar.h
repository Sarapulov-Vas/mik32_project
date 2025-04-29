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

/**
  * @brief Режимы работы радара
  */
typedef enum { Scan = 0, Manual = 1, Settings } Modes;

/**
  * @brief Основная структура управления радаром
  */
typedef struct {
    SPI_HandleTypeDef spi;                      ///< Структура SPI для дисплея
    HAL_SSD1306_HandleTypeDef scr;              ///< Структура управления дисплеем
    Servo_HandleTypeDef servo;                  ///< Структура управления сервоприводом
    HC_SR04_HandleTypeDef HC_SR04;              ///< Структура датчика расстояния
    ADC_HandleTypeDef hadc;                     ///< Структура АЦП для потенциометра
    uint32_t potentiometr_chanel;               ///< Канал АЦП для потенциометра
    GPIO_TypeDef *Button_Port;                  ///< Порт кнопки управления
    HAL_PinsTypeDef Button_Pin;                 ///< Пин кнопки управления
    HAL_GPIO_Line_Config Button_Mux_Line;       ///< Линия мультиплексора для кнопки
    HAL_GPIO_Line Button_Line;                  ///< Линия GPIO для кнопки
    HAL_SSD1306_Vertex radar_map[180];          ///< Массив точек для отображения радарной карты
    int16_t angle;                              ///< Текущий угол поворота сервопривода
    uint8_t direction;                          ///< Направление вращения (0/1)
    Modes mode;                                 ///< Текущий режим работы
    uint64_t last_button_press_time;            ///< Время последнего нажатия кнопки
    uint64_t button_press_time;                 ///< Время текущего нажатия кнопки
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS]; ///< Массив настроек системы
    uint8_t setting_number;                     ///< Текущая настройка для изменения
    uint8_t change_setting;                     ///< Флаг изменения настроек
} Radar_HandleTypeDef;

extern Radar_HandleTypeDef radar;

/**
  * @brief Основная функция инициализации и запуска радара
  */
void Init_And_Run();

#endif
