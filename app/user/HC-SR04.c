#include "HC-SR04.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_scr1_timer.h"

void HC_SR04_Init(HC_SR04_HandleTypeDef *HC_SR04) {
    GPIO_InitTypeDef GPIO_InitStruct_Trig = {0};
    GPIO_InitTypeDef GPIO_InitStruct_Echo = {0};

    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

    GPIO_InitStruct_Trig.Pin = HC_SR04->Trig_Pin;
    GPIO_InitStruct_Trig.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct_Trig.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(HC_SR04->Trig_Port, &GPIO_InitStruct_Trig);

    GPIO_InitStruct_Echo.Pin = HC_SR04->Echo_Pin;
    GPIO_InitStruct_Echo.Mode = HAL_GPIO_MODE_GPIO_INPUT;
    GPIO_InitStruct_Echo.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(HC_SR04->Echo_Port, &GPIO_InitStruct_Echo);

    HAL_GPIO_WritePin(HC_SR04->Trig_Port, HC_SR04->Trig_Pin, GPIO_PIN_LOW);
}

static uint32_t pulseIn(HC_SR04_HandleTypeDef *HC_SR04) {
    uint64_t start = __HAL_SCR1_TIMER_GET_TIME();
    while (HAL_GPIO_ReadPin(HC_SR04->Echo_Port, HC_SR04->Echo_Pin) == GPIO_PIN_HIGH &&
           (__HAL_SCR1_TIMER_GET_TIME() - start) < DEFAULT_TIMEOUT) {
    }

    start = __HAL_SCR1_TIMER_GET_TIME();
    while (HAL_GPIO_ReadPin(HC_SR04->Echo_Port, HC_SR04->Echo_Pin) != GPIO_PIN_HIGH &&
           (__HAL_SCR1_TIMER_GET_TIME() - start) < DEFAULT_TIMEOUT) {
    }

    start = __HAL_SCR1_TIMER_GET_TIME();

    while (HAL_GPIO_ReadPin(HC_SR04->Echo_Port, HC_SR04->Echo_Pin) == GPIO_PIN_HIGH &&
           (__HAL_SCR1_TIMER_GET_TIME() - start) < DEFAULT_TIMEOUT) {
    }

    return (__HAL_SCR1_TIMER_GET_TIME() - start) / 32;
}

uint32_t HC_SR04_ping_cm(HC_SR04_HandleTypeDef *HC_SR04) {
    HAL_GPIO_WritePin(HC_SR04->Trig_Port, HC_SR04->Trig_Pin, GPIO_PIN_HIGH);
    HAL_DelayUs(50);
    HAL_GPIO_WritePin(HC_SR04->Trig_Port, HC_SR04->Trig_Pin, GPIO_PIN_LOW);

    uint32_t us = pulseIn(HC_SR04);
    return (us / SOUND_SPEED_CM_US);
}
