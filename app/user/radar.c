#include "radar.h"
#include <stdlib.h>
#include <stdio.h>
#include "mik32_hal_ssd1306_fonts.h"
#include "mik32_hal.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_irq.h"
#include "mik32_hal_scr1_timer.h"

Radar_HandleTypeDef radar = {0};

static void SystemClock_Config();
static void SPI_Init(SPI_HandleTypeDef *spi);
static void int_to_str(char *str, uint8_t n);
static void Drow_Radius_line(uint8_t angle, uint8_t radius, HAL_SSD1306_Color color);
static uint8_t Measure_Distance_And_Display();
static uint32_t Read_Potentiometr();
static void ADC_Init();
static void GPIO_Init();
static void Scan();
static void Manual_Mode();
static void handle_button_click();

void Init_And_Run()
{
    HAL_Init();
    SystemClock_Config();
    SPI_Init(&radar.spi);
    ssd1306_Init(&radar.scr, BRIGHTNESS_FULL);
    Servo_Init(&radar.servo);
    HC_SR04_Init(&radar.HC_SR04);
    ADC_Init();
    GPIO_Init();

    ssd1306_DrawCircle(&radar.scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
    ssd1306_SetCursor(&radar.scr, SSD1306_WIDTH - 4 * 6, 0);
    ssd1306_WriteString(&radar.scr, "Scan", Font_6x8, White);

    HAL_EPIC_MaskLevelSet(HAL_EPIC_GPIO_IRQ_MASK);
    HAL_IRQ_EnableInterrupts();
    while (1)
    {
        switch (radar.mode)
        {
        case 0:
            Scan();
            break;
        case 1:
            Manual_Mode();
            break;
        default:
            break;
        }
    }
}

static void Scan()
{
    if (radar.angle > 180) radar.angle = 180;
    if (radar.angle < 0) radar.angle = 0;

    if ((radar.angle == 180 && radar.direction == 0) || (radar.angle == 0 && radar.direction == 1))
    {
        radar.direction = (radar.direction + 1) % 2;
    }

    uint8_t ang = radar.direction == 0 ? radar.angle - 60 : radar.angle + 60;
    if (ang >= 0 && ang <= 180)
    {
        ssd1306_DrawPixel(&radar.scr, radar.radar_map[ang / STEP].x, radar.radar_map[ang / STEP].y, Black);
    }

    ssd1306_DrawPixel(&radar.scr, radar.radar_map[radar.angle / STEP].x,
        radar.radar_map[radar.angle / STEP].y, Black);

    if (Measure_Distance_And_Display() < MAX_DISTANCE && radar.mode == 0)
    {
        ssd1306_DrawPixel(&radar.scr, radar.radar_map[radar.angle / STEP].x,
            radar.radar_map[radar.angle / STEP].y, White);
    }

    radar.angle = radar.direction == 0 ? radar.angle + STEP : radar.angle - STEP;
}

static void Manual_Mode()
{
    uint32_t value = Read_Potentiometr(radar);
    radar.angle = value < MIN_POTENCIOMETR_VALUE ? 0 : value > MAX_POTENCIOMETR_VALUE ? 180 :
        ((value - MIN_POTENCIOMETR_VALUE) * 180 / (MAX_POTENCIOMETR_VALUE - MIN_POTENCIOMETR_VALUE)) / STEP * STEP;
        Measure_Distance_And_Display();
}

static uint8_t Measure_Distance_And_Display()
{
    // Servo_Write(&radar.servo, radar.angle);
    char str[3];
    uint16_t dist = HC_SR04_ping_cm(&radar.HC_SR04);
    dist = dist > MAX_DISTANCE ? MAX_DISTANCE : dist;

    int_to_str(str, dist);
    ssd1306_FillRectangle(&radar.scr, 0, 0, 18, 8, Black);

    ssd1306_SetCursor(&radar.scr, 0, 0);
    ssd1306_WriteString(&radar.scr, str, Font_6x8, White);

    Drow_Radius_line(radar.angle, SCALE(dist), White);

    HAL_EPIC_MaskLevelClear(HAL_EPIC_GPIO_IRQ_MASK);
    ssd1306_UpdateScreen(&radar.scr);
    HAL_EPIC_MaskLevelSet(HAL_EPIC_GPIO_IRQ_MASK);

    ssd1306_Line(&radar.scr, RADAR_CENTER, RADAR_CENTER,
        radar.radar_map[radar.angle / STEP].x, radar.radar_map[radar.angle / STEP].y, Black);

    return dist;
}

static void Drow_Radius_line(uint8_t angle, uint8_t radius, HAL_SSD1306_Color color)
{
    uint8_t x = angle > 90 ?
        RADAR_CENTER - floor(cos(DEG_TO_RAD(angle)) * radius):
        RADAR_CENTER + floor(cos(DEG_TO_RAD(180 - angle)) * radius);
    uint8_t y = RADAR_CENTER - floor(sin(DEG_TO_RAD(angle)) * radius);
    radar.radar_map[angle / STEP].x = x;
    radar.radar_map[angle / STEP].y = y;
    ssd1306_Line(&radar.scr, RADAR_CENTER, RADAR_CENTER, x, y, color);
}

static uint32_t Read_Potentiometr()
{
    HAL_ADC_SINGLE_AND_SET_CH(radar.hadc.Instance, radar.potentiometr_chanel);
    return HAL_ADC_WaitAndGetValue(&radar.hadc);
}

static void int_to_str(char *str, uint8_t n)
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
    spi->Init.SPI_Mode = HAL_SPI_MODE_MASTER;

    spi->Init.CLKPhase = SPI_PHASE_OFF;
    spi->Init.CLKPolarity = SPI_POLARITY_LOW;
    spi->Init.ThresholdTX = 1;

    spi->Init.BaudRateDiv = SPI_BAUDRATE_DIV8;
    spi->Init.Decoder = SPI_DECODER_NONE;
    spi->Init.ManualCS = SPI_MANUALCS_ON;
    HAL_SPI_Init(spi);
}

