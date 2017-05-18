
/*
 * Electro-encephaloLED
 * ''''''''''''''''''''
 * 
 * Made at FabLab Renens, fablab-renens.ch
 * 
 * Requirements:
 * - arduino IDE 1.8.2 (as tested)
 * - IRremote by shirrif v. 2.0.1
 * - FastLED v.3.1
 * 
 * - Adafruit RGB LED strips (WS2801)
 * 
 */


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


/////////////////////////////////////////////////////////////////////////
//
// START values that you might want/need to customize
////////////////////////////////////////////////////////////////////////

// PIN assignments

#define RECV_PIN 5  // IR receiver

#define DATA_PIN 6  // LED data
#define CLOCK_PIN 7 // LED clock

#define NUM_LEDS 35 // how many LEDs in the strip

// LED mapping
// Have a look at the documentation for further information.
// prefixing with EEL_ to avoid naming conflicts with underlying libraries
#define EEL_D0X  0
#define EEL_D0  1
#define EEL_D0F 2
#define EEL_F0D 3
#define EEL_F0  4
#define EEL_F0G 5
#define EEL_G0F 6
#define EEL_G0  7
#define EEL_G0X 8
#define EEL_X0G 9
#define EEL_X0  10
#define EEL_X0D 11
#define EEL_D1X 12
#define EEL_X1D 13
#define EEL_D1  14
#define EEL_F2D 15
#define EEL_F1D 16
#define EEL_F2  17
#define EEL_F1G 18
#define EEL_G2  19
#define EEL_G1X 20
#define EEL_G2X 21
#define EEL_X1G 22
#define EEL_X2  23
#define EEL_X2D 24
#define EEL_D3  25
#define EEL_F4D 26
#define EEL_F3  27
#define EEL_F2G 28
#define EEL_F4G 29
#define EEL_G3  30
#define EEL_X4G 31
#define EEL_S 32
#define EEL_X4D 33
#define EEL_X3  34

// Brain / functions Zones
 
#define NUM_ZONES 4 // do not forget to change this if you add new zones

#define TOUCH 0
#define HEAR 1
#define SMELL 2
#define VISION 3

CRGB zoneColors[NUM_ZONES] = {
    CRGB::Gold, // touch
    CRGB::Red, // hear
    CRGB::Green, // smell
    CRGB::Blue // vision
};

// Initial state for the zones
bool isZoneActive[NUM_ZONES] = {false, false, false, false}; // do not forget to initialize this as required if you add new zones

// refresh frequency
#define FRAMES_PER_SECOND  120

// LED brightness
#define BRIGHTNESS          96

// codes received by the remote
#define BTN_1 0xFD08F7
#define BTN_2 0xFD8877
#define BTN_3 0xFD48B7
#define BTN_4 0xFD28D7
#define BTN_5 0xFDA857
#define BTN_6 0xFD6897

// uncomment this for debug mode (which basically displays some information to the console, but can slow down the execution)
//#define EEL_DEBUG


/////////////////////////////////////////////////////////////////////////
//
// END values that you might want/need to customize
////////////////////////////////////////////////////////////////////////




#define NO_ZONE -1

#define MODE_ZONES 0 // when in this mode, you can set each zone on/off individually
#define MODE_CRAZY 1 // when in this mode, all LEDs are lit randomly

// just for convenience
#define array_size(x)       (sizeof(x) / sizeof(x[0]))

// declara the array of LEDs
CRGB leds[NUM_LEDS];


// include IR library
#include <IRremote.h>

// init the IR reveiver object
IRrecv irrecv(RECV_PIN);

// structure to receive IR signals
decode_results results;

byte mode = MODE_ZONES;




int touch[] = {EEL_S, EEL_F4G, EEL_F4D, EEL_F2G, EEL_F2D};
int hear[] = {EEL_G2,EEL_D1};
int smell[] = {EEL_F0, EEL_F0G, EEL_F0D};
int vision[] = {EEL_X0, EEL_X0D, EEL_X0G, EEL_X1G, EEL_D1X};


