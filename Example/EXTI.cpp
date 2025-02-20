#include "Arduino.h"

#define LED_Pin PC0
#define KEY_Pin PB1

void LED_Toogle()
{
    Serial.println("KEY is pressed!");
    togglePin(LED_Pin);
}

void setup()
{
    Serial.begin(19200);
    pinMode(LED_Pin, OUTPUT);
    pinMode(KEY_Pin, INPUT_PULLUP);
    attachInterrupt(KEY_Pin, LED_Toogle, FALLING);
}

void loop()
{
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
