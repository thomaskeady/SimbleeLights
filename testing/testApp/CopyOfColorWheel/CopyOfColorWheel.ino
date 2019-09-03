/*
The sketch receives a Bluetooth Low Energy 4 connection,
then uses PWM on the RGB shield to approximate the color
selected.

This sketch is used with SimbleeForMobile.  The sketch will maintain
the color selected when disconnected, and can be easily
modified when reconnected.
*/

/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

#include <SimbleeForMobile.h>

// pin 2 on the RGB shield is the red led
int led1 = 2;
// pin 3 on the RGB shield is the green led
int led2 = 3;
// pin 4 on the RGB shield is the blue led
int led3 = 4;

// current color (start with black)
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

void setup() {
  // setup the leds for output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // set initial color
  analogWrite(led1, red);
  analogWrite(led2, green);
  analogWrite(led3, blue);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  SimbleeForMobile.advertisementData = "rgb";

  // use a shared cache
  SimbleeForMobile.domain = "Simblee.com";

  SimbleeForMobile.begin();
}

void loop() {
  // switch to lower power mode
  // todo: Simblee_ULPDelay(INFINITE);

  // process must be called in the loop for SimbleeForMobile
  SimbleeForMobile.process();
}

uint8_t rtextfield;
uint8_t rslider;
uint8_t gtextfield;
uint8_t gslider;
uint8_t btextfield;
uint8_t bslider;
uint8_t swatch;
uint8_t color_wheel;

void update();

void ui()
{
  color_t darkgray = rgb(85, 85, 85);

  SimbleeForMobile.beginScreen(darkgray);

  // put the controls at the top, so the keyboard doesn't cover them up

  int sliderLeftPad = 25;
  int sliderXstart = 110;
  int sliderTextXPad = 6;
  int sliderTextWidth = 30;
  int sliderWidth = 175;
  int fieldLeftPad = 15;
  int fieldWidth = 50;
  int sliderHeight = 45;

  SimbleeForMobile.drawText(sliderLeftPad, sliderXstart + sliderTextXPad, "R:", WHITE);
  rslider = SimbleeForMobile.drawSlider(sliderLeftPad + sliderTextWidth, sliderXstart, sliderWidth, 0, 255);
  rtextfield = SimbleeForMobile.drawTextField(sliderLeftPad + sliderTextWidth + sliderWidth + fieldLeftPad, sliderXstart, fieldWidth, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(sliderLeftPad, sliderXstart + sliderTextXPad + sliderHeight, "G:", WHITE);
  gslider = SimbleeForMobile.drawSlider(sliderLeftPad + sliderTextWidth, sliderXstart + sliderHeight, sliderWidth, 0, 255);
  gtextfield = SimbleeForMobile.drawTextField(sliderLeftPad + sliderTextWidth + sliderWidth + fieldLeftPad, sliderXstart + sliderHeight, fieldWidth, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(sliderLeftPad, sliderXstart + sliderTextXPad + sliderHeight*2, "B:", WHITE);
  bslider = SimbleeForMobile.drawSlider(sliderLeftPad + sliderTextWidth, sliderXstart + sliderHeight * 2, sliderWidth, 0, 255);
  btextfield = SimbleeForMobile.drawTextField(sliderLeftPad + sliderTextWidth + sliderWidth + fieldLeftPad, sliderXstart + sliderHeight * 2, fieldWidth, 255, "", WHITE, darkgray);
  

  // border
  //SimbleeForMobile.drawRect(25, 200, 270, 40, WHITE);
  swatch = SimbleeForMobile.drawRect(0, 240, SimbleeForMobile.screenWidth, SimbleeForMobile.screenHeight - 240, BLACK);

  color_wheel = SimbleeForMobile.drawImage(COLOR_WHEEL, 12, 250);

  SimbleeForMobile.setEvents(color_wheel, EVENT_COLOR);

  // todo; color swatch

  SimbleeForMobile.endScreen();

  // populate with the current red/green/blue values
  // (this must be done after endScreen() to force it to be done each time,
  // otherwise the initial values will be populated from the cache)

  update();
}

void update()
{
  analogWrite(led1, red);
  analogWrite(led2, green);
  analogWrite(led3, blue);

  SimbleeForMobile.updateValue(rslider, red);
  SimbleeForMobile.updateValue(rtextfield, red);

  SimbleeForMobile.updateValue(gslider, green);
  SimbleeForMobile.updateValue(gtextfield, green);

  SimbleeForMobile.updateValue(bslider, blue);
  SimbleeForMobile.updateValue(btextfield, blue);

  SimbleeForMobile.updateColor(swatch, rgb(red, green, blue));
}

void ui_event(event_t &event)
{
  if (event.id == color_wheel)
  {
    red = event.red;
    green = event.green;
    blue = event.blue;
  }
  else if (event.id == rslider || event.id == rtextfield)
    red = event.value;
  else if (event.id == gslider || event.id == gtextfield)
    green = event.value;
  else if (event.id == bslider || event.id == btextfield)
    blue = event.value;

  update();
}

