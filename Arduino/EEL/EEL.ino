#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#define NUM_LEDS 35

#define DATA_PIN 3
#define CLOCK_PIN 4

#define DEBUG

// Mapping the LEDs id to the name used in EEL

#define S     12
#define F4G   22
#define F4D   23
#define F2G   27
#define F2D   29


CRGB physicalLEDsSensoryCortex[] = {S, F4G, F4D, F2G, F2D};
// Definition of groups/areas
CRGBSet sensoryCortex(physicalLEDsSensoryCortex, 5);



void setup() {

  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
  
}

void loop() {
  // PARSE INPUTS
  /////////////////
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  
  // ACTION
  /////////////////


}
