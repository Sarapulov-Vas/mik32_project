#ifndef __SERVO
#define __SERVO

#include "mik32_hal_timer32.h"
#include <gpio.h>

#define SERVO_MIN_PULSE 500  // 0
#define SERVO_MAX_PULSE 2450 // 180
#define SERVO_PWM_FREQ 50

#define SYSTEM_FREQ_HZ 32000000UL
#define PWM_PERIOD_TICKS (SYSTEM_FREQ_HZ / SERVO_PWM_FREQ)

/**
  * @brief Структура управления сервоприводом
  */
typedef struct {
    TIMER32_HandleTypeDef htim;
    TIMER32_CHANNEL_HandleTypeDef htimer_channel;
    TIMER32_TypeDef *timer;
    HAL_TIMER32_CHANNEL_IndexTypeDef chanel;

} Servo_HandleTypeDef;

/**
  * @brief Инициализация сервопривода
  * @param servo Указатель на структуру управления сервоприводом
  */
void Servo_Init(Servo_HandleTypeDef *servo);

/**
  * @brief Установка угла поворота сервопривода
  * @param servo Указатель на структуру управления сервоприводом
  * @param angle Угол поворота (0-180 градусов)
  */
void Servo_Write(Servo_HandleTypeDef *servo, uint32_t angle);

#endif
