/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file nvic.h
 * @brief Nested vector interrupt controller support.
 *
 * Basic usage:
 *
 * @code
 *   // Initialise the interrupt controller and point to the vector
 *   // table at the start of flash.
 *   nvic_init(0x08000000, 0);
 *   // Bind in a timer interrupt handler
 *   timer_attach_interrupt(TIMER_CC1_INTERRUPT, handler);
 *   // Optionally set the priority
 *   nvic_irq_set_priority(NVIC_TIMER1_CC, 5);
 *   // All done, enable all interrupts
 *   nvic_globalirq_enable();
 * @endcode
 */

#ifndef _NVIC_H_
#define _NVIC_H_

#include "libmaple_types.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** NVIC register map type. */
typedef struct nvic_reg_map
{
    __IO uint32 ISER[3];      /**< Interrupt Set Enable Registers */
    uint32 RESERVED0[29];     /**< Reserved */
    __IO uint32 ICER[3];      /**< Interrupt Clear Enable Registers */
    uint32 RSERVED1[29];      /**< Reserved */
    __IO uint32 ISPR[3];      /**< Interrupt Set Pending Registers */
    uint32 RESERVED2[29];     /**< Reserved */
    __IO uint32 ICPR[3];      /**< Interrupt Clear Pending Registers */
    uint32 RESERVED3[29];     /**< Reserved */
    __IO uint32 IABR[3];      /**< Interrupt Active bit Registers */
    uint32 RESERVED4[61];     /**< Reserved */
    __IO uint8  IP[84];       /**< Interrupt Priority Registers */
    uint32 RESERVED5[683];    /**< Reserved */
    __IO uint32 STIR;         /**< Software Trigger Interrupt Registers */
} nvic_reg_map;

/** NVIC register map base pointer. */
#define NVIC_BASE                       ((struct nvic_reg_map*)0xE000E100)

/**
 * @brief Interrupt vector table interrupt numbers.
 *
 * Each positive-valued enumerator is the position of the
 * corresponding interrupt in the vector table.  Negative-valued
 * enumerators correspond to interrupts controlled by the system
 * handler block.
 *
 * @see scb.h
 */
