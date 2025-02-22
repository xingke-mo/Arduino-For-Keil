#include"encode.h"

EncodeCallBack ENCALL;

void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
    u8 EXTADDR;
    u8 EXTOFFSET;
    EXTADDR = BITx / 4; //�õ��жϼĴ�����ı��
    EXTOFFSET = (BITx % 4) * 4;
    RCC->APB2ENR |= 0x01; //ʹ��io����ʱ��
    AFIO->EXTICR[EXTADDR] &= ~(0x000F << EXTOFFSET); //���ԭ�����ã�����
    AFIO->EXTICR[EXTADDR] |= GPIOx << EXTOFFSET; //EXTI.BITxӳ�䵽GPIOx.BITx
    //�Զ�����
    EXTI->IMR |= 1 << BITx; //  ����line BITx�ϵ��ж�

    //EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
    if(TRIM & 0x01)
    {
        EXTI->FTSR |= 1 << BITx;    //line BITx���¼��½��ش���
    }

    if(TRIM & 0x02)
    {
        EXTI->RTSR |= 1 << BITx;    //line BITx���¼��������ش���
    }
}

static void InitGPIO()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
    GPIOA->ODR |= BIT7 | BIT6;
    GPIOA->CRL &= 0x00ffffff;
    GPIOA->CRL |= 0x88000000;

    GPIOB->ODR |= BIT9;
    GPIOB->CRH &= 0xffffff0f;
    GPIOB->CRH |= 0x00000080;

    Ex_NVIC_Config(0, 6, 1);
    Ex_NVIC_Config(0, 7, 1);
    Ex_NVIC_Config(1, 9, 1);

    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}

static void Init(EncodeCallBack f)
{
    ENCALL = f;
    InitGPIO();
}

static u8 rl;
static u8 rr;
static u8 dis;
void EXTI9_5_IRQHandler(void)
{

    if(dis == 0)
    {
        if(EXTI->PR & RR)
        {
            EXTI->PR = RR;

            if(rl && (!(GPIOA->IDR & RL)))
            {
                dis = 5;
                rr = rl = 0;
                ENCALL(RotL);
            }
            else
            {
                rr = 1;
            }
        }

        if(EXTI->PR & RL)
        {
            EXTI->PR = RL;

            if(rr && (!(GPIOA->IDR & RR)))
            {
                dis = 5;
                rr = rl = 0;
                ENCALL(RotR);
            }
            else
            {
                rl = 1;
            }
        }
    }

    if(EXTI->PR & DD)
    {
        rr = rl = 0;
        EXTI->PR = DD;
        ENCALL(RDown);
    }
}

static void TimeCheck(void)
{
    if((GPIOA->IDR & RR) && (GPIOA->IDR & RL))
    {
        rr = rl = 0;
    }

    if(dis)
    {
        dis--;
    }
}

const EnCodeBase encode =
{
    Init,
    TimeCheck,
};
