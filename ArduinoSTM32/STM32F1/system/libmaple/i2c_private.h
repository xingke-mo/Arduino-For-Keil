/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2012 LeafLabs, LLC.
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

#ifndef _LIBMAPLE_I2C_PRIVATE_H_
#define _LIBMAPLE_I2C_PRIVATE_H_

#include <libmaple/i2c_common.h>

/* For old-style definitions (SDA/SCL on same GPIO device) */
#define I2C_DEV_OLD(num, port, sda, scl)          \
    {                                             \
        .regs         = I2C##num##_BASE,          \
                        .msg          = NULL,                     \
                                        .error_flags  = 0,                        \
                                                .timestamp    = 0,                        \
                                                        .gpio_port    = port,                     \
                                                                .sda_port     = NULL,                     \
                                                                        .scl_port     = NULL,                     \
                                                                                .msgs_left    = 0,                        \
                                                                                        .sda_pin      = sda,                      \
                                                                                                .scl_pin      = scl,                      \
                                                                                                        .clk_id       = RCC_I2C##num,             \
                                                                                                                .ev_nvic_line = NVIC_I2C##num##_EV,       \
                                                                                                                        .er_nvic_line = NVIC_I2C##num##_ER,       \
                                                                                                                                .state        = I2C_STATE_DISABLED,       \
                                                                                                                                        .config_flags = 0,                        \
                                                                                                                                                .i2c_slave_xmit_callback = NULL,          \
                                                                                                                                                        .i2c_slave_recv_callback = NULL,          \
                                                                                                                                                                .i2c_slave_xmit_msg = NULL,               \
                                                                                                                                                                        .i2c_slave_recv_msg = NULL,               \
    }

/* For new-style definitions (SDA/SCL may be on different GPIO devices) */
#define I2C_DEV_NEW(num, sdaport, sdabit, sclport, sclbit)          \
    {                                                               \
        .regs         = I2C##num##_BASE,                            \
                        .msg          = NULL,                                       \
                                        .error_flags  = 0,                                          \
                                                .timestamp    = 0,                                          \
                                                        .gpio_port    = NULL,                                       \
                                                                .sda_port     = sdaport,                                    \
                                                                        .scl_port     = sclport,                                    \
                                                                                .msgs_left    = 0,                                          \
                                                                                        .sda_pin      = sdabit,                                     \
                                                                                                .scl_pin      = sclbit,                                     \
                                                                                                        .clk_id       = RCC_I2C##num,                               \
                                                                                                                .ev_nvic_line = NVIC_I2C##num##_EV,                         \
                                                                                                                        .er_nvic_line = NVIC_I2C##num##_ER,                         \
                                                                                                                                .state        = I2C_STATE_DISABLED,                         \
                                                                                                                                        .config_flags = 0,                                          \
                                                                                                                                                .i2c_slave_xmit_callback = NULL,                            \
                                                                                                                                                        .i2c_slave_recv_callback = NULL,                            \
                                                                                                                                                                .i2c_slave_xmit_msg = NULL,                                 \
                                                                                                                                                                        .i2c_slave_recv_msg = NULL,                                 \
    }

void _i2c_irq_handler(i2c_dev *dev);
void _i2c_irq_error_handler(i2c_dev *dev);

struct gpio_dev;

static inline struct gpio_dev* scl_port(const i2c_dev *dev)
{
    return (dev->gpio_port == NULL) ? dev->scl_port : dev->gpio_port;
}

static inline struct gpio_dev* sda_port(const i2c_dev *dev)
{
    return (dev->gpio_port == NULL) ? dev->sda_port : dev->gpio_port;
}

#endif  /* _LIBMAPLE_I2C_PRIVATE_H_ */
