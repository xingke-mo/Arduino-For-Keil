/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-20 10:28:20
 */

/*
第三方库移植示例
    1.从Arduino的安装库的文件夹中找到要调用的第三方库
    2.将刚才找到的第三方库放到Keil工程模板的Libraries文件夹内
    3.启动Keil工程
    4.将第三方库的源代码文件添加到Keil的工程组
    5.找到第三方库文件夹
    6.添加路径
    7.找到第三方库自带的示例程序(一般位于第三方库目录下的examples内)
    8.复制示例程序
    9.将示例程序复制到Keil工程模板的main.cpp里，替换 setup() 和 loop() 函数
    10.修改示例程序里的引脚定义为STM32的引脚
    11.点击编译，如果编译无错误表示移植成功

void setup()
{
	pinMode(PA0,OUTPUT);			//使用Arduino函数将配置PA0为输出模式
}

void loop()
{
	GPIOA -> BSRR = GPIO_Pin_0;		//使用寄存器将PA0电平拉高
	delay(1000);				//延时一秒
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	//使用标准外设库的函数将PA0电平拉低
	delay(1000);				//延时一秒
}

支持Arduino函数有:

时间:
	delay(Time_ms);
	delayMicroseconds(Time_us);
	millis();
	micros();
    
输入输出:
	pinMode(Pin,Mode);
	attachInterrrupt(Pin,Callback_function,Trigger_Mode);	
	digitalWrite(Pin,State);
	digitalRead(Pin);
	analogRead(Pin);
	analogWrite(Pin,DutyCycle);
	shiftOut(dataPin,clockPin,bitOrder,value);
	shiftIn(dataPin,clockPin,bitOrder);
	tone(Pin,Frequency,Time_ms);
	tone(Pin,Frequency);
Print
	Serial.print("Hello STM32");
	Serial.printf("Hello STM%d",32);
	Serial << "Hello STM32" << "\r\n";
String
	String s1 = "Hello";
	String s2 = " STM32";
	Serial.print(s1 + s2);
Stream
	Serial.begin(19200);
	Serial.setTimeout(20);
	if(Serial.available())
	{
		String s = Serial.readString();
	}
    
外设:
Serial
 	Serial.begin(19200);
	while(Serial.available())
	{
		Serial.print(Serial.read());
	}
Wire
	Wire.begin();
	Wire.beginTransmission(0xFF);
	Wire.write(0x01);
	Wire.endTransmission();
SPI
	SPI.begin();
	SPI.transfer(0xFF);
*/


#include "Arduino.h"

#if 0
#define LED_Pin     PC0
#define LED1_Pin    PC1
#define LED2_Pin    PC2

#define PWM_Pin     PC7

#define ADC_Pin     PA0

#define Write_Pin   PC3

#define Read_Pin    PB1
#define KEY_Pin     PB0

unsigned long lasttime;

void LED_Toogle()
{
    Serial.println("KEY is pressed!");
    togglePin(LED_Pin);
}

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
    lasttime = millis();

    Serial.begin(19200);
    
    Serial.printf("C++ Version: %d\r\n", __cplusplus);
    Serial.printf("Compiling Time: %s %s\r\n", __DATE__, __TIME__);
    pinMode(LED_Pin, OUTPUT);
    pinMode(PWM_Pin, PWM);

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
    Serial.println("Serial printing...");
    Serial.setTimeout(10);
    Serial.attachInterrupt(Serial_EventHandler);
}

void loop()
{
    if(millis() - lasttime >=5000)
    {
        lasttime = millis();
        if(digitalRead(LED_Pin)==HIGH)
        {
            digitalWrite(LED_Pin, LOW);
        }
        else
        {
            digitalWrite(LED_Pin, HIGH);
        }
      }

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
    SysClock_Init(F_CPU_24MHz);
    Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
