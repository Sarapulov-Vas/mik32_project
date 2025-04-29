#ifndef __UTILITIES
#define __UTILITIES

#include "mik32_hal_adc.h"
#include <inttypes.h>

#define MIN_POTENCIOMETR_VALUE 200
#define MAX_POTENCIOMETR_VALUE 4000

/**
  * @brief Конвертирует градусы в радианы
  * @param angle Угол в градусах
  * @return Угол в радианах
  */
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)

/**
  * @brief Преобразует значение потенциометра в заданный диапазон с шагом
  * @param value Значение потенциометра
  * @param min_val Минимальное значение выходного диапазона
  * @param max_val Максимальное значение выходного диапазона
  * @param step Шаг квантования выходного значения
  * @return Значение в заданном диапазоне, кратное шагу
  */
#define POT_TO_VALUE(value, min_val, max_val, step)                                                \
    (value) <= (MIN_POTENCIOMETR_VALUE) ? (min_val)                                                \
    : (value) >= (MAX_POTENCIOMETR_VALUE)                                                          \
        ? (max_val)                                                                                \
        : ((((uint32_t)(value) - (MIN_POTENCIOMETR_VALUE)) * ((max_val) - (min_val)) /             \
            ((MAX_POTENCIOMETR_VALUE) - (MIN_POTENCIOMETR_VALUE))) /                               \
               (step) * (step) +                                                                   \
           (min_val))

           /**
  * @brief Конвертирует целое число в строку
  * @param str Указатель на буфер для результата (должен быть достаточного размера)
  * @param n Число для конвертации
  */
void int_to_str(char *str, uint8_t n);

/**
  * @brief Читает значение с потенциометра
  * @param hadc Указатель на структуру управления АЦП
  * @param potentiometr_chanel Номер канала АЦП, к которому подключен потенциометр
  */
uint32_t Read_Potentiometr(ADC_HandleTypeDef *hadc, uint32_t potentiometr_chanel);

#endif