/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
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
 * @file   aeroquad32.h
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Private include file for Maple Native in boards.h
 *
 * See maple.h for more information on these definitions.
 */

#ifndef _BOARD_DISCOVERY_F4_H_
#define _BOARD_DISCOVERY_F4_H_

#define Port2Pin(port, bit) ((port-'A')*16+bit)

#define CLOCK_SPEED_MHZ  168


#define BOARD_LED_PIN           PD12
#define BOARD_BUTTON_PIN        PA0

#define BOARD_USB_DM_PIN        PA11
#define BOARD_USB_DP_PIN        PA12

#define BOARD_NR_USARTS         5
#define BOARD_USART1_TX_PIN     PA9
#define BOARD_USART1_RX_PIN     PA10
#define BOARD_USART2_TX_PIN     PA2
#define BOARD_USART2_RX_PIN     PA3
#define BOARD_USART3_TX_PIN     PD8
#define BOARD_USART3_RX_PIN     PD9
#define BOARD_UART4_TX_PIN      PC10
#define BOARD_UART4_RX_PIN      PC11
#define BOARD_UART5_TX_PIN      PC12
#define BOARD_UART5_RX_PIN      PD2

#define BOARD_NR_SPI            3
#ifdef BOARD_STM32F4_NETDUINO2PLUS
#define BOARD_SPI1_NSS_PIN      PC8
#else
#define BOARD_SPI1_NSS_PIN      PA4
#endif
#define BOARD_SPI1_MOSI_PIN     PA7
#ifdef BOARD_STM32F4_NETDUINO2PLUS
#define BOARD_SPI1_MISO_PIN     PB4
#else
#define BOARD_SPI1_MISO_PIN     PA6
#endif
#define BOARD_SPI1_SCK_PIN      PA5
#define BOARD_SPI2_NSS_PIN      PB12
#define BOARD_SPI2_MOSI_PIN     PB15
#define BOARD_SPI2_MISO_PIN     PB14
#define BOARD_SPI2_SCK_PIN      PB13
#define BOARD_SPI3_NSS_PIN      PA15
#define BOARD_SPI3_MOSI_PIN     PB5
#define BOARD_SPI3_MISO_PIN     PB4
#define BOARD_SPI3_SCK_PIN      PB3

#ifdef BOARD_STM32F4_NETDUINO2PLUS
#define BOARD_SPI3B_NSS_PIN      PB0
#else
#define BOARD_SPI3B_NSS_PIN      PB8
#endif
#define BOARD_SPI3B_MOSI_PIN     PC12
#define BOARD_SPI3B_MISO_PIN     PC11
#define BOARD_SPI3B_SCK_PIN      PC10

#define BOARD_NR_GPIO_PINS      112
#define BOARD_NR_PWM_PINS       22
#define BOARD_NR_ADC_PINS       16
#define BOARD_NR_USED_PINS      43 // ala42 not set yet
#define BOARD_JTMS_SWDIO_PIN    PA13
#define BOARD_JTCK_SWCLK_PIN    PA14
#define BOARD_JTDI_PIN          PA15
#define BOARD_JTDO_PIN          PB3
#define BOARD_NJTRST_PIN        PB4


enum
{
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
    PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15
};

#endif
