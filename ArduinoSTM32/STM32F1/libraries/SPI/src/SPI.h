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
 * @file wirish/include/wirish/HardwareSPI.h
 * @brief High-level SPI interface
 *
 * This is a "bare essentials" polling driver for now.
 */

/* TODO [0.1.0] Remove deprecated methods. */



#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <libmaple/libmaple_types.h>
#include <libmaple/spi.h>
#include <libmaple/dma.h>

#include <boards.h>
#include <stdint.h>
#include <wirish.h>

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
//#define SPI_HAS_TRANSACTION

#define SPI_CLOCK_DIV2   SPI_BAUD_PCLK_DIV_2
#define SPI_CLOCK_DIV4   SPI_BAUD_PCLK_DIV_4
#define SPI_CLOCK_DIV8   SPI_BAUD_PCLK_DIV_8
#define SPI_CLOCK_DIV16  SPI_BAUD_PCLK_DIV_16
#define SPI_CLOCK_DIV32  SPI_BAUD_PCLK_DIV_32
#define SPI_CLOCK_DIV64  SPI_BAUD_PCLK_DIV_64
#define SPI_CLOCK_DIV128 SPI_BAUD_PCLK_DIV_128
#define SPI_CLOCK_DIV256 SPI_BAUD_PCLK_DIV_256

/*
 * Roger Clark. 20150106
 * Commented out redundant AVR defined
 *
#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

// define SPI_AVR_EIMSK for AVR boards with external interrupt pins
#if defined(EIMSK)
  #define SPI_AVR_EIMSK  EIMSK
#elif defined(GICR)
  #define SPI_AVR_EIMSK  GICR
#elif defined(GIMSK)
  #define SPI_AVR_EIMSK  GIMSK
#endif
*/

#ifndef STM32_LSBFIRST
#define STM32_LSBFIRST 0
#endif
#ifndef STM32_MSBFIRST
#define STM32_MSBFIRST 1
#endif

// PC13 or PA4
#define BOARD_SPI_DEFAULT_SS PA4
//#define BOARD_SPI_DEFAULT_SS PC13

#define SPI_MODE0 SPI_MODE_0
#define SPI_MODE1 SPI_MODE_1
#define SPI_MODE2 SPI_MODE_2
#define SPI_MODE3 SPI_MODE_3

#define DATA_SIZE_8BIT SPI_CR1_DFF_8_BIT
#define DATA_SIZE_16BIT SPI_CR1_DFF_16_BIT

typedef enum
{
    SPI_STATE_IDLE,
    SPI_STATE_READY,
    SPI_STATE_RECEIVE,
    SPI_STATE_TRANSMIT,
    SPI_STATE_TRANSFER
} spi_mode_t;
class SPISettings
{
public:
    SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode)
    {
        if(__builtin_constant_p(clock))
        {
            init_AlwaysInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
        }
        else
        {
            init_MightInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
        }
    }
    SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize)
    {
        if(__builtin_constant_p(clock))
        {
            init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
        }
        else
        {
            init_MightInline(clock, bitOrder, dataMode, dataSize);
        }
    }
    SPISettings(uint32_t clock)
    {
        if(__builtin_constant_p(clock))
        {
            init_AlwaysInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
        }
        else
        {
            init_MightInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
        }
    }
    SPISettings()
    {
        init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
    }
private:
    void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize)
    {
        init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
    }
    void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) __attribute__((__always_inline__))
    {
        this->clock = clock;
        this->bitOrder = bitOrder;
        this->dataMode = dataMode;
        this->dataSize = dataSize;
    }
    uint32_t clock;
    uint32_t dataSize;
    uint32_t clockDivider;
    BitOrder bitOrder;
    uint8_t dataMode;
    uint8_t _SSPin;
    volatile spi_mode_t state;
    spi_dev *spi_d;
    dma_channel spiRxDmaChannel, spiTxDmaChannel;
    dma_dev* spiDmaDev;
    void (*receiveCallback)(void) = NULL;
    void (*transmitCallback)(void) = NULL;

    friend class SPIClass;
};


/*
 * Kept for compat.
 */

/**
 * @brief Wirish SPI interface.
 *
 * This implementation uses software slave management, so the caller
 * is responsible for controlling the slave select line.
 */
class SPIClass
{
public:

