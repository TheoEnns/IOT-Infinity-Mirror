#pragma once

#include <WiFiNINA.h>
int wstatus = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//// NTP CValues - Deprecated
//unsigned int localPort = 2390;      // local port to listen for UDP packets
//IPAddress dns(8, 8, 8, 8);

/************************
 *  Wifi client
 ************************/
WiFiClient client;

/************************
 *  Wifi functions
 ************************/
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void wifi_setup() {
  // check for the WiFi module:
  wstatus = WiFi.status();
  if (wstatus == WL_NO_MODULE) {
       Serial.println("Communication with WiFi module failed!");
  }

  // attempt to connect to Wifi network:
  if(wstatus != WL_CONNECTED) {
    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Please upgrade the firmware");
    }
    
    for(int i=0;i<1;i++){ // Try 1 times
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      wstatus = WiFi.begin(ssid, pass);
//      delay(500);
    }
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
}
