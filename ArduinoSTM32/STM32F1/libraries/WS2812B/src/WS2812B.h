/*--------------------------------------------------------------------
  The WS2812B library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  See <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef WS2812B_H
#define WS2812B_H

#include <Arduino.h>
/*
 * old version used 3 separate tables, one per byte of the 24 bit encoded data
 *
static const uint8_t byte0Lookup[256]={0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB};
static const uint8_t byte1Lookup[256]={0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D};
static const uint8_t byte2Lookup[256]={0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6};
*/

// New version uses one large LUT as its faster index into sequential bytes for the GRB pattern
static uint8_t encoderLookup[256 * 3] = {   0x92, 0x49, 0x24, 0x92, 0x49, 0x26, 0x92, 0x49, 0x34, 0x92, 0x49, 0x36, 0x92, 0x49, 0xA4, 0x92, 0x49, 0xA6, 0x92, 0x49, 0xB4, 0x92, 0x49, 0xB6, 0x92, 0x4D, 0x24, 0x92, 0x4D, 0x26, 0x92, 0x4D, 0x34, 0x92, 0x4D, 0x36, 0x92, 0x4D, 0xA4, 0x92, 0x4D, 0xA6, 0x92, 0x4D, 0xB4, 0x92, 0x4D, 0xB6, 0x92, 0x69, 0x24, 0x92, 0x69, 0x26, 0x92, 0x69, 0x34, 0x92, 0x69, 0x36, 0x92, 0x69, 0xA4, 0x92, 0x69, 0xA6, 0x92, 0x69, 0xB4, 0x92, 0x69, 0xB6, 0x92, 0x6D, 0x24, 0x92, 0x6D, 0x26, 0x92, 0x6D, 0x34, 0x92, 0x6D, 0x36, 0x92, 0x6D, 0xA4, 0x92, 0x6D, 0xA6, 0x92, 0x6D, 0xB4, 0x92, 0x6D, 0xB6, 0x93, 0x49, 0x24, 0x93, 0x49, 0x26, 0x93, 0x49, 0x34, 0x93, 0x49, 0x36, 0x93, 0x49, 0xA4, 0x93, 0x49, 0xA6, 0x93, 0x49, 0xB4, 0x93, 0x49, 0xB6, 0x93, 0x4D, 0x24, 0x93, 0x4D, 0x26, 0x93, 0x4D, 0x34, 0x93, 0x4D, 0x36, 0x93, 0x4D, 0xA4, 0x93, 0x4D, 0xA6, 0x93, 0x4D, 0xB4, 0x93, 0x4D, 0xB6, 0x93, 0x69, 0x24, 0x93, 0x69, 0x26, 0x93, 0x69, 0x34, 0x93, 0x69, 0x36, 0x93, 0x69, 0xA4, 0x93, 0x69, 0xA6, 0x93, 0x69, 0xB4, 0x93, 0x69, 0xB6, 0x93, 0x6D, 0x24, 0x93, 0x6D, 0x26, 0x93, 0x6D, 0x34, 0x93, 0x6D, 0x36, 0x93, 0x6D, 0xA4, 0x93, 0x6D, 0xA6, 0x93, 0x6D, 0xB4, 0x93, 0x6D, 0xB6, 0x9A, 0x49, 0x24, 0x9A, 0x49, 0x26, 0x9A, 0x49, 0x34, 0x9A, 0x49, 0x36, 0x9A, 0x49, 0xA4, 0x9A, 0x49, 0xA6, 0x9A, 0x49, 0xB4, 0x9A, 0x49, 0xB6, 0x9A, 0x4D, 0x24, 0x9A, 0x4D, 0x26, 0x9A, 0x4D, 0x34, 0x9A, 0x4D, 0x36, 0x9A, 0x4D, 0xA4, 0x9A, 0x4D, 0xA6, 0x9A, 0x4D, 0xB4, 0x9A, 0x4D, 0xB6, 0x9A, 0x69, 0x24, 0x9A, 0x69, 0x26, 0x9A, 0x69, 0x34, 0x9A, 0x69, 0x36, 0x9A, 0x69, 0xA4, 0x9A, 0x69, \
                                            0xA6, 0x9A, 0x69, 0xB4, 0x9A, 0x69, 0xB6, 0x9A, 0x6D, 0x24, 0x9A, 0x6D, 0x26, 0x9A, 0x6D, 0x34, 0x9A, 0x6D, 0x36, 0x9A, 0x6D, 0xA4, 0x9A, 0x6D, 0xA6, 0x9A, 0x6D, 0xB4, 0x9A, 0x6D, 0xB6, 0x9B, 0x49, 0x24, 0x9B, 0x49, 0x26, 0x9B, 0x49, 0x34, 0x9B, 0x49, 0x36, 0x9B, 0x49, 0xA4, 0x9B, 0x49, 0xA6, 0x9B, 0x49, 0xB4, 0x9B, 0x49, 0xB6, 0x9B, 0x4D, 0x24, 0x9B, 0x4D, 0x26, 0x9B, 0x4D, 0x34, 0x9B, 0x4D, 0x36, 0x9B, 0x4D, 0xA4, 0x9B, 0x4D, 0xA6, 0x9B, 0x4D, 0xB4, 0x9B, 0x4D, 0xB6, 0x9B, 0x69, 0x24, 0x9B, 0x69, 0x26, 0x9B, 0x69, 0x34, 0x9B, 0x69, 0x36, 0x9B, 0x69, 0xA4, 0x9B, 0x69, 0xA6, 0x9B, 0x69, 0xB4, 0x9B, 0x69, 0xB6, 0x9B, 0x6D, 0x24, 0x9B, 0x6D, 0x26, 0x9B, 0x6D, 0x34, 0x9B, 0x6D, 0x36, 0x9B, 0x6D, 0xA4, 0x9B, 0x6D, 0xA6, 0x9B, 0x6D, 0xB4, 0x9B, 0x6D, 0xB6, 0xD2, 0x49, 0x24, 0xD2, 0x49, 0x26, 0xD2, 0x49, 0x34, 0xD2, 0x49, 0x36, 0xD2, 0x49, 0xA4, 0xD2, 0x49, 0xA6, 0xD2, 0x49, 0xB4, 0xD2, 0x49, 0xB6, 0xD2, 0x4D, 0x24, 0xD2, 0x4D, 0x26, 0xD2, 0x4D, 0x34, 0xD2, 0x4D, 0x36, 0xD2, 0x4D, 0xA4, 0xD2, 0x4D, 0xA6, 0xD2, 0x4D, 0xB4, 0xD2, 0x4D, 0xB6, 0xD2, 0x69, 0x24, 0xD2, 0x69, 0x26, 0xD2, 0x69, 0x34, 0xD2, 0x69, 0x36, 0xD2, 0x69, 0xA4, 0xD2, 0x69, 0xA6, 0xD2, 0x69, 0xB4, 0xD2, 0x69, 0xB6, 0xD2, 0x6D, 0x24, 0xD2, 0x6D, 0x26, 0xD2, 0x6D, 0x34, 0xD2, 0x6D, 0x36, 0xD2, 0x6D, 0xA4, 0xD2, 0x6D, 0xA6, 0xD2, 0x6D, 0xB4, 0xD2, 0x6D, 0xB6, 0xD3, 0x49, 0x24, 0xD3, 0x49, 0x26, 0xD3, 0x49, 0x34, 0xD3, 0x49, 0x36, 0xD3, 0x49, 0xA4, 0xD3, 0x49, 0xA6, 0xD3, 0x49, 0xB4, 0xD3, 0x49, 0xB6, 0xD3, 0x4D, 0x24, 0xD3, 0x4D, 0x26, 0xD3, 0x4D, 0x34, 0xD3, \
                                            0x4D, 0x36, 0xD3, 0x4D, 0xA4, 0xD3, 0x4D, 0xA6, 0xD3, 0x4D, 0xB4, 0xD3, 0x4D, 0xB6, 0xD3, 0x69, 0x24, 0xD3, 0x69, 0x26, 0xD3, 0x69, 0x34, 0xD3, 0x69, 0x36, 0xD3, 0x69, 0xA4, 0xD3, 0x69, 0xA6, 0xD3, 0x69, 0xB4, 0xD3, 0x69, 0xB6, 0xD3, 0x6D, 0x24, 0xD3, 0x6D, 0x26, 0xD3, 0x6D, 0x34, 0xD3, 0x6D, 0x36, 0xD3, 0x6D, 0xA4, 0xD3, 0x6D, 0xA6, 0xD3, 0x6D, 0xB4, 0xD3, 0x6D, 0xB6, 0xDA, 0x49, 0x24, 0xDA, 0x49, 0x26, 0xDA, 0x49, 0x34, 0xDA, 0x49, 0x36, 0xDA, 0x49, 0xA4, 0xDA, 0x49, 0xA6, 0xDA, 0x49, 0xB4, 0xDA, 0x49, 0xB6, 0xDA, 0x4D, 0x24, 0xDA, 0x4D, 0x26, 0xDA, 0x4D, 0x34, 0xDA, 0x4D, 0x36, 0xDA, 0x4D, 0xA4, 0xDA, 0x4D, 0xA6, 0xDA, 0x4D, 0xB4, 0xDA, 0x4D, 0xB6, 0xDA, 0x69, 0x24, 0xDA, 0x69, 0x26, 0xDA, 0x69, 0x34, 0xDA, 0x69, 0x36, 0xDA, 0x69, 0xA4, 0xDA, 0x69, 0xA6, 0xDA, 0x69, 0xB4, 0xDA, 0x69, 0xB6, 0xDA, 0x6D, 0x24, 0xDA, 0x6D, 0x26, 0xDA, 0x6D, 0x34, 0xDA, 0x6D, 0x36, 0xDA, 0x6D, 0xA4, 0xDA, 0x6D, 0xA6, 0xDA, 0x6D, 0xB4, 0xDA, 0x6D, 0xB6, 0xDB, 0x49, 0x24, 0xDB, 0x49, 0x26, 0xDB, 0x49, 0x34, 0xDB, 0x49, 0x36, 0xDB, 0x49, 0xA4, 0xDB, 0x49, 0xA6, 0xDB, 0x49, 0xB4, 0xDB, 0x49, 0xB6, 0xDB, 0x4D, 0x24, 0xDB, 0x4D, 0x26, 0xDB, 0x4D, 0x34, 0xDB, 0x4D, 0x36, 0xDB, 0x4D, 0xA4, 0xDB, 0x4D, 0xA6, 0xDB, 0x4D, 0xB4, 0xDB, 0x4D, 0xB6, 0xDB, 0x69, 0x24, 0xDB, 0x69, 0x26, 0xDB, 0x69, 0x34, 0xDB, 0x69, 0x36, 0xDB, 0x69, 0xA4, 0xDB, 0x69, 0xA6, 0xDB, 0x69, 0xB4, 0xDB, 0x69, 0xB6, 0xDB, 0x6D, 0x24, 0xDB, 0x6D, 0x26, 0xDB, 0x6D, 0x34, 0xDB, 0x6D, 0x36, 0xDB, 0x6D, 0xA4, 0xDB, 0x6D, 0xA6, 0xDB, 0x6D, 0xB4, 0xDB, 0x6D, 0xB6
                                        };

