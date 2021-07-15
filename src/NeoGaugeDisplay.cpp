/*!
 * @file NeoGaugeDisplay.cpp
 *
 * @mainpage Display one or more gauges on a NeoPixel strand or ring.
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for NeoGaugeDisplay library for the
 * Arduino platform, allowing a broad range of microcontroller boards
 * (most AVR boards, many ARM devices, ESP8266 and ESP32, among others)
 * to control a NeoPixels strand(s) to display one or more gauges.
 * Gauges occupy a given number of pixels on the NeoPixel strand, have default
 * or custom color (ranges) and can be reversed to display it's value in the
 * opposite direction.
 *
 * This library is a written as a "light" shell around the Adafruit_NeoPixel library.
 * So to use the library you also need to install the Adafruit_NeoPixel library.
 * <https://github.com/adafruit/Adafruit_NeoPixel>
 *
 * @section author Author
 *
 * Written by Martin van den Berg.
 *
 * @section license License
 *
 * This file is part of the NeoGaugeDisplay library.
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
 * License along with NeoPixel. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "NeoGaugeDisplay.h"

    /*!
    @brief  Private constuctor functiom for the display.
    @param  pin number for the Neopixel data.
    @param  space value defines the number of blank LED's between each gauge.
*/
    void
    NeoGaugeDisplay::init(uint8_t pin, uint8_t space)
{
  _pin = pin;
  _space = space;

  _numberOfGauges = 0;
  _background.value = 0x000000; // default background color is black (LED's turn off)
  _brightness = 32;             // default brightness set at 12.5%
  _range = 0x3ff;               // default range set at 1023
}

/*!
 * @brief Add a (basic) gauge to the display
 *
 * The next color is picked from the six "rainbow" colors (`_RainbowColor`)
 * for the start and end color. This creates a solid (no gradient) gauge.
 * The `reverse` option is set to `false`
 *
 * @param size in number of pixels
 */
uint8_t NeoGaugeDisplay::addGauge(uint8_t size)
{
  uint32_t gaugeColor = pgm_read_dword_near(_RainbowColor + (_numberOfGauges % 6));
  _gauges[_numberOfGauges] = {0, size, gaugeColor, gaugeColor, false};
  return _numberOfGauges++;
}

/*!
 * @brief Add a gauge to the display
 *
 * > The color of the LED's in between the first and last will transition from startColor
 * > to endColor.
 *
 * @param size in number of pixels
 * @param startRGBcode color value first pixel of the gauge
 * @param endRGBcode color value last pixel of the gauge
 * @param reverse boolean to reverse the direction of the gauge on the display
 */
uint8_t NeoGaugeDisplay::addGauge(uint8_t size, uint32_t startRGBcode, uint32_t endRGBcode, bool reverse)
{
  _gauges[_numberOfGauges] = {0, size, startRGBcode, endRGBcode, reverse};
  return _numberOfGauges++;
}

/*!
 * @brief Initialize the diplay after all gauges have been added.
 *
 * > This function should be only called once in the setup phase of your program.
 */
void NeoGaugeDisplay::initialize()
{
  _display = new Adafruit_NeoPixel(totalNumberOfPixels(), _pin, NEO_GRB + NEO_KHZ800);

// This is for Trinket 5V 16MHz
#if defined(__AVR_ATtiny85__)
  if (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  _display->begin();
  _display->setBrightness(_brightness);
  _display->show(); // Initialize all pixels to 'off'
}

/*!
  @brief  Shows (displays) the changes made to the gauges values.
*/
void NeoGaugeDisplay::show(void)
{
  int pixelCounter = 0;
  Color pixelColor;
  for (int i = 0; i < _numberOfGauges; i++)
  {
    Gauge gauge = _gauges[i];
    int16_t reading = map(gauge.value, 0, _range, 0, gauge.size);

    Color c;
    for (int pixel = 0; pixel < gauge.size; pixel++)
    {

      c.value = getPixelColor(gauge, pixel);

      if (gauge.reverse)
        pixelColor = (reading <= (gauge.size - pixel - 1)) ? _background : c;
      else
        pixelColor = (reading >= (pixel + 1)) ? c : _background;

      _display->setPixelColor(pixelCounter, _display->Color(pixelColor.rgb.Red, pixelColor.rgb.Green, pixelColor.rgb.Blue));
      pixelCounter++;
    }

    if (i < (_numberOfGauges - 1))
    {
      for (int s = 0; s < _space; s++)
      {
        _display->setPixelColor(pixelCounter, _display->Color(0, 0, 0));
        pixelCounter++;
      }
    }
  }
  _display->show();
}

/*!
  @brief  Private function to get the color for a given pixel (step) within a gauge
  @return uint16_t number of pixels
*/
uint32_t NeoGaugeDisplay::getPixelColor(Gauge gauge, uint8_t step)
{
  Color startColor;
  Color endColor;

  startColor.value = gauge.reverse ? gauge.endColor : gauge.startColor;
  endColor.value = gauge.reverse ? gauge.startColor : gauge.endColor;

  if (step == 0)
    return startColor.value;

  if (step == gauge.size - 1)
    return endColor.value;

  if (startColor.value == endColor.value)
    return startColor.value;

  int16_t stepRed = (endColor.rgb.Red - startColor.rgb.Red) / gauge.size;
  int16_t stepGreen = (endColor.rgb.Green - startColor.rgb.Green) / gauge.size;
  int16_t stepBlue = (endColor.rgb.Blue - startColor.rgb.Blue) / gauge.size;

  Color pixelColor;

  if (step < (gauge.size / 2))
  {
    pixelColor.rgb.Red = startColor.rgb.Red + step * stepRed;
    pixelColor.rgb.Green = startColor.rgb.Green + step * stepGreen;
    pixelColor.rgb.Blue = startColor.rgb.Blue + step * stepBlue;
  }
  else
  {
    step = gauge.size - 1 - step;
    pixelColor.rgb.Red = endColor.rgb.Red - step * stepRed;
    pixelColor.rgb.Green = endColor.rgb.Green - step * stepGreen;
    pixelColor.rgb.Blue = endColor.rgb.Blue - step * stepBlue;
  }

  return pixelColor.value;
}

/*!
  @brief  Private function to get the total number of pixels used
  by all defined gauges and there spacing
  @return uint16_t number of pixels
*/
uint16_t NeoGaugeDisplay::totalNumberOfPixels()
{
  uint16_t numberOfPixels = 0;
  for (int i = 0; i < _numberOfGauges; i++)
  {
    numberOfPixels += (_gauges[i]).size;
  }

  // add the spaces between all gauges
  numberOfPixels += (_numberOfGauges - 1) * _space;

  return numberOfPixels;
}
