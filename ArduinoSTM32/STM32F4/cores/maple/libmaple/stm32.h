/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs, LLC.
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
 * @file stm32.h
 * @brief STM32 chip-specific definitions
 */

#ifndef _STM32_H_
#define _STM32_H_

/*
 * User-specific configuration.
 *
 * The #defines here depend upon how libmaple is used.  Because of the
 * potential for a mismatch between them and the actual libmaple
 * usage, you should try to keep their number to an absolute minimum.
 */

#ifdef __DOXYGEN_PREDEFINED_HACK

/** @brief APB1 clock speed, in Hz. */
#define STM32_PCLK1
/** @brief APB2 clock speed, in Hz. */
#define STM32_PCLK2

/** Deprecated.  Use STM32_PCLK1 instead. */
#define PCLK1
/** Deprecated. Use STM32_PCLK2 instead. */
#define PCLK2

#endif


/*
 * Density-specific configuration.
 */

#ifdef __DOXYGEN_PREDEFINED_HACK

/**
 * @brief Number of interrupts in the NVIC.
 */
#define STM32_NR_INTERRUPTS

/** Deprecated.  Use STM32_NR_INTERRUPTS instead. */
#define NR_INTERRUPTS

#endif


/*
 * MCU-specific configuration.
 */

#ifdef __DOXYGEN_PREDEFINED_HACK

/**
 * Number of GPIO ports.
 */
#define STM32_NR_GPIO_PORTS

/**
 * @brief Multiplier to convert microseconds into loop iterations
 *        in delay_us().
 *
 * @see delay_us()
 */
#define STM32_DELAY_US_MULT

/**
 * @brief Pointer to end of built-in SRAM.
 *
 * Points to the address which is 1 byte past the last valid
 * SRAM address.
 */
#define STM32_SRAM_END

/** Deprecated.  Use STM32_NR_GPIO_PORTS instead. */
#define NR_GPIO_PORTS
/** Deprecated.  Use STM32_DELAY_US_MULT instead. */
#define DELAY_US_MULT

#endif

#include <boards.h>

#undef  STM32_PCLK1
#undef  STM32_PCLK2
#define STM32_PCLK1   (CLOCK_SPEED_HZ/4)
#define STM32_PCLK2   (CLOCK_SPEED_HZ/2)

#define SYSTICK_RELOAD_VAL      (CYCLES_PER_MICROSECOND*1000-1)

#define STM32_NR_GPIO_PORTS          5
#define STM32_DELAY_US_MULT         (CYCLES_PER_MICROSECOND/3)
#define STM32_SRAM_END              ((void*)0x20010000)
//#define STM32_SRAM_END              ((void*)0x20030000)

#define NR_GPIO_PORTS               STM32_NR_GPIO_PORTS
#define DELAY_US_MULT               STM32_DELAY_US_MULT


#endif  /* _STM32_H_ */
