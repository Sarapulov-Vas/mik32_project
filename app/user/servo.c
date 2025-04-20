#include "mik32_hal_timer32.h"
#include "servo.h"

static void Timer32_Init(Servo_HandleTypeDef *servo);
static void GPIO_Init();

void Servo_Init(Servo_HandleTypeDef *servo)
{
    GPIO_Init();
    Timer32_Init(servo);
    HAL_Timer32_Channel_Enable(&servo->htimer_channel);
    HAL_Timer32_Value_Clear(&servo->htim);
    Servo_Write(servo, 0);
    HAL_Timer32_Start(&servo->htim);
}

void Servo_Write(Servo_HandleTypeDef *servo, uint32_t angle)
{
    angle = (angle > 180) ? 180 : angle;
    uint32_t pulse_us = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180;
    uint32_t ocr_value = pulse_us * 32;

    HAL_Timer32_Channel_OCR_Set(&servo->htimer_channel, ocr_value);
    HAL_DelayMs(10);
}

static void Timer32_Init(Servo_HandleTypeDef *servo)
{
    servo->htim.Instance = servo->timer;
    servo->htim.Top = PWM_PERIOD_TICKS;
    servo->htim.State = TIMER32_STATE_DISABLE;
    servo->htim.Clock.Source = TIMER32_SOURCE_PRESCALER;
    servo->htim.Clock.Prescaler = 0;
    servo->htim.InterruptMask = 0;
    servo->htim.CountMode = TIMER32_COUNTMODE_FORWARD;
    HAL_Timer32_Init(&servo->htim);

    servo->htimer_channel.TimerInstance = servo->htim.Instance;
    servo->htimer_channel.ChannelIndex = servo->chanel;
    servo->htimer_channel.PWM_Invert = TIMER32_CHANNEL_NON_INVERTED_PWM;
    servo->htimer_channel.Mode = TIMER32_CHANNEL_MODE_PWM;
    servo->htimer_channel.CaptureEdge = TIMER32_CHANNEL_CAPTUREEDGE_RISING;
    servo->htimer_channel.OCR = SERVO_MIN_PULSE * 32;
    servo->htimer_channel.Noise = TIMER32_CHANNEL_FILTER_OFF;
    HAL_Timer32_Channel_Init(&servo->htimer_channel);
}

static void GPIO_Init()
{
    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
}