typedef enum nvic_irq_num
{
    NVIC_NMI            = -14,  /**< Non-maskable interrupt */
    NVIC_HARDFAULT      = -13,  /**< Hard fault (all class of fault) */
    NVIC_MEM_MANAGE     = -12,  /**< Memory management */
    NVIC_BUS_FAULT      = -11,  /**< Bus fault: prefetch fault, memory
                                     access fault. */
    NVIC_USAGE_FAULT    = -10,  /**< Usage fault: Undefined instruction or
                                     illegal state. */
    NVIC_SVC            = -5,   /**< System service call via SWI insruction */
    NVIC_DEBUG_MON      = -4,   /**< Debug monitor */
    NVIC_PEND_SVC       = -2,   /**< Pendable request for system service */
    NVIC_SYSTICK        = -1,   /**< System tick timer */
    NVIC_WWDG           = 0,    /**< Window watchdog interrupt */
    NVIC_PVD            = 1,    /**< PVD through EXTI line detection */
    NVIC_TAMPSTAMP      = 2,    /**< Tamper and TimeStamp */
    NVIC_RTC            = 3,    /**< Real-time clock */
    NVIC_FLASH          = 4,    /**< Flash */
    NVIC_RCC            = 5,    /**< Reset and clock control */
    NVIC_EXTI0          = 6,    /**< EXTI line 0 */
    NVIC_EXTI1          = 7,    /**< EXTI line 1 */
    NVIC_EXTI2          = 8,    /**< EXTI line 2 */
    NVIC_EXTI3          = 9,    /**< EXTI line 3 */
    NVIC_EXTI4          = 10,   /**< EXTI line 4 */
    NVIC_DMA1_STREAM0   = 11,   /**< DMA1 stream 0 */
    NVIC_DMA1_STREAM1   = 12,   /**< DMA1 stream 1 */
    NVIC_DMA1_STREAM2   = 13,   /**< DMA1 stream 2 */
    NVIC_DMA1_STREAM3   = 14,   /**< DMA1 stream 3 */
    NVIC_DMA1_STREAM4   = 15,   /**< DMA1 stream 4 */
    NVIC_DMA1_STREAM5   = 16,   /**< DMA1 stream 5 */
    NVIC_DMA1_STREAM6   = 17,   /**< DMA1 stream 6 */
    NVIC_ADC_1_2_3      = 18,   /**< ADC 1-3 */
    NVIC_CAN1_TX        = 19,   /**< CAN1 TX */
    NVIC_CAN1_RX0       = 20,   /**< CAN1 RX0 */
    NVIC_CAN1_RX1       = 21,   /**< CAN1 RX1 */
    NVIC_CAN1_SCE       = 22,   /**< CAN1 SCE */
    NVIC_EXTI_9_5       = 23,   /**< EXTI line [9:5] */
    NVIC_TIMER1_BRK     = 24,   /**< Timer 1 break */
    NVIC_TIMER1_UP      = 25,   /**< Timer 1 update */
    NVIC_TIMER1_TRG_COM = 26,   /**< Timer 1 trigger and commutation */
    NVIC_TIMER1_CC      = 27,   /**< Timer 1 capture/compare */
    NVIC_TIMER2         = 28,   /**< Timer 2 */
    NVIC_TIMER3         = 29,   /**< Timer 3 */
    NVIC_TIMER4         = 30,   /**< Timer 4 */
    NVIC_I2C1_EV        = 31,   /**< I2C1 event */
    NVIC_I2C1_ER        = 32,   /**< I2C1 error */
    NVIC_I2C2_EV        = 33,   /**< I2C2 event */
    NVIC_I2C2_ER        = 34,   /**< I2C2 error */
    NVIC_SPI1           = 35,   /**< SPI1 */
    NVIC_SPI2           = 36,   /**< SPI2 */
    NVIC_USART1         = 37,   /**< USART1 */
    NVIC_USART2         = 38,   /**< USART2 */
    NVIC_USART3         = 39,   /**< USART3 */
    NVIC_EXTI_15_10     = 40,   /**< EXTI line [15:10] */
    NVIC_RTCALARM       = 41,   /**< RTC alarm through EXTI line */
    NVIC_USB_FS_WKUP    = 42,   /**< USB FS wakeup through EXTI line */
    NVIC_TIMER8_BRK     = 43,   /**< Timer 8 break */
    NVIC_TIMER8_UP      = 44,   /**< Timer 8 update */
    NVIC_TIMER8_TRG_COM = 45,   /**< Timer 8 trigger and commutation */
    NVIC_TIMER8_CC      = 46,   /**< Timer 8 capture/compare */
    NVIC_DMA1_STREAM7   = 47,   /**< DMA1 stream 7 */
    NVIC_FSMC           = 48,   /**< FSMC */
    NVIC_SDIO           = 49,   /**< SDIO */
    NVIC_TIMER5         = 50,   /**< Timer 5 */
    NVIC_SPI3           = 51,   /**< SPI3 */
    NVIC_UART4          = 52,   /**< UART4 */
    NVIC_UART5          = 53,   /**< UART5 */
    NVIC_TIMER6_DAC     = 54,   /**< Timer 6 & DAC */
    NVIC_TIMER7         = 55,   /**< Timer 7 */
    NVIC_DMA2_STREAM0   = 56,   /**< DMA2 stream 0 */
    NVIC_DMA2_STREAM1   = 57,   /**< DMA2 stream 1 */
    NVIC_DMA2_STREAM2   = 58,   /**< DMA2 stream 2 */
    NVIC_DMA2_STREAM3   = 59,   /**< DMA2 stream 3 */
    NVIC_DMA2_STREAM4   = 60,   /**< DMA2 stream 4 */
    NVIC_ETH            = 61,   /**< Ethernet */
    NVIC_ETH_WKUP       = 62,   /**< Ethernet Wakeup through EXTI line */
    NVIC_CAN2_TX        = 63,   /**< CAN2 TX */
    NVIC_CAN2_RX0       = 64,   /**< CAN2 RX0 */
    NVIC_CAN2_RX1       = 65,   /**< CAN2 RX1 */
    NVIC_CAN2_SCE       = 66,   /**< CAN2 SCE */
    NVIC_USB_FS         = 67,   /**< USB FS global interrupt */
    NVIC_DMA2_STREAM5   = 68,   /**< DMA2 stream 5 */
    NVIC_DMA2_STREAM6   = 69,   /**< DMA2 stream 6 */
    NVIC_DMA2_STREAM7   = 70,   /**< DMA2 stream 7 */
    NVIC_USART6         = 71,   /**< USART6 */
    NVIC_I2C3_EV        = 72,   /**< I2C3 event */
    NVIC_I2C3_ER        = 73,   /**< I2C3 error */
    NVIC_USB_HS_EP1_OUT = 74,
    NVIC_USB_HS_EP1_IN  = 75,
    NVIC_USB_HS_WKUP    = 76,
    NVIC_USB_HS         = 77,
    NVIC_DCMI           = 78,
    NVIC_CRYPT          = 79,
    NVIC_HASH_RNG       = 80,
    NVIC_FPU            = 81,
    NVIC_UART7          = 82,
    NVIC_UART8          = 83,
    NVIC_SPI4           = 84,
    NVIC_SPI5           = 85,
    NVIC_UART9          = 88,
    NVIC_UART10         = 89,
    NVIC_QUAD_SPI       = 92,
    NVIC_LAST_IRQ_NUM,
} nvic_irq_num;

