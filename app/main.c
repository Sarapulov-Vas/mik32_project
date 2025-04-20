#include <stdio.h>
#include "mik32_hal_usart.h"
#include "mik32_hal_spi.h"
#include "mik32_hal_ssd1306.h"
#include "mik32_hal_ssd1306_fonts.h"
#include "servo.h"
#include "HC-SR04.h"
#include "xprintf.h"

static void SystemClock_Config();
static void USART_Init();
static void SPI_Init();
static void Scr_Init();

USART_HandleTypeDef husart0;
SPI_HandleTypeDef spi;
HAL_SSD1306_HandleTypeDef scr;
Servo_HandleTypeDef servo;
HC_SR04_TypeDef HC_SR04;

int main()
{
    SystemClock_Config();
    USART_Init();
    SPI_Init();
    Scr_Init();
    servo.timer = TIMER32_1;
    servo.chanel = TIMER32_CHANNEL_0;
    Servo_Init(&servo);

    HC_SR04.Trig_Port = GPIO_0;
    HC_SR04.Trig_Pin = GPIO_PIN_8;
    HC_SR04.Echo_Port = GPIO_0;
    HC_SR04.Echo_Pin = GPIO_PIN_1;
    HC_SR04_Init(&HC_SR04);
    char str[5];

    while (1) {
        // xprintf("%d\n", HC_SR04_ping_cm(&HC_SR04));
        sprintf(str, "%ld", HC_SR04_ping_cm(&HC_SR04));
        ssd1306_SetCursor(&scr, 0, 0);
        ssd1306_WriteString(&scr, str, Font_6x8, White);
        ssd1306_UpdateScreen(&scr);
        HAL_DelayMs(500);
        ssd1306_Fill(&scr, Black);
        ssd1306_UpdateScreen(&scr);
    }
}

void SystemClock_Config(void)
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

void USART_Init()
{
    husart0.Instance = UART_0;
    husart0.transmitting = Enable;
    husart0.receiving = Enable;
    husart0.frame = Frame_8bit;
    husart0.parity_bit = Disable;
    husart0.parity_bit_inversion = Disable;
    husart0.bit_direction = LSB_First;
    husart0.data_inversion = Disable;
    husart0.tx_inversion = Disable;
    husart0.rx_inversion = Disable;
    husart0.swap = Disable;
    husart0.lbm = Disable;
    husart0.stop_bit = StopBit_1;
    husart0.mode = Asynchronous_Mode;
    husart0.xck_mode = XCK_Mode3;
    husart0.last_byte_clock = Disable;
    husart0.overwrite = Disable;
    husart0.rts_mode = AlwaysEnable_mode;
    husart0.dma_tx_request = Disable;
    husart0.dma_rx_request = Disable;
    husart0.channel_mode = Duplex_Mode;
    husart0.tx_break_mode = Disable;
    husart0.Interrupt.ctsie = Disable;
    husart0.Interrupt.eie = Disable;
    husart0.Interrupt.idleie = Disable;
    husart0.Interrupt.lbdie = Disable;
    husart0.Interrupt.peie = Disable;
    husart0.Interrupt.rxneie = Disable;
    husart0.Interrupt.tcie = Disable;
    husart0.Interrupt.txeie = Disable;
    husart0.Modem.rts = Disable; //out
    husart0.Modem.cts = Disable; //in
    husart0.Modem.dtr = Disable; //out
    husart0.Modem.dcd = Disable; //in
    husart0.Modem.dsr = Disable; //in
    husart0.Modem.ri = Disable;  //in
    husart0.Modem.ddis = Disable;//out
    husart0.baudrate = 115200;
    HAL_USART_Init(&husart0);
}


void SPI_Init() {
    spi.Instance = SPI_1;
    /* Режим SPI */
    spi.Init.SPI_Mode = HAL_SPI_MODE_MASTER;

    /* Настройки */
    spi.Init.CLKPhase = SPI_PHASE_OFF;
    spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi.Init.ThresholdTX = 1;

    /* Настройки для ведущего */
    spi.Init.BaudRateDiv = SPI_BAUDRATE_DIV8;
    spi.Init.Decoder = SPI_DECODER_NONE;
    spi.Init.ManualCS = SPI_MANUALCS_ON;
    spi.Init.ChipSelect = SPI_CS_0;
    if (HAL_SPI_Init(&spi) == HAL_OK)
    {
        HAL_USART_Print(&husart0, "SPI init OK\r\n", USART_TIMEOUT_DEFAULT);
    }
}

void Scr_Init()
{
    scr.Init.Interface = HAL_SPI;
    scr.Init.Spi = &spi;
    scr.Init.SSD1306_DC_Port = GPIO_1;
    scr.Init.SSD1306_DC_Pin = GPIO_PIN_8;
    scr.Init.SSD1306_Reset_Port = GPIO_1;
    scr.Init.SSD1306_Reset_Pin= GPIO_PIN_9;

    ssd1306_Init(&scr, BRIGHTNESS_FULL);
}
