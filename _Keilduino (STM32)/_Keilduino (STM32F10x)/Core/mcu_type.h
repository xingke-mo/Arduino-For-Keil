/*
 * @Author: xingke.mo xingke.mo
 * @Date: 2025-02-20 09:14:14
 * @LastEditors: xingke.mo xingke.mo
 * @LastEditTime: 2025-02-20 10:14:27
 * @FilePath: \_Keilduino (STM32F10x)\Core\mcu_type.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __MCU_TYPE
#define __MCU_TYPE

//*************** BASIC ***************//
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

typedef enum
{
    F_CPU_16MHz,
    F_CPU_24MHz,
    F_CPU_32MHz,
    F_CPU_40MHz,
    F_CPU_48MHz,
    F_CPU_56MHz,
    F_CPU_64MHz,
    F_CPU_72MHz,
    F_CPU_80MHz,
    F_CPU_88MHz,
    F_CPU_96MHz,
    F_CPU_104MHz,
    F_CPU_112MHz,
    F_CPU_120MHz,
    F_CPU_128MHz,
} F_CPU_Type;

typedef struct
{
    uint32_t F_CPU_x;
    uint32_t RCC_PLLMul_x;
} SysClock_TypeDef;

#define __KEILDUINO__               780
#define __STM32__
#define __STM32F1__
#define F_CPU                    SystemCoreClock
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

#endif
