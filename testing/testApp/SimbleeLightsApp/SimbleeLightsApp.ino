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

/*
 * TwoScreens demonstraits how multiple screens can be used in SimbleeForMobile
 *
 * The application consists of two basic screens; each containing a label and a button.
 * When the button on the first screen is pressed, the second screen is displayed
 * and vice versa.
 *
 * Button events are also registered for and acted upon in the application.
 */

#include <SimbleeForMobile.h>

#define RED_LED 2
#define GREEN_LED 3
#define BLUE_LED 4



//
// The ID of the button which displays screen 2
//
int toScreen2ButtonID;

//
// The ID of the button which displays screen 1
//
int toScreen1ButtonID;

//
// The ID of the current screen being displayed
//
int currentScreen;

/*
 * Traditional Arduino setup routine
 *
 * Initialize the SimbleeForMobile environment.
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Setup beginning");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);

  SimbleeForMobile.deviceName = "2 Screens";
  SimbleeForMobile.advertisementData = "Sample";

  // use a shared cache
  SimbleeForMobile.domain = "twoscreens.simblee.com";

  // Begin Simble UI
  SimbleeForMobile.begin();

  Serial.println("Setup completed");
}

/*
 * The traditional Arduino loop method
 *
 * Enable SimbleeForMobile functionality by calling the process method
 * each time through the loop. This method must be called regularly for
 * functionality to work.
 */
void loop() {
  // put your main code here, to run repeatedly:
  // process must be called in the loop for Simblee UI
  SimbleeForMobile.process();
}

/*
 * Callback when a Central connects to this device
 *
 * Reset the current screen to non being displayed
 */
void SimbleeForMobile_onConnect()
{
  currentScreen = -1;
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // TODO precompute screen height and width divided by 2, 3, ? to save time when rendering
}


/*
 * Callback when a Central disconnects from this device
 *
 * Not used in this sketch. Could clean up any resources
 * no longer required.
 */
void SimbleeForMobile_onDisconnect()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

/*
 * SimbleeForMobile ui callback requesting the user interface
 *
 * Check which screen is being requested.
 * If that is the current screen, simply return.
 * If it is a different screen, create the requested screen.
 * A screen request which is out of range, is logged and ignored.
 */
void ui()
{
  if (SimbleeForMobile.screen == currentScreen) return;

  currentScreen = SimbleeForMobile.screen;
  switch (SimbleeForMobile.screen)
  {
    case 1:
      createScreen1();
      break;

    case 2:
      createScreen2();
      break;

    default:
      Serial.print("ui: Uknown screen requested: ");
      Serial.println(SimbleeForMobile.screen);
  }
}

/*
 * SimbleeForMobile event callback method
 *
 * An event we've registered for has occur. Process the event.
 * The only events of interest are EVENT_RELEASE which signify a button press.
 * If the button to open the screen not being displayed is pressed, request
 * to show that screen.
 */
void ui_event(event_t &event)
{
  //  printEvent(event);
  if (event.id == toScreen1ButtonID && event.type == EVENT_RELEASE && currentScreen == 2)
  {
    SimbleeForMobile.showScreen(1);
  } else if (event.id == toScreen2ButtonID && event.type == EVENT_RELEASE && currentScreen == 1)
  {
    SimbleeForMobile.showScreen(2);
  }
}

/*
 * Create the first screen.
 *
 * The screen simply consists of a label displaying the screen number
 * and a button which swaps screens. Register for events on the button
 * such that we receive notifications when it is pressed.
 */
