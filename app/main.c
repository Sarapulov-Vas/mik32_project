#include "radar.h"

static Radar_HandleTypeDef radar = {0};

int main()
{
    radar.servo.timer = TIMER32_1;
    radar.servo.chanel = TIMER32_CHANNEL_0;

    radar.HC_SR04.Trig_Port = GPIO_0;
    radar.HC_SR04.Trig_Pin = GPIO_PIN_8;
    radar.HC_SR04.Echo_Port = GPIO_0;
    radar.HC_SR04.Echo_Pin = GPIO_PIN_1;

    radar.spi.Instance = SPI_1;
    radar.spi.Init.ManualCS = SPI_MANUALCS_ON;
    radar.spi.Init.ChipSelect = SPI_CS_0;

    radar.scr.Init.Interface = HAL_SPI;
    radar.scr.Init.Spi = &radar.spi;
    radar.scr.Init.SSD1306_DC_Port = GPIO_1;
    radar.scr.Init.SSD1306_DC_Pin = GPIO_PIN_8;
    radar.scr.Init.SSD1306_Reset_Port = GPIO_1;
    radar.scr.Init.SSD1306_Reset_Pin= GPIO_PIN_9;

    radar.potentiometr_chanel = 1;

    Init_Radar(&radar);
    Scan(&radar);
    // Manual_Mode(&radar);
}
