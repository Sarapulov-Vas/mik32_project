#include "settings.h"
#include "mik32_hal_ssd1306_fonts.h"
#include "utilities.h"

static void Apply_Min_Angle_Setting(Setting_HandleTypeDef *settings,
                                    HAL_SSD1306_HandleTypeDef *scr);
static void Apply_Max_Angle_Setting(Setting_HandleTypeDef *settings,
                                    HAL_SSD1306_HandleTypeDef *scr);
static void Apply_Brightness_Setting(Setting_HandleTypeDef *settings,
                                     HAL_SSD1306_HandleTypeDef *scr);

void Init_Default_Setting(Setting_HandleTypeDef *settings) {
    settings[Distance].Name = "Distance cm";
    settings[Distance].Value = DEFAULT_DISTANCE;
    settings[Distance].Min_Value = MIN_DISTANCE;
    settings[Distance].Max_Value = MAX_DISTANCE;
    settings[Distance].Apply_Setting = NULL;

    settings[Step].Name = "Step";
    settings[Step].Value = DEFAULT_STEP;
    settings[Step].Min_Value = MIN_STEP;
    settings[Step].Max_Value = MAX_STEP;
    settings[Step].Apply_Setting = NULL;

    settings[Min_Angle].Name = "Min angle";
    settings[Min_Angle].Value = DEFAULT_MIN_ANGLE;
    settings[Min_Angle].Min_Value = MIN_ANGLE;
    settings[Min_Angle].Max_Value = DEFAULT_MAX_ANGLE;
    settings[Min_Angle].Apply_Setting = Apply_Min_Angle_Setting;

    settings[Max_Angle].Name = "Max angle";
    settings[Max_Angle].Value = DEFAULT_MAX_ANGLE;
    settings[Max_Angle].Min_Value = DEFAULT_MIN_ANGLE;
    settings[Max_Angle].Max_Value = MAX_ANGLE;
    settings[Max_Angle].Apply_Setting = Apply_Max_Angle_Setting;

    settings[Radar_Viewing_Angle].Name = "Viewing angle";
    settings[Radar_Viewing_Angle].Value = DEFAULT_RADAR_VIEWING_ANGLE;
    settings[Radar_Viewing_Angle].Min_Value = MIN_RADAR_VIEWING_ANGLE;
    settings[Radar_Viewing_Angle].Max_Value = MAX_RADAR_VIEWING_ANGLE;
    settings[Radar_Viewing_Angle].Apply_Setting = NULL;

    settings[Brightness].Name = "Brightness";
    settings[Brightness].Value = DEFAULT_BRIGHTNESS;
    settings[Brightness].Min_Value = MIN_BRIGHTNESS;
    settings[Brightness].Max_Value = MAX_BRIGHTNESS;
    settings[Brightness].Apply_Setting = Apply_Brightness_Setting;
}

void Display_Settings(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings) {
    ssd1306_Fill(scr, Black);
    ssd1306_SetCursor(scr, 6 * 6, 0);
    ssd1306_WriteString(scr, "Settings", Font_6x8, White);
    char str[5];
    for (uint8_t i = 0; i < NUMBER_OF_SETTINGS; i++) {
        ssd1306_SetCursor(scr, 0, 9 + i * 9);
        ssd1306_WriteString(scr, settings[i].Name, Font_6x8, White);
        ssd1306_SetCursor(scr, SSD1306_WIDTH - 3 * 6, 9 + i * 9);
        int_to_str(str, settings[i].Value);
        ssd1306_WriteString(scr, str, Font_6x8, White);
    }
    ssd1306_UpdateScreen(scr);
}

void Seclect_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num) {
    char str[4];
    ssd1306_FillRectangle(scr, 0, 8 + 9 * num, SSD1306_WIDTH, 7 + 9 * (num + 1), White);
    ssd1306_SetCursor(scr, 0, 9 + 9 * num);
    ssd1306_WriteString(scr, settings[num].Name, Font_6x8, Black);
    int_to_str(str, settings[num].Value);
    ssd1306_SetCursor(scr, SSD1306_WIDTH - 3 * 6, 9 + num * 9);
    ssd1306_WriteString(scr, str, Font_6x8, Black);
}

void Deseclect_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings, int num) {
    char str[4];
    ssd1306_FillRectangle(scr, 0, 8 + 9 * num, SSD1306_WIDTH, 7 + 9 * (num + 1), Black);
    ssd1306_SetCursor(scr, 0, 9 + 9 * num);
    ssd1306_WriteString(scr, settings[num].Name, Font_6x8, White);
    int_to_str(str, settings[num].Value);
    ssd1306_SetCursor(scr, SSD1306_WIDTH - 3 * 6, 9 + num * 9);
    ssd1306_WriteString(scr, str, Font_6x8, White);
}

void Change_Setting(HAL_SSD1306_HandleTypeDef *scr, Setting_HandleTypeDef *settings,
                    ADC_HandleTypeDef *hadc, uint32_t potentiometr_chanel, int num) {
    char str[4];
    uint32_t value = Read_Potentiometr(hadc, potentiometr_chanel);
    settings[num].Value = POT_TO_VALUE(value, settings[num].Min_Value, settings[num].Max_Value, 1);
    int_to_str(str, settings[num].Value);
    ssd1306_FillRectangle(scr, SSD1306_WIDTH - 3 * 6, 8 + 9 * num, SSD1306_WIDTH, 7 + 9 * (num + 1),
                          White);
    ssd1306_SetCursor(scr, SSD1306_WIDTH - 3 * 6, 9 + num * 9);
    ssd1306_WriteString(scr, str, Font_6x8, Black);
    ssd1306_UpdateScreen(scr);
}

void Apply_Setting(Setting_HandleTypeDef *settings, HAL_SSD1306_HandleTypeDef *scr, uint8_t num) {
    if (settings[num].Apply_Setting != NULL) {
        settings[num].Apply_Setting(settings, scr);
    }
}

static void Apply_Min_Angle_Setting(Setting_HandleTypeDef *settings,
                                    HAL_SSD1306_HandleTypeDef *scr) {
    settings[Max_Angle].Min_Value = settings[Min_Angle].Value;
}

static void Apply_Max_Angle_Setting(Setting_HandleTypeDef *settings,
                                    HAL_SSD1306_HandleTypeDef *scr) {
    settings[Min_Angle].Max_Value = settings[Max_Angle].Value;
}

static void Apply_Brightness_Setting(Setting_HandleTypeDef *settings,
                                     HAL_SSD1306_HandleTypeDef *scr) {
    ssd1306_SetContrast(scr, settings[Brightness].Value);
}
