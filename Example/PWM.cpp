/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-18 16:13:54
 */
 
#include "Arduino.h"

#define PWM_Pin   PA1

void setup()
{
    /*PWM DutyCycle: 0~1000 -> 0~100%*/
    /*PWM Frequency: 1KHz*/
    PWM_Init(PWM_Pin, 100, 1000);
}

void loop()
{
    for(int i = 0; i <= 500; i++)
    {
        pwmWrite(PWM_Pin, i);
        delay(1);
    }
    
    for(int i = 500; i > 0; i--)
    {
        analogWrite(PWM_Pin, i);
        delay(1);
    }
}
#if 1
/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    ///GPIO_JTAG_Disable();
    SysClock_Init(F_CPU_24MHz);       
    Delay_Init();
    setup();
    for(;;)loop();
}
#endif
