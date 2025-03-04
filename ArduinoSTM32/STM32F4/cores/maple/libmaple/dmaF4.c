/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Michael Hope.
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
 * @file dmaF4.c
 * @brief Direct Memory Access peripheral support
 */

#include "dma.h"
#include "bitband.h"
#include "util.h"

/*
 * Devices
 */


dma_handler_t dma1_handlers[8] =
{
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM0 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM1 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM2 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM3 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM4 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM5 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM6 },
    { .handler = NULL, .irq_line = NVIC_DMA1_STREAM7 },
};

dma_handler_t dma2_handlers[8] =
{
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM0 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM1 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM2 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM3 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM4 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM5 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM6 },
    { .handler = NULL, .irq_line = NVIC_DMA2_STREAM7 },
};

/** DMA1 device */
const dma_dev dma1 =
{
    .regs      = DMA1_BASE,
    .clk_id    = RCC_DMA1,
    .handler_p = &dma1_handlers,
};

/** DMA2 device */
const dma_dev dma2 =
{
    .regs      = DMA2_BASE,
    .clk_id    = RCC_DMA2,
    .handler_p = &dma2_handlers,
};

/*
 * Convenience routines
 */

/**
 * @brief Attach an interrupt to a DMA transfer.
 *
 * Interrupts are enabled using appropriate mode flags in
 * dma_setup_transfer().
 *
 * @param dev DMA device
 * @param stream Stream to attach handler to
 * @param handler Interrupt handler to call when channel interrupt fires.
 * @see dma_setup_transfer()
 * @see dma_detach_interrupt()
 */
void dma_attach_interrupt(const dma_dev *dev,
                          dma_stream stream,
                          void (*handler)(void))
{
    dma_handler_t * dma_handler_p = &(*(dev->handler_p))[stream];
    dma_handler_p->handler = handler;
    nvic_irq_enable(dma_handler_p->irq_line);
}

/**
 * @brief Detach a DMA transfer interrupt handler.
 *
 * After calling this function, the given channel's interrupts will be
 * disabled.
 *
 * @param dev DMA device
 * @param stream Stream whose handler to detach
 * @sideeffect Clears interrupt enable bits in the channel's CCR register.
 * @see dma_attach_interrupt()
 */
void dma_detach_interrupt(const dma_dev *dev, dma_stream stream)
{
    dma_handler_t * dma_handler_p = &(*(dev->handler_p))[stream];
    nvic_irq_disable(dma_handler_p->irq_line);
    dma_handler_p->handler = NULL;
}

const uint8 dma_isr_bits_shift[] = { 0, 6, 16, 22};

uint8 dma_get_isr_bit(const dma_dev *dev, dma_stream stream, uint8_t mask)
{
    if(stream & 0xFC)
    {
        return ((dev->regs->HISR) >> dma_isr_bits_shift[stream & 0x03]) & mask;
    }
    else
    {
        return ((dev->regs->LISR) >> dma_isr_bits_shift[stream & 0x03]) & mask;
    }
}

void dma_clear_isr_bit(const dma_dev *dev, dma_stream stream, uint8_t mask)
{
    if(stream & 0xFC)
    {
        dev->regs->HIFCR = (uint32)mask << dma_isr_bits_shift[stream & 0x03];
    }
    else
    {
        dev->regs->LIFCR = (uint32)mask << dma_isr_bits_shift[stream & 0x03];
    }
}

void dma_set_mem_addr(const dma_dev *dev, dma_stream stream, __IO void *addr)
{
    dma_disable(dev, stream);
    dev->regs->STREAM[stream].M0AR = (uint32)addr;
}

void dma_set_per_addr(const dma_dev *dev, dma_stream stream, __IO void *addr)
{
    dma_disable(dev, stream);
    dev->regs->STREAM[stream].PAR = (uint32)addr;
}


/*
 * IRQ handlers
 */

static inline void dispatch_handler(const dma_dev *dev, dma_stream stream)
{
    voidFuncPtr handler = (*(dev->handler_p))[stream].handler;

    if(handler)
    {
        handler();
    }

    dma_clear_isr_bits(dev, stream); // in case handler doesn't
}

void __irq_dma1_stream0(void)
{
    dispatch_handler(DMA1, DMA_STREAM0);
}

void __irq_dma1_stream1(void)
{
    dispatch_handler(DMA1, DMA_STREAM1);
}

void __irq_dma1_stream2(void)
{
    dispatch_handler(DMA1, DMA_STREAM2);
}

void __irq_dma1_stream3(void)
{
    dispatch_handler(DMA1, DMA_STREAM3);
}

void __irq_dma1_stream4(void)
{
    dispatch_handler(DMA1, DMA_STREAM4);
}

void __irq_dma1_stream5(void)
{
    dispatch_handler(DMA1, DMA_STREAM5);
}

void __irq_dma1_stream6(void)
{
    dispatch_handler(DMA1, DMA_STREAM6);
}

void __irq_dma1_stream7(void)
{
    dispatch_handler(DMA1, DMA_STREAM7);
}

void __irq_dma2_stream0(void)
{
    dispatch_handler(DMA2, DMA_STREAM0);
}

void __irq_dma2_stream1(void)
{
    dispatch_handler(DMA2, DMA_STREAM1);
}

void __irq_dma2_stream2(void)
{
    dispatch_handler(DMA2, DMA_STREAM2);
}

void __irq_dma2_stream3(void)
{
    dispatch_handler(DMA2, DMA_STREAM3);
}

void __irq_dma2_stream4(void)
{
    dispatch_handler(DMA2, DMA_STREAM4);
}

void __irq_dma2_stream5(void)
{
    dispatch_handler(DMA2, DMA_STREAM5);
}

void __irq_dma2_stream6(void)
{
    dispatch_handler(DMA2, DMA_STREAM6);
}

void __irq_dma2_stream7(void)
{
    dispatch_handler(DMA2, DMA_STREAM7);
}
