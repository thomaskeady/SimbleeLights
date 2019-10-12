#include <FastLED.h>
//#include "../../datatypesShared.h" // I can't believe Arduino doesnt have a better option for this
//#include "datatypesShared.h"
#include "dataTypesHardLink.h"

#define LED_PIN     5
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//#define UPDATES_PER_SECOND 100
#define ONBOARD_LED 13


// Comment this out to disable all prints
#define SERIAL_PRINT_ENABLED


// Major pieces of code pulled from FastLED ColorPalette example

int updatesPerSecond = 100;

// Coloring variables
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
extern const TProgmemPalette16 myBlackPalette_p PROGMEM;

static uint8_t startIndex = 0;

struct Color
{
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
};

Color color1;
Color color2;

// Receiving variables
int rxBuff[MAX_MESSAGE_LENGTH];

// Mode variables
bool usingPalettes = true;

void setup() {
  // put your setup code here, to run once:
  delay( 2000 ); // power-up safety delay

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

    rxBuff[0] = Serial.read();// - 48; // -48 only necessary if testing over serial
    //Serial.println(first);

    // To mesure timeout value
    rxBuff[1] = Serial.read();// - 48; // Also remember to unplug TX/RX line to program!!
    //Serial.println(second);

#ifdef SERIAL_PRINT_ENABLED
    Serial.print("Got \t");
    Serial.print(rxBuff[0], HEX);
    Serial.print("\t");
    Serial.println(rxBuff[1], HEX);
#endif //SERIAL_PRINT_ENABLED

    switch (rxBuff[0])
    {
      case SET_MODE:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("SET_MODE");
#endif
        //currentPalette = RainbowColors_p;
        setMode();
        break;
      case SET_BRIGHTNESS:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("SET_BRIGHTNESS");
#endif
        FastLED.setBrightness(rxBuff[1]);

        break;
      case SET_SPEED:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("SET_SPEED");
#endif
        updatesPerSecond = rxBuff[1];

        break;
      case SET_COLOR1:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("SET_COLOR1");
#endif
        rxBuff[2] = Serial.read();
        rxBuff[3] = Serial.read();
        color1.red = rxBuff[1];
        color1.blue = rxBuff[2];
        color1.green = rxBuff[3];
        //currentPalette = myRedWhiteBluePalette_p;
        break;
      case SET_COLOR2:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("SET_COLOR2");
#endif
        rxBuff[2] = Serial.read();
        rxBuff[3] = Serial.read();
        color2.red = rxBuff[1];
        color2.blue = rxBuff[2];
        color2.green = rxBuff[3];
        //SetupPurpleAndGreenPalette();
        break;
      default:
#ifdef SERIAL_PRINT_ENABLED
        Serial.println("******** DEFAULT ********");
#endif
        SetupBlackAndWhiteStripedPalette();
        break;
    }

    // Empty serial buffer (should only be needed during debugging)
    while (Serial.available() > 0)
    {
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("******** EXTRA BYTE ********");
#endif
      Serial.read();
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

  // Run strip (maybe dont put this here?)
  if (usingPalettes)
  {
    FillLEDsFromPaletteColors( startIndex);
  }
  else
  {
    //FillLEDsWithoutPalettes(startIndex);
  }
  startIndex = startIndex + 1; /* motion speed */

  FastLED.show();
  FastLED.delay(1000 / updatesPerSecond);

  //  #ifdef SERIAL_PRINT_ENABLED
  //  Serial.print("Curr palette = ");
  //  Serial.println((unsigned int)&currentPalette);
  //  #endif //SERIAL_PRINT_ENABLED

}


void setMode()
{

  CRGB c1 = CHSV(color1.red, color1.green, color1.blue);
  CRGB c2 = CHSV(color2.red, color2.green, color2.blue);

  switch (rxBuff[1])
  {
    case OFF:
    #ifdef SERIAL_PRINT_ENABLED
      Serial.println("OFF");
#endif
      currentPalette = myBlackPalette_p;
      usingPalettes = true;
      break;
    case SOLID:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("SOLID");
#endif
      //usingPalettes = false;
      //CRGB output  = CHSV(color1.red, color1.green, color1.blue);
      //currentPalette = CRGBPalette16(output, output, output, output, output, output, output, output, output, output, output, output, output, output, output, output);
      //currentPalette = CRGBPalette16(c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1, c1);
      fill_solid( currentPalette, 16, c1);
      break;
    case FADE:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("FADE");
#endif
      break;
    case FLASH:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("FLASH");
#endif
      break;
    case THEATER_CHASE:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("THEATER_CHASE");
#endif
      break;
    case TWO_COLOR_THEATER_CHASE:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("TWO_COLOR_THEATER_CHASE");
#endif
      break;
    case TIME_RAINBOW:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("TIME_RAINBOW");
#endif
      break;
    case SPACE_RAINBOW:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("SPACE_RAINBOW");
#endif
      break;
    case CHRISTMAS:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("CHRISTMAS");
#endif
      break;
    case SCANNER:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("SCANNER");
#endif
      break;
    case CANDLE:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("CANDLE");
#endif
      break;

    default:
#ifdef SERIAL_PRINT_ENABLED
      Serial.println("DEFAULT");
#endif
      break;

  }
}

void FillLEDsWithoutPalettes()
{

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

const TProgmemPalette16 myBlackPalette_p PROGMEM =
{
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

