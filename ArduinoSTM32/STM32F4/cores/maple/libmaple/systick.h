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
 * @file systick.h
 *
 * @brief Various system timer definitions
 */

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "libmaple_types.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** SysTick register map type */
typedef struct systick_reg_map
{
    __IO uint32 CTRL;           /**< Control and status register */
    __IO uint32 LOAD;           /**< Reload value register */
    __IO uint32 VAL;            /**< Current value register ("count") */
    __IO uint32 CALIB;          /**< Calibration value register */
} systick_reg_map;

extern systick_reg_map * const SYSTICK;

/** SysTick register map base pointer */
#define SYSTICK_BASE                    ((struct systick_reg_map*)0xE000E010)

/*
 * Register bit definitions.
 */

/* Control and status register */

#define SYSTICK_CTRL_COUNTFLAG          BIT(16)
#define SYSTICK_CTRL_CLKSOURCE          BIT(2)
#define SYSTICK_CTRL_CLKSOURCE_CORE     BIT(2)
#define SYSTICK_CTRL_TICKINT            BIT(1)
#define SYSTICK_CTRL_TICKINT_PENDING    BIT(1)
#define SYSTICK_CTRL_ENABLE             BIT(0)

/* Calibration value register */

#define SYSTICK_CALIB_NOREF             BIT(31)
#define SYSTICK_CALIB_SKEW              BIT(30)
#define SYSTICK_CALIB_TENMS             0xFFFFFF

/** System elapsed time, in milliseconds */
extern volatile uint32 systick_uptime_millis;


void systick_init(uint32 reload_val);
void systick_disable();
void systick_enable();
void systick_attach_callback(void (*callback)(void));

/**
 * @brief Returns the system uptime, in milliseconds.
 */
static inline uint32 systick_uptime(void)
{
    return systick_uptime_millis;
}

/**
 * @brief Returns the current value of the SysTick counter.
 */
static inline uint32 systick_get_count(void)
{
    return SYSTICK->VAL;
}

/**
 * @brief Check for underflow.
 *
 * This function returns 1 if the SysTick timer has counted to 0 since
 * the last time it was called.  However, any reads of any part of the
 * SysTick Control and Status Register SYSTICK_BASE->CSR will
 * interfere with this functionality.  See the ARM Cortex M3 Technical
 * Reference Manual for more details (e.g. Table 8-3 in revision r1p1).
 */
static inline uint32 systick_check_underflow(void)
{
    return SYSTICK->CTRL & SYSTICK_CTRL_COUNTFLAG;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif

