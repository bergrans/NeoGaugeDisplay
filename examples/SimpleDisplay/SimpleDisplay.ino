// Simple dual stacked gauge display sketch (c) 2021 Martin van den Berg
//
// Two gauges, of 9 pixels, are stacked with a two pixel space
// so a total of 20 pixels NeoPixels strand is needed
// or alter the size of the gauges yourself.
// Two analog inputs a used to show there value, hooking up a potentiometer
// (connected between GND and Vcc) to these analog pins will work fine.
//
// Released under the GPLv3 license

#include <NeoGaugeDisplay.h>

int neoPixelPin = 5;
int analogPinGaugeOne = A0;
int analogPinGaugeTwo = A1;

// The NeoPixel display, defined with a 2 pixel space value
NeoGaugeDisplay display(neoPixelPin, 2);

// The two gauge we are going to add to the display.
char gaugeOne;
char gaugeTwo;

void setup()
{
  // Add  your gauges the simple way, default "rainbow" colors are used
  gaugeOne = display.addGauge(9); // First gauge is 9 pixels long
  gaugeTwo = display.addGauge(9); // Second gauge is 9 pixels long

  // initialize the display, call this function once when all gauges are added.
  display.initialize();

  // You might want to change some settings.
  // Like setting the display background color,
  // the display brightness or the range of the gauges
  display.setBackgroundColor(0x404040); // default is 0xFFFFFF (LED's turned off)
  display.setBrightness(16); // default is 32
  display.setRange(900); // default is 1024 (10 bits)
}

void loop()
{
  display.setGaugeValue(gaugeOne, analogRead(analogPinGaugeOne));
  display.setGaugeValue(gaugeTwo, analogRead(analogPinGaugeTwo));
  display.show();
}
