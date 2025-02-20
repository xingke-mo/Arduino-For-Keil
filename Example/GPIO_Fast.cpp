/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-18 16:13:54
 */
 
#include "Arduino.h"

#define Write_Pin PC0
#define Read_Pin  PB1

void setup()
{
    pinMode(Write_Pin, OUTPUT);
    pinMode(Read_Pin, INPUT);
}

void loop()
{
    int value = digitalRead_FAST(Read_Pin);

    if(value)
        digitalWrite_HIGH(Write_Pin);
    else
        digitalWrite_LOW(Write_Pin);
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
