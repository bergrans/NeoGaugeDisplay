// A somewhat more advanved display sketch (c) 2021 Martin van den Berg
//
// Showing almost all library options
// Two gauges of 16 pixels are used, so a strand of 32 NeoPixels is needed
// or alter the size of the gauges yourself.
// Two analog inputs a used to show there value, hooking up a potentiometer
// (connected between GND and Vcc) to these analog pins will work fine.
//
// Released under the GPLv3 license

#include <NeoGaugeDisplay.h>

int neoPixelPin = 5;
int analogPinGaugeOne = A0;
int analogPinGaugeTwo = A1;

NeoGaugeDisplay display(neoPixelPin); // The actual NeoPixel gauge-display

// The two gauge we are going to add to the display.
char gaugeOne;
char gaugeTwo;

void setup()
{
  // Add gauges the advanced way
  gaugeOne = display.addGauge(16, 0x004000, 0xff0000, false); // First gauge. 16 pixels, green to red, default direction
  gaugeTwo = display.addGauge(16, 0x0000ff, 0xffffff, true);  // Second gauge. 16 pixels, blue to white, reversed direction

  // initialize the display, call this function once
  // when all gauges are added.
  display.initialize();

  // You might want to change some settings.
  // Like setting the display background color,
  // the display brightness or the range of the gauges
  // display.setBackgroundColor(0x080808); // default is 0xFFFFFF (LED's turned off)
  display.setBrightness(64); // default is 32
  display.setRange(950); // default is 1024 (10 bits)
}


void loop()
{
  display.setGaugeValue(gaugeOne, analogRead(analogPinGaugeOne));
  display.setGaugeValue(gaugeTwo, analogRead(analogPinGaugeTwo));
  display.show();
}
