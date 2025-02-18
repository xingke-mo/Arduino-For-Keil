/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-18 15:14:30
 * @FilePath: \_Arduino-Keil-STM32\_Keilduino (STM32)\_Keilduino (STM32F10x)\USER\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Arduino.h"
#if 1
#define LED_Pin PC13
#define PWM_Pin PA0
#define ADC_Pin PA0

#define Write_Pin PA0
#define Read_Pin  PA1

#define KEY_Pin PA1

void LED_Toogle()
{
    Serial.println("KEY is pressed!");
    togglePin(LED_Pin);
}

#define LED1_Pin PA0
#define LED2_Pin PA1

void Timer1_Callback()
{
    Serial.println("Timer 1 Interrupt!");
    togglePin(LED1_Pin);
}

void Timer2_Callback()
{
    Serial.println("Timer 2 Interrupt!");
    togglePin(LED2_Pin);
}

void Serial_EventHandler()
{
    togglePin(LED_Pin);
}

void setup()
{
    Serial.begin(115200);
    Serial.printf("C++ Version: %d\r\n", __cplusplus);
    Serial.printf("Compiling Time: %s %s\r\n", __DATE__, __TIME__);
    pinMode(LED_Pin, OUTPUT);
    pinMode(PWM_Pin, PWM);

    //ADC_DMA
    Serial.begin(115200);
    pinMode(PA0, INPUT_ANALOG_DMA);
    pinMode(PA1, INPUT_ANALOG_DMA);
    pinMode(PA2, INPUT_ANALOG_DMA);
    pinMode(PA3, INPUT_ANALOG_DMA);
    pinMode(PA4, INPUT_ANALOG_DMA);
    pinMode(PA5, INPUT_ANALOG_DMA);
    pinMode(PA6, INPUT_ANALOG_DMA);
    pinMode(PA7, INPUT_ANALOG_DMA);

    pinMode(PB0, INPUT_ANALOG_DMA);
    pinMode(PB1, INPUT_ANALOG_DMA);

    pinMode(PC0, INPUT_ANALOG_DMA);
    pinMode(PC1, INPUT_ANALOG_DMA);
    pinMode(PC2, INPUT_ANALOG_DMA);
    pinMode(PC3, INPUT_ANALOG_DMA);
    pinMode(PC4, INPUT_ANALOG_DMA);
    pinMode(PC5, INPUT_ANALOG_DMA);
    ADC_DMA_Init();

    //EXTI
    Serial.println("KEY is pressed!");
    togglePin(LED_Pin);

    //GPIO_Fast
    pinMode(Write_Pin, OUTPUT);
    pinMode(Read_Pin, INPUT);

    //PWM
    /*PWM DutyCycle: 0~1000 -> 0~100%*/
    /*PWM Frequency: 10KHz*/
    PWM_Init(PWM_Pin, 1000, 10000);

    //Timer
    //Serial.println("Timer 1 Interrupt!");
    togglePin(LED1_Pin);

    //USART
    pinMode(LED_Pin, OUTPUT);
    Serial.begin(115200);
    Serial.println("Serial printing...");
    Serial.setTimeout(10);
    Serial.attachInterrupt(Serial_EventHandler);
}

void loop()
{
    Serial.println("Serial Test!");
    delay(2000);

    Serial.println("Digital Write TEST:");
    for(int i = 0; i < 5; i++)
    {
        digitalWrite(LED_Pin, HIGH);
        delay(1000);
        digitalWrite(LED_Pin, LOW);
        delay(1000);
    }

    Serial.println("Digital Read TEST:");
    pinMode(LED_Pin, INPUT);
    uint8_t State = digitalRead(LED_Pin);
    Serial.printf("LED_Pin State is:%d\r\n", State);
    pinMode(LED_Pin, OUTPUT);

    Serial.println("Analog Write TEST:");
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <= 1000; j++)
        {
            analogWrite(PWM_Pin, j);
            delay(1);
        }
        for(int j = 1000; j > 0; j--)
        {
            analogWrite(PWM_Pin, j);
            delay(1);
        }
    }

    Serial.println("ADC_Pin Analog Read TEST:");
    pinMode(ADC_Pin, INPUT_ANALOG);
    uint16_t ADC_Val = analogRead(ADC_Pin);
    Serial.printf("ADC_Pin ADC Val is:%d\r\n", ADC_Val);
    pinMode(PWM_Pin, PWM);

    //ADC_DMA
    Serial.printf(
        "PA0=%d,PA1=%d,PA2=%d,PA3=%d,PA4=%d,PA5=%d,PA6=%d,PA7=%d ",
        analogRead_DMA(PA0),
        analogRead_DMA(PA1),
        analogRead_DMA(PA2),
        analogRead_DMA(PA3),
        analogRead_DMA(PA4),
        analogRead_DMA(PA5),
        analogRead_DMA(PA6),
        analogRead_DMA(PA7)
    );

    Serial.printf(
        "-- PB0=%d,PB1=%d ",
        analogRead_DMA(PB0),
        analogRead_DMA(PB1)
    );

    Serial.printf(
        "-- PC0=%d,PC1=%d,PC2=%d,PC3=%d,PC4=%d,PC5=%d\r\n",
        analogRead_DMA(PC0),
        analogRead_DMA(PC1),
        analogRead_DMA(PC2),
        analogRead_DMA(PC3),
        analogRead_DMA(PC4),
        analogRead_DMA(PC5)
    );

    //GPIO_Fast
    int value = digitalRead_FAST(Read_Pin);

    digitalWrite_HIGH(Write_Pin);
    digitalWrite_LOW(Write_Pin);

    //PWM
    for(int i = 0; i <= 1000; i++)
    {
        pwmWrite(PWM_Pin, i);
        delay(1);
    }
    for(int i = 1000; i > 0; i--)
    {
        analogWrite(PWM_Pin, i);
        delay(1);
    }

    //USART
    if(Serial.available())
    {
        String receivedString = Serial.readString();
        Serial.printf("Serial received:%s\r\n", receivedString.c_str());
    }
}
#else
extern void setup() ;

extern void loop() ;
#endif
/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
//  SysClock_Init(F_CPU_128MHz);
    Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
