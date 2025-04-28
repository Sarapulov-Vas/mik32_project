#include "radar.h"
#include <stdlib.h>
#include "mik32_hal_ssd1306_fonts.h"
#include "mik32_hal.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_irq.h"
#include "mik32_hal_scr1_timer.h"
#include "utilities.h"

Radar_HandleTypeDef radar = {0};

static void SystemClock_Config();
static void SPI_Init(SPI_HandleTypeDef *spi);
static void Drow_Radius_line(uint8_t angle, uint8_t radius, HAL_SSD1306_Color color);
static uint8_t Measure_Distance_And_Display();
static uint8_t Scale(uint16_t distance);
static void ADC_Init();
static void GPIO_Init();
static void Scan_Mode();
static void Manual_Mode();
static void Settings_Mode();
static void handle_button_click();

void Init_And_Run()
{
    HAL_Init();
    SystemClock_Config();
    Init_Default_Setting(radar.settings);
    SPI_Init(&radar.spi);
    ssd1306_Init(&radar.scr, radar.settings[Brightness].Value);
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
        ssd1306_Fill(&radar.scr, Black);
        switch (radar.mode)
        {
        case Scan:
            ssd1306_DrawCircle(&radar.scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
            ssd1306_SetCursor(&radar.scr, SSD1306_WIDTH - 4 * 6, 0);
            ssd1306_WriteString(&radar.scr, "Scan", Font_6x8, White);
            Scan_Mode();
            break;
        case Manual:
            ssd1306_DrawCircle(&radar.scr, RADAR_CENTER, RADAR_CENTER, RADAR_CENTER, White);
            ssd1306_SetCursor(&radar.scr, SSD1306_WIDTH - 6 * 6, 0);
            ssd1306_WriteString(&radar.scr, "Manual", Font_6x8, White);
            Manual_Mode();
            break;
        case Settings:
            Display_Settings(&radar.scr, radar.settings);
            Settings_Mode();
            break;
        default:
            break;
        }
    }
}

static void Scan_Mode()
{
    while (radar.mode == Scan)
    {
        if (radar.angle > radar.settings[Max_Angle].Value) radar.angle = radar.settings[Max_Angle].Value;
        if (radar.angle < radar.settings[Min_Angle].Value) radar.angle = radar.settings[Min_Angle].Value;

        if ((radar.angle == radar.settings[Max_Angle].Value && radar.direction == 0) ||
            (radar.angle == radar.settings[Min_Angle].Value && radar.direction == 1))
        {
            radar.direction = (radar.direction + 1) % 2;
        }

        int16_t ang = radar.direction == 0 ? radar.angle - radar.settings[Radar_Viewing_Angle].Value :
            radar.angle + radar.settings[Radar_Viewing_Angle].Value;
        if (ang >= radar.settings[Min_Angle].Value && ang <= radar.settings[Max_Angle].Value)
        {
            ssd1306_DrawPixel(&radar.scr, radar.radar_map[ang / radar.settings[Step].Value].x,
                radar.radar_map[ang / radar.settings[Step].Value].y, Black);
        }

        ssd1306_DrawPixel(&radar.scr, radar.radar_map[radar.angle / radar.settings[Step].Value].x,
            radar.radar_map[radar.angle / radar.settings[Step].Value].y, Black);

        if (Measure_Distance_And_Display() < radar.settings[Distance].Value)
        {
            ssd1306_DrawPixel(&radar.scr, radar.radar_map[radar.angle / radar.settings[Step].Value].x,
                radar.radar_map[radar.angle / radar.settings[Step].Value].y, White);
        }

        radar.angle = radar.direction == 0 ? radar.angle + radar.settings[Step].Value :
            radar.angle - radar.settings[Step].Value;
    }
}

static void Manual_Mode()
{
    while (radar.mode == Manual)
    {
        uint32_t value = Read_Potentiometr(&radar.hadc, radar.potentiometr_chanel);
        radar.angle = POT_TO_VALUE(value, radar.settings[Min_Angle].Value,
            radar.settings[Max_Angle].Value, radar.settings[Step].Value);
        Measure_Distance_And_Display();
    }
}

