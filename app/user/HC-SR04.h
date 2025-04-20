#ifndef __HC_SR04
#define __HC_SR04

#include "mik32_hal_timer32.h"
#include <gpio.h>

#define DEFAULT_TIMEOUT 500000
#define SOUND_SPEED_CM_US 58.3f

typedef struct {
    GPIO_TypeDef *Echo_Port;
    HAL_PinsTypeDef Echo_Pin;
    GPIO_TypeDef *Trig_Port;
    HAL_PinsTypeDef Trig_Pin;
} HC_SR04_TypeDef;

void HC_SR04_Init(HC_SR04_TypeDef *HC_SR04);
uint32_t HC_SR04_ping_cm(HC_SR04_TypeDef *HC_SR04);

#endif