    /**
     * @param spiPortNumber Number of the SPI port to manage.
     */
    SPIClass(uint32 spiPortNumber);

    /*
     * Set up/tear down
     */

    /**
     * @brief Equivalent to begin(SPI_1_125MHZ, MSBFIRST, 0).
     */
    void begin(void);

    /**
     * @brief Turn on a SPI port and set its GPIO pin modes for use as a slave.
     *
     * SPI port is enabled in full duplex mode, with software slave management.
     *
     * @param bitOrder Either LSBFIRST (little-endian) or MSBFIRST(big-endian)
     * @param mode SPI mode to use
     */
    void beginSlave(uint32 bitOrder, uint32 mode);

    /**
     * @brief Equivalent to beginSlave(MSBFIRST, 0).
     */
    void beginSlave(void);

    /**
     * @brief Disables the SPI port, but leaves its GPIO pin modes unchanged.
     */
    void end(void);

    void beginTransaction(SPISettings settings)
    {
        beginTransaction(BOARD_SPI_DEFAULT_SS, settings);
    }
    void beginTransaction(uint8_t pin, SPISettings settings);
    void endTransaction(void);

    void beginTransactionSlave(SPISettings settings);

    void setClockDivider(uint32_t clockDivider);
    void setBitOrder(BitOrder bitOrder);
    void setDataMode(uint8_t dataMode);

    // SPI Configuration methods
    void attachInterrupt(void);
    void detachInterrupt(void);

    /*  Victor Perez. Added to change datasize from 8 to 16 bit modes on the fly.
    *   Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
    *   Requires an added function spi_data_size on  STM32F1 / cores / maple / libmaple / spi.c
    */
    void setDataSize(uint32 ds);

    /*  Victor Perez 2017. Added to set and clear callback functions for callback
    * on DMA transfer completion.
    * onReceive used to set the callback in case of dmaTransfer (tx/rx), once rx is completed
    * onTransmit used to set the callback in case of dmaSend (tx only). That function
    * will NOT be called in case of TX/RX
    */
    void onReceive(void(*)(void));
    void onTransmit(void(*)(void));

    /*
     * I/O
     */

    /**
     * @brief Return the next unread byte/word.
     *
     * If there is no unread byte/word waiting, this function will block
     * until one is received.
     */
    uint16 read(void);

    /**
     * @brief Read length bytes, storing them into buffer.
     * @param buffer Buffer to store received bytes into.
     * @param length Number of bytes to store in buffer. This
     *               function will block until the desired number of
     *               bytes have been read.
     */
    void read(uint8 *buffer, uint32 length);

    /**
     * @brief Transmit one byte/word.
     * @param data to transmit.
     */
    void write(uint16 data);
    void write16(uint16 data); // write 2 bytes in 8 bit mode (DFF=0)

    /**
     * @brief Transmit one byte/word a specified number of times.
     * @param data to transmit.
     */
    void write(uint16 data, uint32 n);

    /**
     * @brief Transmit multiple bytes/words.
     * @param buffer Bytes/words to transmit.
     * @param length Number of bytes/words in buffer to transmit.
     */
    void write(const void * buffer, uint32 length);

    // Some libraries (like recent Adafruit graphics libraries) require
    // the write function be availabe under the name transfer, so here it is:
    inline void transfer(const void * buffer, uint32 length)
    {
        write(buffer, (uint32)length);
    }

    /**
     * @brief Transmit a byte, then return the next unread byte.
     *
     * This function transmits before receiving.
     *
     * @param data Byte to transmit.
     * @return Next unread byte.
     */
    uint8 transfer(uint8 data) const;
    uint16_t transfer16(uint16_t data) const;
    void transfer(const uint8_t * tx_buf, uint8_t * rx_buf, uint32 len);
    void transfer(uint8_t * trx_buf, uint32 len)
    {
        transfer((const uint8_t *)trx_buf, trx_buf, len);
    }


