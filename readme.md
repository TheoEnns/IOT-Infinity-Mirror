

## IOT Infinity Mirror

A wall mounted infinity mirror with IOT control integrated into google home assistant using IFTTT and Adafruit IOT plugins


## Getting Started and Installing

You will need to create you own SecretConfig.h file of the format:

---
    #pragma once

    #define AIO_SERVERPORT 1883  
    #define AIO_SERVER      "io.adafruit.com"
    #define AIO_KEY       ""
    #define DEVICE_NAME ""
    #define AIO_USERNAME  ""
    #define SECRET_SSID ""
    #define SECRET_PASS ""
---


### Prerequisite parts

This project uses the following equipment:
* 3D printed parts located in the parts folder
* [Arduino Nano 33 IoT with headers Mounted](https://www.amazon.com/gp/product/B07WPFQZQ1) - Arduino Board wioth built in Wifi
* [Nano Breakout Mount](https://www.amazon.com/gp/product/B072FCNF54)
* [SparkFun Qwiic Keypad - 12 Button](https://www.sparkfun.com/products/15290) - This keypad is for manual control as backup (optional)
* 2x [WS2812B RGB 5050SMD Individual Addressable 3.3FT 144](https://www.amazon.com/gp/product/B01CDTEJR0/) - High Density LED String
* [5V 15A AC to DC Power Supply Adapter](https://www.amazon.com/gp/product/B01LXN7MN3/) - power supply
* [Semi-Transparent 12 inch acrylic mirror](https://www.amazon.com/gp/product/B07XTRC7F1/) - Van be cit via dremel to round shape. Doesn't look great though due to bending and poor clean-ability
* [Semi-Transparent 12 inch glass mirror](https://www.twowaymirrors.com/glass/) - Thanks to the great folk at Two Way Mirrors for the semi-transparent mirror! This could be replaced by cheaper acrylic two way mirror on amazon (which I initially used), but glass looks much cooler!

Some soldering and M3 screws required.


## Running the tests

Example animation commands:


    Hey Google, set mirror rainbow

    Okay Google, mirror clock

    Okay Google, mirror off

Commands can take the form(s)

    (Okay,Hey) Google, (mirror,set mirror,magic mirror) $


where the command $ includes

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


## Built With

* [IFTTT](https://ifttt.com/) - This links Google Assistant commands to push commands to Adafruit IO servers
* [Adafruit IO](https://io.adafruit.com/) - Free web proprerties hosting with dashboard plugins and http query API
* [Google Assistant](https://assistant.google.com/) - Feel free to replace with Alexa, Siri, or your prefered IOT voice system



## Authors

* **Theodore Enns** - [Check out my youtube channel!](https://www.youtube.com/channel/UCWVNKVqwlDwOcAsUMM4TJnA)

