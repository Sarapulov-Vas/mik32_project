#include "mik32_hal_ssd1306_fonts.h"
#include "mock_mik32_hal_adc.h"
#include "mock_mik32_hal_ssd1306.h"
#include "mock_utilities.h"
#include "settings.h"
#include "unity.h"

void test_Init_Default_Setting_Should_InitializeAllSettingsCorrectly(void) {
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];

    Init_Default_Setting(settings);

    TEST_ASSERT_EQUAL_STRING("Distance cm", settings[Distance].Name);
    TEST_ASSERT_EQUAL(DEFAULT_DISTANCE, settings[Distance].Value);
    TEST_ASSERT_EQUAL(MIN_DISTANCE, settings[Distance].Min_Value);
    TEST_ASSERT_EQUAL(MAX_DISTANCE, settings[Distance].Max_Value);
    TEST_ASSERT_NULL(settings[Distance].Apply_Setting);

    TEST_ASSERT_EQUAL_STRING("Step", settings[Step].Name);
    TEST_ASSERT_EQUAL(DEFAULT_STEP, settings[Step].Value);
    TEST_ASSERT_EQUAL(MIN_STEP, settings[Step].Min_Value);
    TEST_ASSERT_EQUAL(MAX_STEP, settings[Step].Max_Value);
    TEST_ASSERT_NULL(settings[Step].Apply_Setting);

    TEST_ASSERT_EQUAL_STRING("Min angle", settings[Min_Angle].Name);
    TEST_ASSERT_EQUAL(DEFAULT_MIN_ANGLE, settings[Min_Angle].Value);
    TEST_ASSERT_EQUAL(MIN_ANGLE, settings[Min_Angle].Min_Value);
    TEST_ASSERT_EQUAL(DEFAULT_MAX_ANGLE, settings[Min_Angle].Max_Value);
    TEST_ASSERT_NOT_NULL(settings[Min_Angle].Apply_Setting);

    TEST_ASSERT_EQUAL_STRING("Max angle", settings[Max_Angle].Name);
    TEST_ASSERT_EQUAL(DEFAULT_MAX_ANGLE, settings[Max_Angle].Value);
    TEST_ASSERT_EQUAL(DEFAULT_MIN_ANGLE, settings[Max_Angle].Min_Value);
    TEST_ASSERT_EQUAL(MAX_ANGLE, settings[Max_Angle].Max_Value);
    TEST_ASSERT_NOT_NULL(settings[Max_Angle].Apply_Setting);

    TEST_ASSERT_EQUAL_STRING("Viewing angle", settings[Radar_Viewing_Angle].Name);
    TEST_ASSERT_EQUAL(DEFAULT_RADAR_VIEWING_ANGLE, settings[Radar_Viewing_Angle].Value);
    TEST_ASSERT_EQUAL(MIN_RADAR_VIEWING_ANGLE, settings[Radar_Viewing_Angle].Min_Value);
    TEST_ASSERT_EQUAL(MAX_RADAR_VIEWING_ANGLE, settings[Radar_Viewing_Angle].Max_Value);
    TEST_ASSERT_NULL(settings[Radar_Viewing_Angle].Apply_Setting);

    TEST_ASSERT_EQUAL_STRING("Brightness", settings[Brightness].Name);
    TEST_ASSERT_EQUAL(DEFAULT_BRIGHTNESS, settings[Brightness].Value);
    TEST_ASSERT_EQUAL(MIN_BRIGHTNESS, settings[Brightness].Min_Value);
    TEST_ASSERT_EQUAL(MAX_BRIGHTNESS, settings[Brightness].Max_Value);
    TEST_ASSERT_NOT_NULL(settings[Brightness].Apply_Setting);
}

// void test_Change_Setting_Should_UpdateSettingValueBasedOnPotentiometer(void) {
//     HAL_SSD1306_HandleTypeDef scr;
//     ADC_HandleTypeDef hadc;
//     Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];
//     const int setting_num = Max_Angle;
//     const uint32_t pot_channel = 1;
//     const uint32_t raw_pot_value = 2048;

//     Init_Default_Setting(settings);

//     const uint8_t expected_value =
//         settings[setting_num].Min_Value +
//         (raw_pot_value * (settings[setting_num].Max_Value - settings[setting_num].Min_Value)) /
//             4095;

//     Read_Potentiometr_ExpectAndReturn(&hadc, pot_channel, raw_pot_value);

//     int_to_str_Expect("90", expected_value);

//     ssd1306_FillRectangle_Expect(&scr, SSD1306_WIDTH - 3 * 6, 8 + 9 * setting_num, SSD1306_WIDTH,
//                                  7 + 9 * (setting_num + 1), White);

//     ssd1306_SetCursor_Expect(&scr, SSD1306_WIDTH - 3 * 6, 9 + setting_num * 9);

//     ssd1306_WriteString_ExpectAndReturn(&scr, "90", Font_6x8, Black, 0);

//     ssd1306_UpdateScreen_ExpectAndReturn(&scr, 0);

//     Change_Setting(&scr, settings, &hadc, pot_channel, setting_num);

//     TEST_ASSERT_EQUAL(expected_value, settings[setting_num].Value);

//     TEST_ASSERT_GREATER_OR_EQUAL(settings[setting_num].Min_Value, settings[setting_num].Value);
//     TEST_ASSERT_LESS_OR_EQUAL(settings[setting_num].Max_Value, settings[setting_num].Value);
// }

