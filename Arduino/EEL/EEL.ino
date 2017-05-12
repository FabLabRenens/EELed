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

// LED stuff
#define NUM_LEDS 35
#define DATA_PIN 6
#define CLOCK_PIN 7
// Mapping the LEDs id to the name used in EEL

#define S     12
#define F4G   22
#define F4D   23
#define F2G   27
#define F2D   29


//CRGB physicalLEDsSensoryCortex[] = {S, F4G, F4D, F2G, F2D};
// Definition of groups/areas
//CRGBSet sensoryCortex(physicalLEDsSensoryCortex, 5);

CRGB leds[NUM_LEDS];


// IR stuff
#include <IRremote.h>

int RECV_PIN = 5;

IRrecv irrecv(RECV_PIN);

decode_results results;



#define DEBUG


//TEMP: for beat sample
uint8_t bpm = 80;
int8_t fadeval = 114; 

void setup() {

  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  //FastLED.addLeds<WS2801, RGB>(sensoryCortex, NUM_LEDS);
  
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  //TODO: use this:
  //FastLED.setBrightness(max_bright);
  //set_max_power_in_volts_and_milliamps(5, 500);


  irrecv.enableIRIn(); // Start the receiver
  FastLED.clear();
}

void loop() {
  // PARSE INPUTS
  /////////////////
    if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if(results.value == 0xFD08F7) {
      
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        leds[2] = CRGB::Red;
        leds[3] = CRGB::Red;
        leds[4] = CRGB::Red;
        leds[5] = CRGB::Red;
        leds[6] = CRGB::Red;
    }
    else if (results.value == 0xFD8877) {
       
        leds[0] = CRGB::Blue;
        leds[1] = CRGB::Blue;
        leds[2] = CRGB::Blue;
        leds[3] = CRGB::Blue;
        leds[4] = CRGB::Blue;
        leds[5] = CRGB::Blue;
        leds[6] = CRGB::Blue;
    }
/*FFFFFFFF
FD8877
FD48B7
FD28D7
FDA857
FFFFFFFF
FD6897
*/
    irrecv.resume(); // Receive the next value
  }
     dot_beat();
    FastLED.show();
  
  // ACTION
  /////////////////
  

}
void dot_beat() {

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Red;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Green;

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()