void createScreen1()
{
  //
  // Portrait mode is the default, so that isn't required
  // here, but shown for completeness. LANDSCAPE could be
  // used for that orientation.
  //
  SimbleeForMobile.beginScreen(GRAY, PORTRAIT);

  int butHeight = 37; // Empirically set, not sure if there is a way to change this
  int butHeightGap = 6;

  // For 3 buttons in a row
  int but3gap = 8;
  int but3width = (SimbleeForMobile.screenWidth - (but3gap * 4)) / 3;

  // For 2 buttons in a row
  int but2gap = 8;
  int but2width = (SimbleeForMobile.screenWidth - (but2gap * 3)) / 2;

  // 1 buttin in a row
  int but1gap = 8;
  int but1width = SimbleeForMobile.screenWidth - (but1gap * 2);

  //int textID = SimbleeForMobile.drawText(80, 60, "Screen 1", BLACK, 40);
  //toScreen2ButtonID = SimbleeForMobile.drawButton(100, 200, 100, "Screen 2");

  //SimbleeForMobile.drawText(SimbleeForMobile.screenWidth/2, SimbleeForMobile.screenHeight/2, "1", BLACK, 40);

  int butXstart = 105;

  // Init spacing variables
  int firstButX;
  int firstButY;
  int secondButX;
  int secondButY;
  int thirdButX;
  int thirdButY;
  int rowNum = 0;

  // Special first row for screen selector goes here
//  firstButX = butXstart;
//  firstButY = but1gap;
//  // BEGIN ROW 0
//
//  SimbleeForMobile.drawButton(firstButY, firstButX, but1width, "Set Colors", BLACK);

  //butXstart += 45;

  // END ROW 0
  //++rowNum;
  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  firstButY = but3gap;
  secondButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  secondButY = but3gap * 2 + but3width;
  thirdButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  thirdButY = but3gap * 3 + but3width * 2;

  // BEGIN ROW 1

  SimbleeForMobile.drawButton(firstButY, firstButX, but3width, "OFF", BLACK);
  SimbleeForMobile.drawButton(secondButY, secondButX, but3width, "Solid", BLACK);
  SimbleeForMobile.drawButton(thirdButY, thirdButX, but3width, "Fade", BLACK);

  // END ROW 1
  ++rowNum;
  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  firstButY = but3gap;
  secondButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  secondButY = but3gap * 2 + but3width;
  thirdButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  thirdButY = but3gap * 3 + but3width * 2;
  // BEGIN ROW 2

  SimbleeForMobile.drawButton(firstButY, firstButX, but3width, "Candle", BLACK);
  SimbleeForMobile.drawButton(secondButY, secondButX, but3width, "Flash", BLACK);
  SimbleeForMobile.drawButton(thirdButY, thirdButX, but3width, "Scanner", BLACK);

  // END ROW 2
  ++rowNum;
  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  firstButY = but2gap;
  secondButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  secondButY = but2gap * 2 + but2width;
  // BEGIN ROW 3

  SimbleeForMobile.drawButton(firstButY, firstButX, but2width, "Time Rainbow", BLACK);
  SimbleeForMobile.drawButton(secondButY, secondButX, but2width, "Theater Chase", BLACK);

  // END ROW 3
  ++rowNum;
  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  firstButY = but2gap;
  secondButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  secondButY = but2gap * 2 + but2width;
  // BEGIN ROW 4

  SimbleeForMobile.drawButton(firstButY, firstButX, but2width, "Space Rainbow", BLACK);
  SimbleeForMobile.drawButton(secondButY, secondButX, but2width, "2 Color TC", BLACK);

  // END ROW 4
  ++rowNum;
  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  firstButY = but2gap;
  secondButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  secondButY = but2gap * 2 + but2width;
  // BEGIN ROW 5

  SimbleeForMobile.drawButton(firstButY, firstButX, but2width, "4th of July", BLACK);
  SimbleeForMobile.drawButton(secondButY, secondButX, but2width, "Christmas", BLACK);

  // END ROW 5
  //++rowNum;
  //  firstButX = butXstart + butHeight * rowNum + butHeightGap * rowNum;
  //  firstButY = but1gap;
  // BEGIN ROW 6

  //  SimbleeForMobile.drawButton(firstButY, firstButX, but1width, "Set Colors", BLACK);

  // BEGIN ROW 6
  //++rowNum;

  // BEGIN ROW 7

  
// Special case for reset button
firstButX = SimbleeForMobile.screenHeight - butHeightGap - butHeight;
firstButY = but1gap;
SimbleeForMobile.drawButton(firstButY, firstButX, but1width, "Reset Arduino", BLACK);


  //
  // Receive notifications when a "release" occurs on the button.
  // Other event notifications are supported and can be combined in this call.
  // I.e. to receive press and release events you'd pass:
  //
  // EVENT_PRESS | EVENT_RELEASE
  //
  // into the function:
  //
  // SimbleeForMobile.setEvents(toScreen2ButtonID, EVENT_PRESS | EVENT_RELEASE);
  //
  // However, you don't want to register for events which are not required as
  // that results in extra traffic.
  //
  SimbleeForMobile.setEvents(toScreen2ButtonID, EVENT_RELEASE);
  SimbleeForMobile.endScreen();
}

/*
 * Create the second screen.
 *
 * The screen simply consists of a label displaying the screen number
 * and a button which swaps screens. Register for events on the button
 * such that we receive notifications when it is pressed.
 */
void createScreen2()
{
  //
  // Default to Portrait orientation
  //
  SimbleeForMobile.beginScreen(WHITE);

  int textID = SimbleeForMobile.drawText(80, 60, "Screen 2", BLACK, 40);
  toScreen1ButtonID = SimbleeForMobile.drawButton(100, 200, 100, "Screen 1");

  SimbleeForMobile.setEvents(toScreen1ButtonID, EVENT_RELEASE);
  SimbleeForMobile.endScreen();
}

/*
 * Utility method to print information regarding the given event
 */
void printEvent(event_t &event)
{
  Serial.println("Event:");
  Serial.print("  ID: ");
  Serial.println(event.id);

  Serial.print("   Type: ");
  Serial.println(event.type);

  Serial.print("   Value: ");
  Serial.println(event.value);

  Serial.print("   Text: ");
  Serial.println(event.text);

  Serial.print("   Coords: ");
  Serial.print(event.x);
  Serial.print(",");
  Serial.println(event.y);
}