class WS2812B
{
public:

    // Constructor: number of LEDs
    WS2812B(uint16_t number_of_leds); // Constuctor
    ~WS2812B();
    void
    begin(void),
          show(void),
          setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
          //   setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w),
          setPixelColor(uint16_t n, uint32_t c),
          setBrightness(uint8_t),
          clear(),
          updateLength(uint16_t n);
    uint8_t
    //   *getPixels(void) const,
    getBrightness(void) const;
    uint16_t
    numPixels(void) const;
    static uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b),
          Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    // uint32_t
    //   getPixelColor(uint16_t n) const;
    inline bool
    canShow(void)
    {
        return (micros() - endTime) >= 300L;
    }

private:

    boolean
    begun;         // true if begin() previously called
    uint16_t
    numLEDs,       // Number of RGB LEDs in strip
    numBytes;      // Size of 'pixels' buffer

    uint8_t
    brightness,
    *pixels,        // Holds the current LED color values, which the external API calls interact with 9 bytes per pixel + start + end empty bytes
    *doubleBuffer,   // Holds the start of the double buffer (1 buffer for async DMA transfer and one for the API interaction.
    rOffset,       // Index of red byte within each 3- or 4-byte pixel
    gOffset,       // Index of green byte
    bOffset,       // Index of blue byte
    wOffset;       // Index of white byte (same as rOffset if no white)
    uint32_t
    endTime;       // Latch timing reference
};


#endif // WS2812B_H
