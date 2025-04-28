#include "utilities.h"
#include <inttypes.h>

void int_to_str(char *str, uint8_t n)
{
    char buffer[5];
    int i = 0;

    do {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    } while (n > 0);

    int len = i;
    for (int i = 0; i < len; i++)
    {
	    str[i] = buffer[len - 1 - i];
    }

    str[len] = '\0';
}

uint32_t Read_Potentiometr(ADC_HandleTypeDef *hadc, uint32_t potentiometr_chanel)
{
    HAL_ADC_SINGLE_AND_SET_CH(hadc->Instance, potentiometr_chanel);
    return HAL_ADC_WaitAndGetValue(hadc);
}
