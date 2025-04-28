#ifndef __UTILITIES
#define __UTILITIES

#include "mik32_hal_adc.h"
#include <inttypes.h>

#define MIN_POTENCIOMETR_VALUE 200
#define MAX_POTENCIOMETR_VALUE 4000

#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)
#define POT_TO_VALUE(value, min_val, max_val, step)                                                \
    (value) <= (MIN_POTENCIOMETR_VALUE) ? (min_val)                                                \
    : (value) >= (MAX_POTENCIOMETR_VALUE)                                                          \
        ? (max_val)                                                                                \
        : ((((uint32_t)(value) - (MIN_POTENCIOMETR_VALUE)) * ((max_val) - (min_val)) /             \
            ((MAX_POTENCIOMETR_VALUE) - (MIN_POTENCIOMETR_VALUE))) /                               \
               (step) * (step) +                                                                   \
           (min_val))

void int_to_str(char *str, uint8_t n);
uint32_t Read_Potentiometr(ADC_HandleTypeDef *hadc, uint32_t potentiometr_chanel);

#endif