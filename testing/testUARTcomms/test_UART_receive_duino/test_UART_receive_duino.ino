#include <FastLED.h>
//#include "../../datatypesShared.h" // I can't believe Arduino doesnt have a better option for this
#include "datatypesShared.h"

#define LED_PIN     5
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100
#define ONBOARD_LED 13


// Major pieces of code pulled from FastLED ColorPalette example

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

static uint8_t startIndex = 0;


void setup() {
  // put your setup code here, to run once:
  delay( 3000 ); // power-up safety delay

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  // Setup serial to receive from Simblee
  Serial.begin(SERIAL_BAUD);

  // Anything else?
  pinMode(ONBOARD_LED, OUTPUT);

  // FastLED stuff
  //  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
}

bool LED_ON = false;

void loop() {
  // put your main code here, to run repeatedly:
  // Check if anything in UART buffer
  //  if something, execute
  while (Serial.available() > 0)
  {
    int first = Serial.read();// - 48; // -48 only necessary if testing over serial
    Serial.println(first);

    // To mesure timeout value
    //Serial.println("About to read");
    int second = Serial.read();// - 48; // Also remember to unplug TX/RX line to program!!
    //Serial.println("Done waiting");
    Serial.println(second);
    
    switch (first)
    {
      case SET_MODE:
        currentPalette = RainbowColors_p;
        break;
      case SET_BRIGHTNESS:
        FastLED.setBrightness(second);

        break;
      case SET_SPEED:
        SetupTotallyRandomPalette();
        break;
      case SET_RED:
        currentPalette = myRedWhiteBluePalette_p;
        break;
      case SET_GREEN:
        SetupPurpleAndGreenPalette();
        break;
      case SET_BLUE:
        currentPalette = CloudColors_p;
        break;
      default:
        SetupBlackAndWhiteStripedPalette();
        break;
    }
    if (LED_ON)
    {
      digitalWrite(ONBOARD_LED, LOW);
      LED_ON = false;
    }
    else
    {
      digitalWrite(ONBOARD_LED, HIGH);
      LED_ON = true;
    }

  }

  // Run strip
  FillLEDsFromPaletteColors( startIndex);
startIndex = startIndex + 1; /* motion speed */

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};


