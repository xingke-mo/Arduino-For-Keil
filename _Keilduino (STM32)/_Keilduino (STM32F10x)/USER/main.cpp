/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-18 11:43:35
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-18 16:13:54
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
基本:
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

	Serial.begin(115200);
	Serial.setTimeout(20);
	if(Serial.available())
	{
		String s = Serial.readString();
	}
外设:
Serial

 	Serial.begin(115200);
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

/**
 *  TaskScheduler Test
 * 
 *  This test illustrates how to use TS's tickless support functionality
 *  Tickless support enables more deterministic sleep by calculating time delay until next task invocation
 *  during every pass. That delay could then be used to put microcontroller in sleep mode continously
 *  instead of in small intervals
 *
 *  Initially only tasks 1 and 2 are enabled
 *  Task1 runs every 2 seconds 10 times and then stops
 *  Task2 runs every 3 seconds indefinitely
 *  Task1 enables Task3 at its first run
 *  Task3 run every 5 seconds
 *  Task1 disables Task3 on its last iteration and changed Task2 to run every 1/2 seconds
 *  At the end Task2 is the only task running every 1/2 seconds
 */

#define _TASK_SLEEP_ON_IDLE_RUN  // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
#define _TASK_TICKLESS           // Enable support for tickless sleep on FreeRTOS
#define _TASK_DO_NOT_YIELD       // Disable yield() method in execute() 
 
#include <TaskScheduler.h>

// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();

//Tasks
Task t4();
Task t1(2000, 10, &t1Callback);
Task t2(3000, TASK_FOREVER, &t2Callback);
Task t3(5000, TASK_FOREVER, &t3Callback);

Scheduler runner;


void t1Callback() {
    Serial.print("t1: ");
    Serial.println(millis());
    
    if (t1.isFirstIteration()) {
      runner.addTask(t3);
      t3.enable();
      Serial.println("t1: enabled t3 and added to the chain");
    }
    
    if (t1.isLastIteration()) {
      t3.disable();
      runner.deleteTask(t3);
      t2.setInterval(500);
      Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    }
}

void t2Callback() {
    Serial.print("t2: ");
    Serial.println(millis());
	
	// comment this line out if you want to test t2's 500 ms explicit delay
	// as-is this delay tests that task in catch up mode will prevent explicit tickless delay
    delay(501);
  
}

void t3Callback() {
    Serial.print("t3: ");
    Serial.println(millis());
  
}

#if 1
#define LED_Pin PC13
#define PWM_Pin PA0
#define ADC_Pin PA0

#define Write_Pin PA0
#define Read_Pin  PA1

#define KEY_Pin PA1

unsigned long lasttime;

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
    lasttime = millis();

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
    
  Serial.begin(115200);
  Serial.println("Scheduler TEST");

  runner.init();
  Serial.println("Initialized scheduler");

  runner.addTask(t1);
  Serial.println("added t1");
  
  runner.addTask(t2);
  Serial.println("added t2");

  delay(1000);

  t1.enable();
  Serial.println("Enabled t1");
  t2.enable();
  Serial.println("Enabled t2");    
}

unsigned long nr = 0;
void loop()
{
    if(millis() - lasttime >=5000){
        lasttime = millis();
        if(digitalRead(LED_Pin)==HIGH){
        digitalWrite(LED_Pin, LOW);
        }
        else{
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
    
  runner.execute();
  nr = runner.getNextRun();
  if ( nr ) {
    Serial.println("TS stats:");
    Serial.print("\tTotal   tasks: "); Serial.println(runner.getTotalTasks() );
    Serial.print("\tActive  tasks: "); Serial.println(runner.getActiveTasks() );
    Serial.print("\tInvoked tasks: "); Serial.println(runner.getInvokedTasks() );
    Serial.print("Next scheduling pass in "); Serial.print(nr); Serial.println(" ms");
    delay(nr);
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
