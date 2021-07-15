/*!
 * @file NeoGaugeDisplay.h
 *
 * This is part of NeoPixel-Gauge-Display library for the Arduino platform,
 * allowing a broad range of microcontroller boards (most AVR boards,
 * many ARM devices, ESP8266 and ESP32, among others) to control a
 * NeoPixel strand(s) or ring(s) to diplay one or more gauges.
 * This library is a "light" shell around the Adafruit_NeoPixel library.
 * So to use the library you also need to install the Adafruit_NeoPixel library.
 * <https://github.com/adafruit/Adafruit_NeoPixel>
 *
 * Written by Martin van den Berg,
 *
 * NeoGaugeDisplay is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * NeoGaugeDisplay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with NeoPixel.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NeoGaugeDisplay_h
#define NeoGaugeDisplay_h

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*!
 * @brief  Struct for storing gauge properties.
 * - value, the value to show on the gauge
 * - size, number of LED's in the strand
 * - startColor, color of the first gauge LED
 * - endColor, color of the last gauge LED
 * - reverse, boolean set to true will reverse the gauge on the display
 *
 * > The color of the LED's in between the first and last will transition from startColor
 * > to endColor.
 */
struct Gauge
{
  int16_t value;       /*!< gauge value to display */
  uint8_t size;        /*!< gauge size in pixels */
  uint32_t startColor; /*!< gauge first pixel color */
  uint32_t endColor;   /*!< gauge last pixel color */
  bool reverse;        /*!< gauge reverse display option */
};

/*!
  @brief  Struct for storing RGB colors
*/
typedef struct
{
  unsigned char Blue, /*!< RGB blue value */
      Green,          /*!< RGB green value */
      Red,            /*!< RGB red value */
      Alpha;          /*!< RGB alphe value (to used jet) */
} RGB;

/*!
  @brief  Color is a convenience union for setting/reading a RGB color (segment)
*/
typedef union
{
  RGB rgb;            /*!< RGB struct */
  uint32_t value;     /*!< Color as a 32 bit value */
  uint8_t array[4];   /*!< Color accessible per byte */
} Color;

static const PROGMEM uint32_t _RainbowColor[] = {
    0xFF0000, 0xFF4000, 0xFFCC00, 0x00CC00, 0x004080, 0xAA00FF};

/*!
 * @brief Main library class stores state and contains functions for
 * interacting with NeoPixels strands to create a single or multiple gauges
 * on a single stand of NeoPixels.
 *
 * This library is a "light" shell around the Adafruit_NeoPixel library.
 * So to use the library you also need to install the Adafruit_NeoPixel library.
 * <https://github.com/adafruit/Adafruit_NeoPixel>
 */
class NeoGaugeDisplay
{
public:
  void initialize(void);
  void show(void);
  uint8_t addGauge(uint8_t size);
  uint8_t addGauge(uint8_t size, uint32_t startColor, uint32_t endColor, bool reverse);

  /*!
   * @brief Constructor
   *
   * Calls private `init()` function with (default) space size of `0`
   * @param pin to connect to the NeoPixel DATA IN
  */
  NeoGaugeDisplay(uint8_t pin)
  {
    init(pin, 0);
  }

  /*!
   * @brief Constructor
   *
   * Calls private `init()` function.
   * @param  pin to connect to the NeoPixel DATA IN
   * @param  space size (number of blank pixels) between each gauge
   */
  NeoGaugeDisplay(uint8_t pin, uint8_t space)
  {
    init(pin, space);
  }

  /*!
   * @brief  Set the gauge vaule for the given gauge.
   * @param  gid gauge id of the gauge
   * @param  value value to be set
   */
  void setGaugeValue(uint8_t gid, int value)
  {
    _gauges[gid].value = value;
  }

  /*!
   * @brief Set the background color for the entire display.
   *
   * Spaces (LED's between gauges) are left off.
   * @param color of the background
   */
  void setBackgroundColor(uint32_t color)
  {
    _background.value = color;
  }

  /*!
   * @brief Set the brightness level of the display.
   *
   * Default value is `32`
   *
   * > Keep in mind that large strands of NeoPixels at high brightness will consume high currents
   * @param  level of brightness 0 ... 255
  */
  void setBrightness(uint8_t level)
  {
    _brightness = level;
    _display->setBrightness(_brightness);
  }

  /*!
   * @brief Set the gauges range.
   *
   * Default value is 1023 (`0x3FF` or 10 bits)
   *
   * > __For example:__ Lets say you have a gauge with a size of 10 pixels and
   * > the range set to 1000. When you set the gauge value
   * > (by using the `setGaugeValue()` function) to 500, 5 of the 10 gauge
   * > pixels will light up.
   *
   * @param range 0 ... 32767 (0x7FFF)
   */
  void setRange(uint16_t range)
  {
    _range = range;
  }


private:
  Adafruit_NeoPixel *_display;
  uint8_t _pin;
  uint8_t _numberOfGauges;
  uint8_t _space;
  uint8_t _brightness;
  uint16_t _range;

  Gauge _gauges[16];
  Color _background;

  void init(uint8_t pin, uint8_t space);
  uint32_t getPixelColor(Gauge gauge, uint8_t step);
  uint16_t totalNumberOfPixels(void);
};

#endif
