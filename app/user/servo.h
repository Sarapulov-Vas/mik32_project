#ifndef __SERVO
#define __SERVO

#include "mik32_hal_timer32.h"
#include <gpio.h>

#define SERVO_MIN_PULSE  500   // 0
#define SERVO_MAX_PULSE  2450  // 180
#define SERVO_PWM_FREQ   50

#define SYSTEM_FREQ_HZ 32000000UL
#define PWM_PERIOD_TICKS (SYSTEM_FREQ_HZ / SERVO_PWM_FREQ)

typedef struct {
    TIMER32_HandleTypeDef htim;
    TIMER32_CHANNEL_HandleTypeDef htimer_channel;
    TIMER32_TypeDef *timer;
    HAL_TIMER32_CHANNEL_IndexTypeDef chanel;
    
} Servo_HandleTypeDef;

void Servo_Init(Servo_HandleTypeDef *servo);
void Servo_Write(Servo_HandleTypeDef *servo, uint32_t angle);

#endif