    /**
     * @brief Sets up a DMA Transfer for "length" bytes.
     * The transfer mode (8 or 16 bit mode) is evaluated from the SPI peripheral setting.
     *
     * This function transmits and receives to buffers.
     *
     * @param transmitBuf buffer Bytes to transmit. If passed as 0, it sends FF repeatedly for "length" bytes
     * @param receiveBuf buffer Bytes to save received data.
     * @param length Number of bytes in buffer to transmit.
     */
    uint8 dmaTransfer(const void * transmitBuf, void * receiveBuf, uint16 length);
    uint8 dmaTransfer(const uint16 value, void * receiveBuf, uint16 length);
    void dmaTransferSet(const void *transmitBuf, void *receiveBuf);
    uint8 dmaTransferRepeat(uint16 length);

    /**
     * @brief Sets up a DMA Transmit for SPI 8 or 16 bit transfer mode.
     * The transfer mode (8 or 16 bit mode) is evaluated from the SPI peripheral setting.
     *
     * This function only transmits and does not care about the RX fifo.
     *
     * @param data buffer half words to transmit,
     * @param length Number of bytes in buffer to transmit.
     * @param minc Set to use Memory Increment mode, clear to use Circular mode.
     */
    uint8 dmaSend(const void * transmitBuf, uint16 length, bool minc = 1);
    void dmaSendSet(const void * transmitBuf, bool minc);
    uint8 dmaSendRepeat(uint16 length);

    uint8 dmaSendAsync(const void * transmitBuf, uint16 length, bool minc = 1);
    /*
     * Pin accessors
     */

    /**
     * @brief Return the number of the MISO (master in, slave out) pin
     */
    uint8 misoPin(void);

    /**
     * @brief Return the number of the MOSI (master out, slave in) pin
     */
    uint8 mosiPin(void);

    /**
     * @brief Return the number of the SCK (serial clock) pin
     */
    uint8 sckPin(void);

    /**
     * @brief Return the number of the NSS (slave select) pin
     */
    uint8 nssPin(void);

    /* Escape hatch */

    /**
     * @brief Get a pointer to the underlying libmaple spi_dev for
     *        this HardwareSPI instance.
     */
    spi_dev* c_dev(void)
    {
        return _currentSetting->spi_d;
    }

    spi_dev *dev()
    {
        return _currentSetting->spi_d;
    }

    /**
    * @brief Sets the number of the SPI peripheral to be used by
    *        this HardwareSPI instance.
    *
    * @param spi_num Number of the SPI port. 1-2 in low density devices
    *           or 1-3 in high density devices.
    */
    void setModule(int spi_num)
    {
        _currentSetting = &_settings[spi_num - 1]; // SPI channels are called 1 2 and 3 but the array is zero indexed
    }

    /* -- The following methods are deprecated --------------------------- */

    /**
     * @brief Deprecated.
     *
     * Use HardwareSPI::transfer() instead.
     *
     * @see HardwareSPI::transfer()
     */
    uint8 send(uint8 data);

    /**
     * @brief Deprecated.
     *
     * Use HardwareSPI::write() in combination with
     * HardwareSPI::read() (or HardwareSPI::transfer()) instead.
     *
     * @see HardwareSPI::write()
     * @see HardwareSPI::read()
     * @see HardwareSPI::transfer()
     */
    uint8 send(uint8 *data, uint32 length);

    /**
     * @brief Deprecated.
     *
     * Use HardwareSPI::read() instead.
     *
     * @see HardwareSPI::read()
     */
    uint8 recv(void);

private:

    SPISettings _settings[BOARD_NR_SPI];
    SPISettings *_currentSetting;

    void updateSettings(void);
    /*
    * Functions added for DMA transfers with Callback.
    * Experimental.
    */

    void EventCallback(void);

#if BOARD_NR_SPI >= 1
    static void _spi1EventCallback(void);
#endif
#if BOARD_NR_SPI >= 2
    static void _spi2EventCallback(void);
#endif
#if BOARD_NR_SPI >= 3
    static void _spi3EventCallback(void);
#endif
    /*
    spi_dev *spi_d;
    uint8_t _SSPin;
    uint32_t clockDivider;
    uint8_t dataMode;
    BitOrder bitOrder;
    */
};

/**
* @brief Waits unti TXE (tx empy) flag set and BSY (busy) flag unset.
*/
static inline void waitSpiTxEnd(spi_dev *spi_d)
{
    while(spi_is_tx_empty(spi_d) == 0);  // wait until TXE=1

    while(spi_is_busy(spi_d) != 0);  // wait until BSY=0
}

extern SPIClass SPI;//(1);// dummy params
#endif
