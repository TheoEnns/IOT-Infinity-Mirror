#include <Wire.h>
#include <SPI.h>

/************************
 *  Control Commands
 ************************/
bool commandEvent = false;
int commandCode = 0;
int currentChoice = 0;
using animation_func = void(*)(); //cycle time
void doNothing(){}
animation_func animation = doNothing;
int num_commands = 19;
String myTextCommands[] = {
  "rainbow",
  "rainbow chase",
  "clock",
  "white red",
  "white blue",
  "white green",
  "white purple",
  "white black",
  "off",
  "on",
  "chase red",
  "chase blue",
  "chase green",
  "chase purple",
  "alignment",
  "brighten",
  "dim",
  "reverse",
  "counter"
};
#include "thumbpad.h"

/************************
 *  Personal Data
 ************************/
#include "SecretConfig.h"
/* 
// Includes from tsecretConfig.h
#define AIO_SERVERPORT  1883
#define AIO_SERVER      "io.adafruit.com"
#define AIO_KEY       "Adafruit IO Key"
#define DEVICE_NAME   "Magic_Mirror_Device"
#define AIO_USERNAME  "user account"
#define tsecRET_SSID   "mySSID"
#define tsecRET_PASS   "password"
*/
// See https://io.adafruit.com/ for web server hosting
// See https://ifttt.com/applets/ for Google Assistant triggers


/************************
 *  Wifi Server
 ************************/
 #include "WifiConnection.h"

/************************
 *  IOT Subscription And Publishing
 ************************/
 #include "IOT_Functions.h"

/************************
 *  Update cycle configuration
 ************************/
 #include "Neopixel_Animation.h"
unsigned long updateCylceTime = 5; // how many millis per polling cycle
unsigned long network_interval = 5000;
unsigned long network_timer = 0;

/************************
 *  Animation Control
 ************************/
#include "Neopixel_Animation.h"

/************************
 *  connection agent
 ************************/
 // Checks for cannnectivity and starts/restarts connections if needed
bool Ensure_Connections(){
  wstatus = WiFi.status();
  if(wstatus != WL_CONNECTED) {
    mqtt.disconnect();
    wifi_setup();
  }
  if(! mqtt.connected()) {
    mqtt.disconnect();
    IOT_Setup();
  }
  return (wstatus == WL_CONNECTED && mqtt.connected());
}

/************************
 *  main program
 ************************/
void processCommands(){
  if(! commandEvent)
    return;
  commandEvent = false;
  if(commandCode<0 || commandCode>=num_commands)
    return;
  cycle = 0;
  Serial.println("switching to command: ");
  switch(commandCode){
    case 0:
      Serial.println("rainbow");
      animSpeed = 1;
      animation = rainbow;
      break;
    case 1:
      Serial.println("rainbow chase");
      animSpeed = 2;
      animation = theaterChaseRainbow;
      break;
    case 2:
      Serial.println("clock");
      anim_direction_f_cw = true;
      anim_direction_b_cw = true;
      animSpeed = 1;
      animation = Clock_Animation;
      break;
    case 3:
      Serial.println("wipe red");
      animSpeed = 1;
      colors[0] = MColor(255,0,0);
      animation = colorWipe;
      break;
    case 4:
      Serial.println("wipe blue");
      animSpeed = 1;
      colors[0] = MColor(0,0,255);
      animation = colorWipe;
      break;
    case 5:
      Serial.println("wipe green");
      animSpeed = 1;
      colors[0] = MColor(0,255,0);
      animation = colorWipe;
      break;
    case 6:
      Serial.println("wipe purple");
      animSpeed = 1;
      colors[0] = MColor(255,0,255);
      animation = colorWipe;
      break;
    case 7:
      Serial.println("wipe black");
      anim_direction_f_cw = true;
      anim_direction_b_cw = true;
      animSpeed = 1;
      colors[0] = MColor(0,0,0);
      animation = colorWipe;
      break;
    case 8:
      Serial.println("off");
      if(currentChoice != 8){
        anim_direction_f_cw = true;
        anim_direction_b_cw = true;
        animSpeed = 1;
        Set_Brightness(150);
        Clear();
        Display_Update();
        animation = doNothing;
      }
      break;
    case 9:
      Serial.println("on");
      Set_Brightness(150);
      if(currentChoice == 8 || currentChoice == 7){
        animSpeed = 1;
        animation = rainbow;
      }
      break;
    case 10:
      Serial.println("red chase");
      animSpeed = 2;
      colors[0] = MColor(255,0,0);
      animation = theaterChase;
      break;
    case 11:
      Serial.println("blue chase");
      animSpeed = 2;
      colors[0] = MColor(0,0,255);
      animation = theaterChase;
      break;
    case 12:
      Serial.println("green chase");
      animSpeed = 2;
      colors[0] = MColor(0,255,0);
      animation = theaterChase;
      break;
    case 13:
      Serial.println("purple chase");
      animSpeed = 2;
      colors[0] = MColor(255,0,255);
      animation = theaterChase;
      break;
    case 14:
      Serial.println("alingment");
      animSpeed = 1;
      animation = Red_Light_Tracing;
      break;
    case 15:
      Serial.println("brighten");
      Set_Brightness(brightness + 50);
      break;
    case 16:
      Serial.println("dim");
      Set_Brightness(brightness - 50);
      break;
    case 17:
      Serial.println("reverse");
      anim_direction_f_cw = !anim_direction_f_cw;
      anim_direction_b_cw = !anim_direction_b_cw;
      break;
    case 18:
      Serial.println("counter");
      anim_direction_b_cw = !anim_direction_b_cw;
      break;
  }
  currentChoice = commandCode;
}
 
void setup() {
  stripF.begin(); 
  stripF.show(); 
  stripF.setBrightness(brightness); 
  stripB.begin();  
  stripB.show();    
  stripB.setBrightness(brightness); 
  Serial.begin(2000000);
  delay(500);
  setupKeypad();

  Serial.println("Magic Mirror Starting ...");
  bool success = Ensure_Connections();
  if (success){
    Serial.println("Magic Mirror Started\n");
  }else{
    Serial.println("Magic Mirror Failed To Start\n");
  }

  Set_Brightness(brightness);
  network_timer = millis()+network_interval;
  heartbeat = false;
  animation = rainbow;
}

void loop() {  
  Ensure_Connections();

  mqtt.processPackets(updateCylceTime); //Delay to read packets
  checkButtons();
  processCommands();
  
  animation();

  if(network_timer < millis()){
    if(! heartbeat){
      mqtt.disconnect();
    }// Seems completely unessecary if you use heartbeat monitoring
//  else if(! mqtt.ping()) {
//    // Note pings can stop this code from working if done too often
//    mqtt.disconnect();
//  }
    heartbeat = false;
    network_timer = millis()+network_interval;
  }else if(network_timer >= millis()+network_interval*2){
    network_timer = millis()+network_interval; // rollover protection
  }
}