/*
 * Initialises the interrupt controller and sets all interrupts to the
 * lowest priority.
 *
 * For stand-alone products, the base address is normally the start of
 * flash (0x08000000).
 *
 * @param vector_table_address  base address of the vector table
 */
void nvic_init(uint32 vector_table_address, uint32 offset);

/**
 * Sets the base address of the vector table.
 */
void nvic_set_vector_table(uint32 address, uint32 offset);

void nvic_irq_set_priority(nvic_irq_num irqn, uint8 priority);

/**
 * Enables interrupts and configurable fault handlers (clear PRIMASK).
 */
static inline void nvic_globalirq_enable()
{
    asm volatile("cpsie i");
}

/**
 * Disable interrupts and configurable fault handlers (set PRIMASK).
 */
static inline void nvic_globalirq_disable()
{
    asm volatile("cpsid i");
}

/**
 * @brief Enable interrupt irq_num
 * @param irq_num Interrupt to enable
 */
static inline void nvic_irq_enable(nvic_irq_num irq_num)
{
    if(irq_num < 0)
    {
        return;
    }

    NVIC_BASE->ISER[irq_num / 32] = BIT(irq_num % 32);
}

/**
 * @brief Disable interrupt irq_num
 * @param irq_num Interrupt to disable
 */
static inline void nvic_irq_disable(nvic_irq_num irq_num)
{
    if(irq_num < 0)
    {
        return;
    }

    NVIC_BASE->ICER[irq_num / 32] = BIT(irq_num % 32);
}

/**
 * @brief Quickly disable all interrupts.
 *
 * Calling this function is significantly faster than calling
 * nvic_irq_disable() in a loop.
 */
static inline void nvic_irq_disable_all(void)
{
    NVIC_BASE->ICER[0] = 0xFFFFFFFF;
    NVIC_BASE->ICER[1] = 0xFFFFFFFF;
    NVIC_BASE->ICER[2] = 0x0001FFFF;
}

#ifdef __cplusplus
}
#endif

#endif

