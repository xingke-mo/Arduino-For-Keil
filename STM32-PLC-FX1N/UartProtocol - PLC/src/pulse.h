#ifndef _PULSE_H_
#define _PULSE_H_

#include"mcuhead.h"

#define Para     0.05

#define PWM_DIR_SET() SETBIT(GPIOB->ODR,BIT7)
#define PWM_DIR_CLR() CLRBIT(GPIOB->ODR,BIT7)

#define PWM_CW_SET() SETBIT(GPIOB->ODR,BIT6)
#define PWM_CW_CLR() CLRBIT(GPIOB->ODR,BIT6)


//=============================================================================================================================================================
//=============================================================================================================================================================

typedef enum
{
    OK = 1,
    ERR,
} Err;
typedef enum
{
    addst,
    avest,
    redst,
    stop,
} st;
typedef enum
{
    Normal,
    Exception,
} mode;
typedef enum
{
    Forward,
    Reverse,
} FR;
typedef struct
{
    u32 FirstSpeed;//���ٶ�
    u32 EndSpeed;   //�յ��ٶ�
    u32 AddTime;//����ʱ��
    u32 MinusTime;//����ʱ��
    u32 Frequency;//Ƶ��
    s32 Quantity;//����λ��
    s32 JQu;        //����λ��
    s32 NowQ;//��ǰλ��
    u32 A;//���ٶ�
    u32 D;//���ٶ�
    u32 F;//��ǰƵ��
    u32 S;//��ǰʱ��

    u32 Apcs;//����������
    u32 Ddip;//����������
    st ST;      //״̬
    mode Mode;//ģʽ
    FR fr;//����ת
} Pwm;
typedef struct
{
    u32 FirstSpeed;//���ٶ�
    u32 EndSpeed;   //�յ��ٶ�
    u32 AddTime;//����ʱ��
    u32 MinusTime;//����ʱ��
    u32 Frequency;//Ƶ��
    s32 Quantity;//����λ��
} AD_Para;
typedef enum
{
    DRVI,
    DRVA,
} Pwm_Mode;
typedef enum
{
    OFF,
    ON,
} Pwm_Stast;
typedef struct
{
    u8 axis;
    AD_Para Para_Data;
    Pwm_Mode pwm_mode;
    Pwm_Stast pwm_stast;
} Axis;
//=============================================================================================================================================================
//=============================================================================================================================================================

s32 Send_JQuantity(u8 axis);

void JQuantity_Clear(u8 axis);

void Pwm_UpData(Axis* axis);
Err Pwm_Start(void);
Err Pwm_Stop(void);
u8 PWM_checkAllStop(void);

void PWM_AxisStats(u8 axis, u8 stats);



u8 DRVI3_UPDATA(Axis* axis);
u8 DRVA3_UPDATA(Axis* axis);



void TIM6_Init(u16 arr, u16 psc);


//=============================================================================================================================================================

void TIM4_Pulse(void);
Err AD_DRVI3(Pwm *pwmset);//��Զ�λ����
Err AD_DRVA3(Pwm* pwmset);//���Զ�λ����

//=============================================================================================================================================================


#endif
