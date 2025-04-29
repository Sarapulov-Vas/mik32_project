#ifndef __HC_SR04
#define __HC_SR04

#include "mik32_hal_timer32.h"
#include <gpio.h>

#define DEFAULT_TIMEOUT 500000
#define SOUND_SPEED_CM_US 58.3f

/**
  * @brief Структура для управления датчиком HC-SR04
  */
typedef struct {
    GPIO_TypeDef *Echo_Port;      ///< Порт GPIO для сигнала Echo
    HAL_PinsTypeDef Echo_Pin;     ///< Номер пина для сигнала Echo
    GPIO_TypeDef *Trig_Port;      ///< Порт GPIO для сигнала Trig
    HAL_PinsTypeDef Trig_Pin;     ///< Номер пина для сигнала Trig
} HC_SR04_HandleTypeDef;

/**
  * @brief Инициализация датчика HC-SR04
  * @param HC_SR04 Указатель на структуру управления датчиком
  */
void HC_SR04_Init(HC_SR04_HandleTypeDef *HC_SR04);

/**
  * @brief Измерение расстояния с помощью датчика
  * @param HC_SR04 Указатель на структуру управления датчиком
  * @return Расстояние в сантиметрах
  */
uint32_t HC_SR04_ping_cm(HC_SR04_HandleTypeDef *HC_SR04);

#endif