// THINGS TO DO WHEN THE MICROCONTROLLER STARTS
void setup() {

  #ifdef EEL_DEBUG
    Serial.begin(9600);
  #endif
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  FastLED.setBrightness(BRIGHTNESS);

  // if short on power from the power supply, you can use this to limit the current automatically:
  // in this case, to 1A at 5v.
  // FastLED.setMaxPowerInVoltsAndMilliamps(5,1000); 
  
  FastLED.clear(); // Set all LEDs to black (off)

  irrecv.enableIRIn(); // Start the receiver

}

// MAIN LOOP
void loop() {
    
    // check if the IR receiver is getting a signal
    if (irrecv.decode(&results)) {

      int zoneButton = NO_ZONE; // init to NO_ZONE
      
      // Check if the signal is one of the buttons we defined
      if(results.value == BTN_1) {
        zoneButton = TOUCH;
        mode = MODE_ZONES;
      }  
      else if (results.value == BTN_2) {
        zoneButton = HEAR;
        mode = MODE_ZONES;
      }
      else if (results.value == BTN_3) {
        zoneButton = SMELL;
        mode = MODE_ZONES;
      }
      else if (results.value == BTN_4) {
        zoneButton = VISION;
        mode = MODE_ZONES;
      }      
      else if (results.value == BTN_6) {
        if (mode == MODE_CRAZY) {
          mode = MODE_ZONES;
          FastLED.clear();
        }
        else {
          mode = MODE_CRAZY;
        }
      }
          
      #ifdef EEL_DEBUG
        Serial.print("Received IR code: ");
        Serial.print(results.value, HEX);
        Serial.print("; zone: ");
        Serial.println(zoneButton);
        Serial.print(" (currently ");
        Serial.print(isZoneActive[zoneButton] == true ? "true" : "false");
        Serial.println(") ");
      #endif
      
      if (zoneButton != NO_ZONE) {
        if (false == isZoneActive[zoneButton]) {
          isZoneActive[zoneButton] = true;
        }
        else {
          isZoneActive[zoneButton] = false;
        }
      }
      irrecv.resume(); // Receive the next value
    }

    if (mode == MODE_ZONES) {
      for(int i = 0; i < NUM_ZONES; i++) {
        if(true == isZoneActive[i]) {
          setGroup(ledIds(i), ledsCount(i), zoneColors[i], 5, true);
        }
        else if (false == isZoneActive[i]) {
          setGroup(ledIds(i), ledsCount(i), CRGB::Black, 0, false);
        }
      }
    }
    
    EVERY_N_MILLISECONDS( 50 ) {
       if (mode == MODE_CRAZY) {
        for(int i = 0; i < NUM_LEDS; i++) {
          int randomIndex = random(4);
          CRGB color;
          switch(randomIndex) {
            case 0:
              color = CRGB::Cyan;
              break;
            case 1:
              color = CRGB::Red;
              break;
            case 2:
            case 3:
              color = CRGB::Black;
              break;
          }
          leds[i] = color;
        }
      }
    }
    
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND); 

}

void setGroup(int group[], int groupSize, CRGB color, fract8 chanceOfGlitter, bool active) {
  for(int i=0; i< groupSize; i++) {

    if(active == true) {
      if (random8() < chanceOfGlitter) {
        leds[ group[i]] += CRGB::White;
      }
      else {
        leds[group[i]] = color;
      }  
    } 
    else {
      leds[group[i]] = CRGB::Black; 
    }
  }
}

int* ledIds(int zone) {
  
  switch(zone) {
    case TOUCH:
      return touch;
      break;
    case HEAR:
      return hear;
      break;
    case SMELL:
      return smell;
      break;
    case VISION:
      return vision;
      break;
  }

}
// TODO: cleaner and more clever way to do this. 
int ledsCount(int zone) {
    switch(zone) {
    case TOUCH:
      return array_size(touch);
      break;
    case HEAR:
      return array_size(hear);
      break;
    case SMELL:
      return array_size(smell);
      break;
    case VISION:
      return array_size(vision);
      break;
  }
}