static void Settings_Mode()
{
    uint8_t previous_n = NUMBER_OF_SETTINGS;
    while (radar.mode == Settings)
    {
        if (radar.change_setting)
        {
            Change_Setting(&radar.scr, radar.settings, &radar.hadc, radar.potentiometr_chanel,
                radar.setting_number);
        }
        else
        {
            uint32_t value = Read_Potentiometr(&radar.hadc, radar.potentiometr_chanel);
            radar.setting_number = POT_TO_VALUE(value, 0, NUMBER_OF_SETTINGS - 1, 1);

            if (radar.setting_number != previous_n)
            {
                Deseclect_Setting(&radar.scr, radar.settings, previous_n);
                Seclect_Setting(&radar.scr, radar.settings, radar.setting_number);
                previous_n = radar.setting_number;
                ssd1306_UpdateScreen(&radar.scr);
            }
        }
    }
}

static uint8_t Measure_Distance_And_Display()
{
    Servo_Write(&radar.servo, radar.angle);
    char str[3];
    uint16_t dist = HC_SR04_ping_cm(&radar.HC_SR04);
    dist = dist > radar.settings[Distance].Value ? radar.settings[Distance].Value : dist;

    int_to_str(str, dist);
    ssd1306_FillRectangle(&radar.scr, 0, 0, 18, 8, Black);

    ssd1306_SetCursor(&radar.scr, 0, 0);
    HAL_EPIC_MaskLevelClear(HAL_EPIC_GPIO_IRQ_MASK);
    ssd1306_WriteString(&radar.scr, str, Font_6x8, White);

    Drow_Radius_line(radar.angle, Scale(dist), White);

    ssd1306_UpdateScreen(&radar.scr);
    HAL_EPIC_MaskLevelSet(HAL_EPIC_GPIO_IRQ_MASK);

    ssd1306_Line(&radar.scr, RADAR_CENTER, RADAR_CENTER,
        radar.radar_map[radar.angle / radar.settings[Step].Value].x,
        radar.radar_map[radar.angle / radar.settings[Step].Value].y, Black);

    return dist;
}

static uint8_t Scale(uint16_t distance)
{
    return distance * RADAR_DISTANCE / radar.settings[Distance].Value;
}

static void Drow_Radius_line(uint8_t angle, uint8_t radius, HAL_SSD1306_Color color)
{
    uint8_t x = angle > 90 ?
        RADAR_CENTER - floor(cos(DEG_TO_RAD(angle)) * radius):
        RADAR_CENTER + floor(cos(DEG_TO_RAD(180 - angle)) * radius);
    uint8_t y = RADAR_CENTER - floor(sin(DEG_TO_RAD(angle)) * radius);
    radar.radar_map[angle / radar.settings[Step].Value].x = x;
    radar.radar_map[angle / radar.settings[Step].Value].y = y;
    ssd1306_Line(&radar.scr, RADAR_CENTER, RADAR_CENTER, x, y, color);
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

    HAL_GPIO_InitInterruptLine(radar.Button_Line, GPIO_INT_MODE_CHANGE);
}

static void handle_button_click()
{
    uint64_t current_time = __HAL_SCR1_TIMER_GET_TIME();

    if ((current_time - radar.last_button_press_time) < DEBOUNCE_DELAY_MS * SYSTEM_FREQ_KHZ)
    {
        return;
    }

    radar.last_button_press_time = current_time;

    if (HAL_GPIO_ReadPin(radar.Button_Port, radar.Button_Pin) == GPIO_PIN_HIGH)
    {
        radar.button_press_time = current_time;
    }
    else
    {
        if (__HAL_SCR1_TIMER_GET_TIME() - radar.button_press_time > HOLD_DELAY_MS * SYSTEM_FREQ_KHZ)
        {
            radar.mode = radar.mode != Settings ? Settings : Scan;
        }
        else if (radar.mode != Settings)
        {
            radar.mode = (radar.mode + 1) % NUMBER_OF_MODES;
        }
        else
        {
            radar.change_setting = !radar.change_setting;
            if (!radar.change_setting)
            {
                Apply_Setting(radar.settings, &radar.scr, radar.setting_number);
            }
        }
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