static void ADC_Init()
{
    radar.hadc.Instance = ANALOG_REG;
    radar.hadc.Init.EXTRef = ADC_EXTREF_OFF;
    radar.hadc.Init.EXTClb = ADC_EXTCLB_CLBREF;
    radar.hadc.Init.Sel = radar.potentiometr_chanel;
    HAL_ADC_Init(&radar.hadc);
}

static void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

    GPIO_InitStruct.Pin = radar.Button_Pin;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_INPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_DOWN;
    HAL_GPIO_Init(radar.Button_Port, &GPIO_InitStruct);

    HAL_GPIO_InitInterruptLine(radar.Button_Line, GPIO_INT_MODE_RISING);
}

static void handle_button_click()
{
    uint64_t current_time = __HAL_SCR1_TIMER_GET_TIME();

    if ((current_time - radar.last_button_press_time) / SYSTEM_FREQ_KHZ < DEBOUNCE_DELAY_MS)
    {
        return;
    }

    radar.last_button_press_time = current_time;
    radar.mode = (radar.mode + 1) % 2;
    ssd1306_Fill(&radar.scr, Black);
    if (radar.mode == 0) {
        ssd1306_DrawCircle(&radar.scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
        ssd1306_SetCursor(&radar.scr, SSD1306_WIDTH - 4 * 6, 0);
        ssd1306_WriteString(&radar.scr, "Scan", Font_6x8, White);
    } else {
        ssd1306_DrawCircle(&radar.scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
        ssd1306_SetCursor(&radar.scr, SSD1306_WIDTH - 6 * 6, 0);
        ssd1306_WriteString(&radar.scr, "Manual", Font_6x8, White);
    }
}

void trap_handler()
{
    if (EPIC_CHECK_GPIO_IRQ() && HAL_GPIO_LineInterruptState(radar.Button_Line))
    {
        handle_button_click();
        HAL_GPIO_ClearInterrupts();
        HAL_EPIC_Clear(EPIC_LINE_GPIO_IRQ_S);
    }
}
