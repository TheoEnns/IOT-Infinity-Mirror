#pragma once

#include <Wire.h>
#include <SPI.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"

KEYPAD keypad1; 
int tempButtonValue = 0;
bool hashFlag = false;
bool starFlag = false;

void setupKeypad(){
  if (keypad1.begin() == false)   // Note, using begin() like this will use default I2C address, 0x4B. 
    // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring.");
  }
  Serial.print("Keypad Initialized. Firmware Version: ");
  Serial.println(keypad1.getVersion());
}

bool checkButtons(){
  keypad1.updateFIFO(); 
  char button = keypad1.getButton();
  if (button == -1)
  {
    Serial.println("No keypad detected. Wire disconnect?");
    // commandEvent = false; // leave this off to let loop consume the event
  }
  else if (button != 0)
  {
    if (button == '#') {
      Serial.print("Press Event: ");
      Serial.println("#");
      commandCode = tempButtonValue;
      tempButtonValue = 0;
      commandEvent = true;
    }else if (button == '*') {
      Serial.print("Press Event: ");
      Serial.println("*");
      commandCode = currentChoice!=8? 8: 0;
      tempButtonValue = 0;
      commandEvent = true;
    }else{ 
      int buttonValue = constrain(button-48,0,9);
      Serial.print("Press Event: ");
      Serial.println(buttonValue);
      tempButtonValue = tempButtonValue*10 + buttonValue;
    }
  }else{
    // buttonPressEvent = false; // leave this off to let loop consume the event
  }
  
  return commandEvent;
}
