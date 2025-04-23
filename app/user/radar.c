#include "radar.h"
#include <stdlib.h>
#include "mik32_hal_ssd1306_fonts.h"

static void SystemClock_Config();
static void SPI_Init(SPI_HandleTypeDef *spi);
static void int_to_str(char *str, uint16_t n);
static void Drow_Radius_line(Radar_HandleTypeDef *radar, uint8_t angle, uint8_t radius, HAL_SSD1306_Color color);
static void Measure_Distance_And_Display(Radar_HandleTypeDef *radar, uint8_t angle);
static uint32_t Read_Potentiometr(Radar_HandleTypeDef *radar);
static void ADC_Init(Radar_HandleTypeDef *radar);


void Init_Radar(Radar_HandleTypeDef *radar)
{
    SystemClock_Config();
    SPI_Init(&radar->spi);
    ssd1306_Init(&radar->scr, BRIGHTNESS_FULL);
    Servo_Init(&radar->servo);
    HC_SR04_Init(&radar->HC_SR04);
    ADC_Init(radar);
}

void Scan(Radar_HandleTypeDef *radar)
{
    ssd1306_DrawCircle(&radar->scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
    ssd1306_SetCursor(&radar->scr, SSD1306_WIDTH - 4 * 6, 0);
    ssd1306_WriteString(&radar->scr, "Scan", Font_6x8, White);
    ssd1306_UpdateScreen(&radar->scr);
    uint8_t direction = 1;
    while (1)
    {
        direction = (direction + 1) % 2;
        for (int16_t angle = direction == 0 ? 0 : 180;
            direction == 0 ? angle <= 180 : angle >= 0;
            angle = direction == 0 ? angle + STEP : angle - STEP)
        {            
            if (angle >= 60 || angle <= 120)
            {
                uint8_t ang = direction == 0 ? angle - 60 : angle +60;
                ssd1306_DrawPixel(&radar->scr, radar->radar_map[ang / STEP].x, radar->radar_map[ang / STEP].y, Black);
            }
            
            ssd1306_DrawPixel(&radar->scr, radar->radar_map[angle / STEP].x,
                radar->radar_map[angle / STEP].y, Black);

            Measure_Distance_And_Display(radar, angle);

            ssd1306_DrawPixel(&radar->scr, radar->radar_map[angle / STEP].x,
                radar->radar_map[angle / STEP].y, White);

        }
    }
}

void Manual_Mode(Radar_HandleTypeDef *radar)
{
    ssd1306_DrawCircle(&radar->scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
    ssd1306_SetCursor(&radar->scr, SSD1306_WIDTH - 6 * 6, 0);
    ssd1306_WriteString(&radar->scr, "Manual", Font_6x8, White);
    ssd1306_UpdateScreen(&radar->scr);
    while (1)
    {
        uint32_t value = Read_Potentiometr(radar);
        uint8_t angle = value < MIN_POTENCIOMETR_VALUE ? 0 : value > MAX_POTENCIOMETR_VALUE ? 180 :
            ((value - MIN_POTENCIOMETR_VALUE) * 180 / (MAX_POTENCIOMETR_VALUE - MIN_POTENCIOMETR_VALUE)) / STEP * STEP; 
            Measure_Distance_And_Display(radar, angle);
    }
}

static void Measure_Distance_And_Display(Radar_HandleTypeDef *radar, uint8_t angle)
{
    // Servo_Write(&radar->servo, angle);
    char str[3];
    uint16_t dist = HC_SR04_ping_cm(&radar->HC_SR04);
    dist = dist > MAX_DISTANCE ? MAX_DISTANCE : dist;

    int_to_str(str, dist);
    if (dist < 10)
    {
        ssd1306_FillRectangle(&radar->scr, 6, 0, 12, 8, Black);
    }

    ssd1306_SetCursor(&radar->scr, 0, 0);
    ssd1306_WriteString(&radar->scr, str, Font_6x8, White);

    Drow_Radius_line(radar, angle, SCALE(dist), White);
    
    ssd1306_UpdateScreen(&radar->scr);
    ssd1306_Line(&radar->scr, RADAR_CENTER, RADAR_CENTER,
        radar->radar_map[angle / STEP].x, radar->radar_map[angle / STEP].y, Black);
}

static void Drow_Radius_line(Radar_HandleTypeDef *radar, uint8_t angle, uint8_t radius, HAL_SSD1306_Color color)
{
    uint8_t x = angle > 90 ?
        RADAR_CENTER - floor(cos(DEG_TO_RAD(angle)) * radius):
        RADAR_CENTER + floor(cos(DEG_TO_RAD(180 - angle)) * radius);
    uint8_t y = RADAR_CENTER - floor(sin(DEG_TO_RAD(angle)) * radius);
    radar->radar_map[angle / STEP].x = x;
    radar->radar_map[angle / STEP].y = y;
    ssd1306_Line(&radar->scr, RADAR_CENTER, RADAR_CENTER, x, y, color);
}

static uint32_t Read_Potentiometr(Radar_HandleTypeDef *radar)
{
    HAL_ADC_SINGLE_AND_SET_CH(radar->hadc.Instance, radar->potentiometr_chanel);
    return HAL_ADC_WaitAndGetValue(&radar->hadc);
}

static void int_to_str(char *str, uint16_t n)
{
    char buffer[3];
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

static void SystemClock_Config()
{
    PCC_InitTypeDef PCC_OscInit = {0};

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider = 0;
    PCC_OscInit.APBMDivider = 0;
    PCC_OscInit.APBPDivider = 0;
    PCC_OscInit.HSI32MCalibrationValue = 128;
    PCC_OscInit.LSI32KCalibrationValue = 8;
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;

    HAL_PCC_Config(&PCC_OscInit);
}

static void SPI_Init(SPI_HandleTypeDef *spi)
{
    /* Режим SPI */
    spi->Init.SPI_Mode = HAL_SPI_MODE_MASTER;

    /* Настройки */
    spi->Init.CLKPhase = SPI_PHASE_OFF;
    spi->Init.CLKPolarity = SPI_POLARITY_LOW;
    spi->Init.ThresholdTX = 1;

    /* Настройки для ведущего */
    spi->Init.BaudRateDiv = SPI_BAUDRATE_DIV8;
    spi->Init.Decoder = SPI_DECODER_NONE;
    spi->Init.ManualCS = SPI_MANUALCS_ON;
    HAL_SPI_Init(spi);
}

static void ADC_Init(Radar_HandleTypeDef *radar)
{
    radar->hadc.Instance = ANALOG_REG;
    radar->hadc.Init.EXTRef = ADC_EXTREF_OFF;
    radar->hadc.Init.EXTClb = ADC_EXTCLB_CLBREF;
    radar->hadc.Init.Sel = radar->potentiometr_chanel;
    HAL_ADC_Init(&radar->hadc);
}
