/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-18 16:13:54
 */
 
#include "Arduino.h"

/*
    Arduino STM32F103C8T6使用STM32的ADC输入和PWM输出
    利用STM32F103C8T6输出PWM引脚输出精度范围：0 ~65535，ADC引脚输入精度范围0 ~4095级
    STM32F103C8T6的PWM功能引脚:
    PA0 PA1 PA3 PA6 PA8 PA9 PA10 PB0 PB6 PB7

    STM32F103C8T6的ADC功能引脚:
    PA0 PA1 PA3 PA4 PA5 PA6 PA7 PB0 PB1

    利用map函数建立映射关系：map(analogInputVal，0，65535，0，4095)；进行映射关系转换。
*/

#define LED1_Pin PC0
#define LED2_Pin PC1

#define KEY_Pin  PB1
#define LED_Pin  PC2

#define Write_Pin PC3
#define Read_Pin  PB2

#define PWM_Pin   PA1

#define ADC_Pin   PA2

void Timer1_Callback()
{
    //Serial.println("Timer 1 Interrupt!");
    togglePin(LED1_Pin);
}

void Timer2_Callback()
{
    //Serial.println("Timer 2 Interrupt!");
    togglePin(LED2_Pin);
}

void LED_Toogle()
{
    //Serial.println("KEY is pressed!");
    togglePin(LED_Pin);
}

void setup()
{
    Serial.begin(19200);  //TX=PA9,RX=PA10

    pinMode(LED1_Pin, OUTPUT);
    pinMode(LED2_Pin, OUTPUT);
    
    pinMode(LED_Pin, OUTPUT);
    pinMode(KEY_Pin, INPUT_PULLUP);
    attachInterrupt(KEY_Pin, LED_Toogle, FALLING);
    
    Timer_SetInterrupt(TIM1, 100000 /*@100ms*/, Timer1_Callback);
    //Timer_SetInterrupt(TIM2, 100000 /*@100ms*/, Timer2_Callback);
    TIM_Cmd(TIM1, ENABLE);
    //TIM_Cmd(TIM2, ENABLE);
    
    pinMode(Write_Pin, OUTPUT);
    pinMode(Read_Pin, INPUT);    
    
    pinMode(ADC_Pin, INPUT_ANALOG);
    
    /*PWM DutyCycle: 0~500 -> 0~100%*/
    /*PWM Frequency: 1KHz*/
    pinMode(PWM_Pin, PWM);
    //PWM_Init(PWM_Pin, 500, 1000);  
    analogWrite(PWM_Pin, 500);

    Serial.println("Starting...");
}

void loop()
{
    int value_last = 1;
    
    int value = digitalRead_FAST(Read_Pin);

    if(value != value_last)
    {
        if(value)
        {
            digitalWrite_HIGH(Write_Pin);
        }
        else
        {
            digitalWrite_LOW(Write_Pin);    
            
            Serial.println("ADC_Pin Analog Read TEST:");
            uint16_t ADC_Val = analogRead(ADC_Pin);
            Serial.printf("ADC_Pin ADC Val is:%d\r\n", ADC_Val);            
            delay(200);            
        }
        
        value_last = value;
    }
#if 1    
    //指数式增加亮度，适合驱动LED，看起来亮度更线性
    for (int i = 0; i < 64; i++)
    {
        delay(20);
        analogWrite(PWM_Pin, i * i);
    }
#endif  
    togglePin(LED2_Pin);   
    delay(50);   
}
#if 1
/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
    SysClock_Init(F_CPU_24MHz);    
    Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
#endif
