#ifndef _runner_H_
#define _runner_H_

#include"mcuhead.h"
#include"uartprotocol.h"

//u8 LoadCode(void);
void RunCode(void);
void StopCode(void);

extern u16 DelayTime;

//����˿ڵ�����
#define OUTPUTCOUNT 12
//����˿ڵ�����
#define INPUTCOUNT 20
//PWM�˿ڵ�����
#define PWMCOUNT 1
//�豸ID
#define ID 0xff50


extern u8 InputData[32];
extern u8 OutputData[32];
extern u8 lastCheckIO;
extern u16 PC;
typedef struct
{
    void(*SetOutput)(UartEvent e);
    void(*GetInput)(UartEvent e);
    u8(*LoadCode)(u8);
    void(*RunCode)();
    void(*StopCode)();
    u8*(*GetCode)(u8 index);
    u16(*GetCodeSize)(u8 index);
} RunnerBase;

extern const RunnerBase CodeRunner;

#endif
