#include "datatypesShared.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);
  
}

Action_t packet = SET_BRIGHTNESS;
uint8_t brightness = 128;
uint8_t higher_bright = 255;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(packet);
  //Serial.write(brightness++);
  Serial.write(brightness++);
  delay(10);
//  Serial.write(packet);
//  //Serial.write(brightness++);
//  Serial.write(higher_bright);
//  delay(1000);

}
