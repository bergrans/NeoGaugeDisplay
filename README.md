# NeoGaugeDisplay

Arduino library to display one or more gauges on a NeoPixel strand or ring.

## Introduction

Controlling the pixels in a strand of NeoPixels is no rocket science. Using a
library like [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
is quite easy. This library is just an extentions on the Adafruit_NeoPixel library
to turn your strand of NeoPixels into a single or multiple gauge display.

This can be achieved by just a couple of code lines.
The tree examples sketches (
[SingeGaugeDisplay](https://github.com/bergrans/NeoGaugeDisplay/tree/master/examples/SingleGaugeDisplay/SingleGaugeDisplay.ino),
[SimpleDisplay](https://github.com/bergrans/NeoGaugeDisplay/blob/master/examples/SimpleDisplay/SimpleDisplay.ino),
[AdvancedOptionsDisplay](https://github.com/bergrans/NeoGaugeDisplay/blob/master/examples/AdvancedOptionsDisplay/AdvancedOptionsDisplay.ino)
) show most of the features of the library.

The diagram below shows most of the options of this library.

![Options](extras/images/options_150.png "Some options")

## Options

* Add one or more gauges to a display (a.k.a. a string of NeoPixels)
* Each gauge has it's own:
  * Size (number of pixels)
  * Start color
  * End Color
  * Direction
* Background color for all gauges
* Gauges can be space by a given number of pixels
* Gauge reading auto scaled to given range

## Limitations

* Max number of gauge is set to 16
* Use solid colored gauges (start color = end color) if speed is key
* Use a range (`setRange()`) >> gauge size

## Install

Checkout the Arduino site to read about [Importing a .zip Library](https://www.arduino.cc/en/Guide/Libraries#importing-a-zip-library).
Download and install the [latest verion of the library](https://github.com/bergrans/NeoGaugeDisplay/tags).

To use this library you also need to install the [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library.