// void test_Change_Setting_Should_UpdateSettingValueBasedOnPotentiometer(void) {
//     HAL_SSD1306_HandleTypeDef scr;
//     ADC_HandleTypeDef hadc;
//     Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];
//     const int setting_num = Max_Angle;
//     const uint32_t pot_channel = 1;
//     const uint32_t raw_pot_value = 2048;

//     Init_Default_Setting(settings);

//     const uint8_t expected_value =
//         settings[setting_num].Min_Value +
//         (raw_pot_value * (settings[setting_num].Max_Value - settings[setting_num].Min_Value)) /
//             4095;

//     char expected_str[4];
//     snprintf(expected_str, sizeof(expected_str), "%d", expected_value);

//     Read_Potentiometr_ExpectAndReturn(&hadc, pot_channel, raw_pot_value);
//     int_to_str_Expect(expected_str, expected_value);

//     ssd1306_FillRectangle_Expect(&scr, SSD1306_WIDTH - 3 * 6, 8 + 9 * setting_num, SSD1306_WIDTH,
//                                  7 + 9 * (setting_num + 1), White);
//     ssd1306_SetCursor_Expect(&scr, SSD1306_WIDTH - 3 * 6, 9 + setting_num * 9);
//     ssd1306_WriteString_ExpectAndReturn(&scr, expected_str, Font_6x8, Black, 0);
//     ssd1306_UpdateScreen_ExpectAndReturn(&scr, 0);

//     Change_Setting(&scr, settings, &hadc, pot_channel, setting_num);

//     TEST_ASSERT_EQUAL(expected_value, settings[setting_num].Value);
// }

// void test_Change_Setting_Should_UpdateSettingValueBasedOnPotentiometer(void) {
//     HAL_SSD1306_HandleTypeDef scr;
//     ADC_HandleTypeDef hadc;
//     Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];
//     const int setting_num = Max_Angle;
//     const uint32_t pot_channel = 1;
//     const uint32_t raw_pot_value = 2048;

//     Init_Default_Setting(settings);

//     const uint8_t expected_value =
//         settings[setting_num].Min_Value +
//         (raw_pot_value * (settings[setting_num].Max_Value - settings[setting_num].Min_Value)) /
//             4095;

//     char expected_str[4];
//     int_to_str(expected_str, expected_value);

//     Read_Potentiometr_ExpectAndReturn(&hadc, pot_channel, raw_pot_value);

//     int_to_str_Expect(expected_str, expected_value);

//     ssd1306_FillRectangle_Expect(&scr, SSD1306_WIDTH - 3 * 6, 8 + 9 * setting_num, SSD1306_WIDTH,
//                                  7 + 9 * (setting_num + 1), White);
//     ssd1306_SetCursor_Expect(&scr, SSD1306_WIDTH - 3 * 6, 9 + setting_num * 9);
//     ssd1306_WriteString_Expect(&scr, expected_str, Font_6x8, Black);
//     ssd1306_UpdateScreen_Expect(&scr);

//     Change_Setting(&scr, settings, &hadc, pot_channel, setting_num);

//     TEST_ASSERT_EQUAL(expected_value, settings[setting_num].Value);
// }

void test_Change_Setting_Should_ProperlyUpdateSettingAndDisplay(void) {
    HAL_SSD1306_HandleTypeDef scr;
    ADC_HandleTypeDef hadc;
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];
    const int test_setting_num = Brightness;
    const uint32_t test_channel = 2;
    const uint32_t raw_pot_value = 3072;

    Init_Default_Setting(settings);

    const uint8_t expected_value = 192;

    Read_Potentiometr_ExpectAndReturn(&hadc, test_channel, raw_pot_value);

    int_to_str_Ignore();

    ssd1306_FillRectangle_Ignore();
    ssd1306_SetCursor_Ignore();
    ssd1306_WriteString_IgnoreAndReturn(0);
    ssd1306_UpdateScreen_IgnoreAndReturn(0);

    Change_Setting(&scr, settings, &hadc, test_channel, test_setting_num);

    TEST_ASSERT_EQUAL(expected_value, settings[test_setting_num].Value);
    TEST_ASSERT_GREATER_OR_EQUAL(settings[test_setting_num].Min_Value,
                                 settings[test_setting_num].Value);
    TEST_ASSERT_LESS_OR_EQUAL(settings[test_setting_num].Max_Value,
                              settings[test_setting_num].Value);
}

void test_Apply_Min_Angle_Setting_Should_UpdateMaxAngleMinValue(void) {
    HAL_SSD1306_HandleTypeDef scr;
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];

    Init_Default_Setting(settings);

    settings[Min_Angle].Value = 45;

    settings[Min_Angle].Apply_Setting(settings, &scr);

    TEST_ASSERT_EQUAL(45, settings[Max_Angle].Min_Value);
}

void test_Apply_Max_Angle_Setting_Should_UpdateMinAngleMaxValue(void) {
    HAL_SSD1306_HandleTypeDef scr;
    Setting_HandleTypeDef settings[NUMBER_OF_SETTINGS];

    Init_Default_Setting(settings);

    settings[Max_Angle].Value = 135;

    settings[Max_Angle].Apply_Setting(settings, &scr);

    TEST_ASSERT_EQUAL(135, settings[Min_Angle].Max_Value);
}
