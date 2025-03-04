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
 * @brief Wirish virtual serial port
 */

#ifndef _USB_SERIAL_H_
#define _USB_SERIAL_H_

#include "usb.h"
#include "Stream.h"

#ifdef SERIAL_USB

/**
 * @brief Virtual serial terminal.
 */
class USBSerial : public Stream
{
public:
    USBSerial(void);

    void begin(void);
    void begin(int);
    void end(void);

    virtual int available(void)
    {
        return usbBytesAvailable();
    }
    virtual int peek(void);
    virtual void flush(void);

    virtual int read(void *buf, uint32 len);
    virtual int read(void);

    size_t write(const uint8);
    size_t write(const char *str);
    size_t write(const void*, uint32);

    uint8 getRTS()
    {
        return usbGetRTS();
    }
    uint8 getDTR()
    {
        return usbGetDTR();
    }
    uint8 usbOK(void);
    operator bool()
    {
        return usbOK();
    }
    uint8 isConnected() __attribute__((deprecated("Use !Serial instead")))
    {
        return usbOK();
    }
    uint8 isConfigured()
    {
        return usbIsConfigured();
    }
    uint8 pending()
    {
        return usbGetPending();
    }

    void enableBlockingTx(void)
    {
        usbEnableBlockingTx();
    }
    void disableBlockingTx(void)
    {
        usbDisableBlockingTx();
    }

protected:
    static bool _hasBegun;
};

extern USBSerial SerialUSB;
#define Serial SerialUSB

#endif //  SERIAL_USB


#endif //  _USB_SERIAL_H_

