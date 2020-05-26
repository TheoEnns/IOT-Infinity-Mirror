#pragma once

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>

//Pin / LED Config
static const int FRONT_STRIP = 0;
static const int BACK_STRIP = 1;
static const int LED_PIN_FRONT = 10;
static const int LED_PIN_BACK = 9;
static const int LED_COUNT = 135;
static const int LED_Start = 2;
static const int LED_End = 135;
static const int LEDF_Offset = 0;
static const int LEDB_Offset = -4;
static const int ACTIVE_LED_COUNT = 133; // LED_COUNT - LED_Start

int brightness = 150;
float animSpeed = 1;
bool anim_direction_f_cw = true;
bool anim_direction_b_cw = true;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel stripF(LED_COUNT, LED_PIN_FRONT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB(LED_COUNT, LED_PIN_BACK, NEO_GRB + NEO_KHZ800);
long cycle = 0; // used for animation phases
double cursorWidth = 0.008333; // Size of clock cursor

uint32_t colors[24];

/************************
 *  RAW LED CONTROL
 ************************/
void Clear(){
  stripF.clear();
  stripB.clear();
}

void Set_Brightness(int _brightness){
  brightness = constrain(_brightness,0,200);    
  stripF.setBrightness(brightness); // (max = 255)
  stripB.setBrightness(brightness); // (max = 255)                     
}

int Get_Brightness(){
  return brightness ;                   
}

uint32_t MColor(int _red, int _green,int _blue){
    //  Color(255,   0,   0) // Red
    //  Color(  0, 255,   0) // Green
    //  Color(  0,   0, 255) // Blue
  return stripF.Color(
      constrain(_red,0,255),
      constrain(_green,0,255), 
      constrain(_blue,0,255)
      );
}

int index2Address(int index){
  return constrain((index)%(LED_End-LED_Start)+LED_Start,LED_Start,LED_COUNT);
}

int reverseIndex(int index){
  return constrain(ACTIVE_LED_COUNT-index,0,ACTIVE_LED_COUNT);
}

float Get_Index_From_Angle(float angle, int strip){
  if(strip == FRONT_STRIP){
    return constrain((ACTIVE_LED_COUNT * (angle/360.0f)),0,ACTIVE_LED_COUNT-1);
  }else if(strip == BACK_STRIP){
    return constrain((ACTIVE_LED_COUNT * (angle/360.0f)),0,ACTIVE_LED_COUNT-1);
  }
  return -1;
}

void Set_Pixel_By_Index(int index, int strip, uint32_t color){
  if(strip == FRONT_STRIP){
    if(!anim_direction_f_cw){
      index = ACTIVE_LED_COUNT - index;
    }
    stripF.setPixelColor(index2Address(             (index)%ACTIVE_LED_COUNT), color);
  }else if(strip == BACK_STRIP){
    if(!anim_direction_b_cw){
      index = ACTIVE_LED_COUNT - index;
    }
    stripB.setPixelColor(index2Address(reverseIndex((index+4)%ACTIVE_LED_COUNT)), color);
  }
}

void Set_Pixel_By_Angle(float angle, int strip, uint32_t color){
  float index = Get_Index_From_Angle( angle,  strip);
  Set_Pixel_By_Index(round(index), strip, color);
}

void Display_Update(){
  stripF.show();
  stripB.show();
}

int Get_MAX_Index(){
  return ACTIVE_LED_COUNT;
}

/************************
 *  Classic Neopixel Animations
 ************************/
void rainbow() {
  // replacing for(long cycle = 0; cycle < 5*65536; cycle += 256)
  cycle += 256*animSpeed;
  if(cycle >= 5*65536)
    cycle = 0;
//  Serial.println( cycle + (1 * 65536L / Get_MAX_Index()) );
  for(int i=0; i<Get_MAX_Index(); i++) { 
    int pixelHue = cycle + (i * 65536L / Get_MAX_Index());
    Set_Pixel_By_Index(i, FRONT_STRIP, stripF.gamma32(stripF.ColorHSV(pixelHue)));
    Set_Pixel_By_Index(i, BACK_STRIP, stripF.gamma32(stripF.ColorHSV(pixelHue)));
  }
  Display_Update();
}

void theaterChaseRainbow() {
  int firstPixelHue = 0;
  cycle = (cycle+1)%(30*((int)animSpeed));

  int a = (cycle/((int)animSpeed))%10;    
  int b = (cycle/((int)animSpeed))%3;    
  Clear();         //   Set all pixels in RAM to 0 (off)
  // 'c' counts up from 'b' to end of strip in steps of 3...
  for(int c=b; c<Get_MAX_Index(); c += 3) {
    int hue   = firstPixelHue + c * 65536L / Get_MAX_Index();
    uint32_t color = stripF.gamma32(stripF.ColorHSV(hue)); // hue -> RGB
    Set_Pixel_By_Index(c, FRONT_STRIP, color);
    Set_Pixel_By_Index(c, BACK_STRIP, color);
  }
  Display_Update(); // Update strip with new contents
}

void theaterChase() {
  cycle = (cycle+1)%(30*((int)animSpeed));

  int a = (cycle/((int)animSpeed))%10;    
  int b = (cycle/((int)animSpeed))%3;    
  Clear();         //   Set all pixels in RAM to 0 (off)
  // 'c' counts up from 'b' to end of strip in steps of 3...
  for(int c=b; c<Get_MAX_Index(); c += 3) {
    Set_Pixel_By_Index(c, FRONT_STRIP, colors[0]);
    Set_Pixel_By_Index(c, BACK_STRIP, colors[0]);
  }
  Display_Update(); // Update strip with new contents
}

void colorWipe() {
  cycle = (cycle+1)%(Get_MAX_Index()*((int)animSpeed));
  int pi = (cycle)/((int)animSpeed);

  Set_Pixel_By_Index(pi, FRONT_STRIP, colors[0]);
  Set_Pixel_By_Index(pi, BACK_STRIP, colors[0]);
  Display_Update();                          //  Update strip to match
}

/************************
 *  Custom Animations
 ************************/

//Description: Used to sync front and back LED Strips. 
//   Cycles a red light in a circle across a background of blue then green lines.
void Red_Light_Tracing(){
  cycle = (cycle+1)%(Get_MAX_Index()*((int)animSpeed));
  for(int i=0;i<Get_MAX_Index();i++){
    Set_Pixel_By_Index(i, FRONT_STRIP, MColor(0, (i%2)==1?255:0, (i%2)==1?0:255));
    Set_Pixel_By_Index(i, BACK_STRIP, MColor(0, (i%2)==1?255:0, (i%2)==1?0:255));
  }
  Set_Pixel_By_Index(cycle/animSpeed, FRONT_STRIP, MColor(255, 0, 0));
  Set_Pixel_By_Index(cycle/animSpeed, BACK_STRIP, MColor(255, 0, 0));
  Display_Update();
}

void Clock_Animation(){   
  double curTime = (millis()/1000.0) - time_diff; 
  double hours = fmod(curTime,86400.0) / 3600.0;
  double minutes = fmod(curTime,3600.0) / 60.0;
  double seconds = fmod(curTime,60.0);
  int red, redB, fade;
  int blue, blueB;
  int green, greenB;

  cycle = (cycle+1)%(120*((int)animSpeed));
  fade = (cycle/((int)animSpeed))%120;
  fade = fade>60 ? 120 - fade: fade;
  double fractionS = seconds/60.0; 
  double fractionM = minutes/60.0;
  double fractionH = fmod(hours,12.0)/12.0;
  for(int i=0;i<Get_MAX_Index();i++){
    double fraction = fmod(((double)i+1)/((double)Get_MAX_Index())+0.5,1.0);
    if( (i+1)%(Get_MAX_Index()/12)==0){
      red = blue = green = 150; 
    }else{
      red = blue = green = fade;
    }
    
    double distS = fmin(fabs(fractionS - fraction),fabs(fraction-fractionS));
    double distM = fmin(fabs(fractionM - fraction),fabs(fraction-fractionM));
    double distH = fmin(fabs(fractionH - fraction),fabs(fraction-fractionH));
    
    if(distS < cursorWidth){
      red += 255.0*(cursorWidth - distS)/cursorWidth;
    }
    if(distM < 2*cursorWidth){
      blue += 255.0*(2*cursorWidth - distM)/cursorWidth;
    }
    if(distH < 3*cursorWidth){
      green += 255.0*(3*cursorWidth - distH)/cursorWidth;
    }
    
    Set_Pixel_By_Index(i, FRONT_STRIP, MColor(red, blue, green));
    Set_Pixel_By_Index(i, BACK_STRIP, MColor(red, blue, green));
  }
  Display_Update();
  
//  Serial.print("fractionS = ");
//  Serial.print(fractionS);
//  Serial.print("  fractionM = ");
//  Serial.print(fractionM);
//  Serial.print("  fractionH = ");
//  Serial.println(fractionH);
}
