// Single gauge display sketch (c) 2021 Martin van den Berg
//
// This example creates a single gauge showing a randon value
//
// Released under the GPLv3 license

#include <NeoGaugeDisplay.h>

int neoPixelPin = 5;

NeoGaugeDisplay display(neoPixelPin); // The actual NeoPixel gauge-display

char gauge;

void setup()
{
  // Add the gauge (size 20 pixels) to the display.
  // Default color is used, first "rainbow" color is red.
  gauge = display.addGauge(20);

  // Initialize the display, call this function once when all gauges are added.
  display.initialize();

  // Initializes the pseudo-random number generator
  randomSeed(analogRead(0));
}

void loop()
{
  display.setGaugeValue(gauge, random(500));
  display.show();
  delay(500);
}
