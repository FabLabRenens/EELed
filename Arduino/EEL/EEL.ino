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
// prefixing with EEL_ to avoid naming conflicts with underlying libraries
#define EEL_D0X	0
#define EEL_D0	1
#define EEL_D0F	2
#define EEL_F0D	3
#define EEL_F0	4
#define EEL_F0G	5
#define EEL_G0F	6
#define EEL_G0	7
#define EEL_G0X	8
#define EEL_X0G	9
#define EEL_X0	10
#define EEL_X0D	11
#define EEL_D1X	12
#define EEL_X1D	13
#define EEL_D1	14
#define EEL_F2D	15
#define EEL_F1D	16
#define EEL_F2	17
#define EEL_F1G	18
#define EEL_G2	19
#define EEL_G1X	20
#define EEL_G2X	21
#define EEL_X1G	22
#define EEL_X2	23
#define EEL_X2D	24
#define EEL_D3	25
#define EEL_F4D	26
#define EEL_F3	27
#define EEL_F2G	28
#define EEL_F4G	29
#define EEL_G3	30
#define EEL_X4G	31
#define EEL_S	32
#define EEL_X4D	33
#define EEL_X3	34

// just for code readabilty and convenience
#define NO_ZONE -1
#define TOUCH 0
#define HEAR 1
#define SMELL 2
#define VISION 3


CRGB zoneColors[4] = {
    CRGB::Gold, // touch
    CRGB::Red, // hear
    CRGB::Green, // smell
    CRGB::Blue // vision
};

bool isZoneActive[4] = {false, false, false, false};





#define array_size(x)       (sizeof(x) / sizeof(x[0]))
// codes sent by the remote
#define BTN_1 0xFD08F7
#define BTN_2 0xFD8877
#define BTN_3 0xFD48B7
#define BTN_4 0xFD28D7
#define BTN_5 0xFDA857
#define BTN_6 0xFD6897

//CRGBSet sensoryCortex(physicalLEDsSensoryCortex, 5);

CRGB leds[NUM_LEDS];


// IR stuff
#include <IRremote.h>

int RECV_PIN = 5;

IRrecv irrecv(RECV_PIN);

decode_results results;



#define EEL_DEBUG


int touch[] = {EEL_S, EEL_F4G, EEL_F4D, EEL_F2G, EEL_F2D};
int hear[] = {EEL_G2,EEL_D1};
int smell[] = {EEL_F0, EEL_F0G, EEL_F0D};
int vision[] = {EEL_X0, EEL_X0D, EEL_X0G, EEL_X1G, EEL_D1X};

//TEMP: for beat sample
/*uint8_t bpm = 80;
int8_t fadeval = 114; 
*/
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

    if (irrecv.decode(&results)) {

      int zoneButton = NO_ZONE;
      if(results.value == BTN_1) {
        zoneButton = TOUCH;
      }  
      else if (results.value == BTN_2) {
        zoneButton = HEAR;
      }
      else if (results.value == BTN_3) {
        zoneButton = SMELL;
      }
      else if (results.value == BTN_4) {
        zoneButton = VISION;
      }        
    
      #ifdef EEL_DEBUG
        Serial.print("received code: ");
        Serial.print(results.value, HEX);
        Serial.print("; zone: ");
        Serial.print(" (currently ");
        Serial.print(isZoneActive[zoneButton] == true ? "true" : "false");
        Serial.print(") ");
        Serial.println(zoneButton);
      #endif
      
      if (zoneButton != NO_ZONE) {
        if (false == isZoneActive[zoneButton]) {
          setGroup(ledIds(zoneButton), ledsCount(zoneButton), zoneColors[zoneButton]);
          isZoneActive[zoneButton] = true;
        }
        else {
          setGroup(ledIds(zoneButton), ledsCount(zoneButton), CRGB::Black);
          isZoneActive[zoneButton] = false;
        }
      }
        

      irrecv.resume(); // Receive the next value
    }
     //dot_beat();
    FastLED.show();
  

}

void setGroup(int group[], int groupSize, CRGB color) {
  for(int i=0; i< groupSize; i++) {
    #ifdef EEL_DEBUG
      Serial.print("leds in this group: ");
      Serial.println(groupSize);
      Serial.print("setting led with index ");
      Serial.print(i);
      Serial.print(" and ID:");
      Serial.println(group[i]);
    #endif
    leds[group[i]] = color;
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

/*
void dot_beat() {

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB( 255, 0, 0); //CRGB::Red;
  leds[inner] = CRGB(240,0,0);
  leds[outer] = CRGB(255, 153, 0);

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()
*/
