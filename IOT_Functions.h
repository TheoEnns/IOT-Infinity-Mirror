#pragma once

#include "WifiConnection.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Timezone.h> 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe timefeed   = Adafruit_MQTT_Subscribe(&mqtt, "time/seconds");
Adafruit_MQTT_Subscribe mirror_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/magicmirror");
Adafruit_MQTT_Publish   mirror_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/magicmirror");
bool heartbeat = false;
String commandText = "null";

/************************
 *  Timer Logic
 ************************/
const int timeZone_PST = -8;  // Pacific Standard Time (USA)
const int timeZone_PDT = -7;  // Pacific Daylight Time (USA)
double time_diff = 0; //delta offset between local clock and timezone adjusted internet time
int tsec;
int tmin;
int thour;
//double fractional_hours = 0;
//double fractional_minutes = 0;
//double fractional_seconds = 0;

/************************
 *  Timezone Information
 ************************/
 //Replace With Your Own Timezone Info
TimeChangeRule DTZ = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule STZ = {"PST", First, Sun, Nov, 2, -480};
Timezone usPacific(DTZ, STZ);

/************************
 *  IOT Callbacks
 ************************/
void timecallback(uint32_t utc_seconds) {
  heartbeat = true;
  uint32_t internet_local_time = usPacific.toLocal(utc_seconds);

//  if(diff==0){
    time_diff = (millis()/1000.0) - internet_local_time;
//  }else{
//    time_diff = (time_diff*9 + (millis()/1000 - epoch))/10;
//  }

//  Serial.print("The unix time is ");   
//  Serial.println(utc_seconds);
//  Serial.print("The millis() is ");   
//  Serial.println(millis());
//  Serial.print("The time_diff is ");   
//  Serial.println(time_diff);

  Serial.print("The internet local time is ");       // UTC is the time at Greenwich Meridian (GMT)
  Serial.print((internet_local_time  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');
  if (((internet_local_time % 3600) / 60) < 10) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((internet_local_time  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if ((internet_local_time % 60) < 10) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(internet_local_time % 60); // print the second
}

void magic_mirror_callback(char *data, uint16_t len) {
  heartbeat = true;
  commandText = data;
  commandText.toLowerCase();
  Serial.print("IOT Callback: ");
  Serial.println(commandText);
  for(int i = 0; i<num_commands; i++){
//    Serial.print("Checking: ");
//    Serial.println(myTextCommands[i]);
    if(myTextCommands[i].compareTo(commandText)==0){
      commandCode = i;
      commandEvent = true;
//      Serial.print("Set command #");
//      Serial.println(commandCode);
    }
  }
}

/************************
 *  IOT Setup
 ************************/
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void IOT_Setup() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.println("Connecting to MQTT... ");

  uint8_t retries = 1;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 tseconds...");
       mqtt.disconnect();
//       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         break;
       }
  }
  if (mqtt.connected())
    Serial.println("MQTT Connected!");
  else{
    Serial.println("MQTT Not Connected!");
    return;
  }

  timefeed.setCallback(timecallback);
  mirror_sub.setCallback(magic_mirror_callback);
  
  // Setup MQTT subscription for time feed.
//  delay(5);
  mqtt.subscribe(&timefeed);
//  delay(5);
  mqtt.subscribe(&mirror_sub);
  Serial.println("MQTT Subscribed!");
}
