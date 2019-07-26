// V1.1 --//
#include "Adafruit_NeoPixel.h"

#define PIN 6  //pinNumber
#define NUM_LEDS 55
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int ifRead;
int num_Value;
int ana_Total;
int avg_ana;
int detect;

int random_R;
int random_G;
int random_B;

// Theme global variable
// Theme 
int i;

// LED 灯板分区 -----------------------------------//
int group1[] = {27, 18, 23, 32, 36, 31, 22};
int group2[] = {10, 14, 17, 26, 35, 41, 44, 40, 37, 28, 19, 13};
int group3[] = {3, 7, 9, 15, 24, 33, 42, 43, 48, 51, 47, 45, 39, 30, 21, 12, 11, 6};
int group4[] = {1, 2, 8, 16, 25, 34, 49, 50, 54, 53, 52, 46, 38, 29, 20, 5, 4, 0};

int triangle1[] = {0, 1, 3, 4, 6, 7, 10, 13, 18, 27};
int triangle2[] = {2, 8, 9, 15, 14, 17, 16, 23, 24};
int triangle3[] = {25, 26, 32, 33, 34, 35, 42, 43, 49};
int triangle4[] = {36, 41, 44, 48, 47, 51, 50, 54, 53};
int triangle5[] = {30, 31, 37, 38, 39, 40, 45, 46, 52};
int triangle6[] = {5, 11, 12, 20, 19, 21, 22, 28, 29};

int tri_diagonal1[] = {4, 6, 10, 14, 17, 24, 25, 33, 35, 41, 44, 47, 52, 45, 37, 28, 19, 11};
int tri_diagonal2[] = {2, 9, 17, 26, 35, 43, 50, 48, 44, 40, 37, 30, 29, 21, 19, 13, 10, 7};
int diagonals[] = {4, 6, 10, 14, 17, 24, 25, 33, 35, 41, 47, 52, 45, 28, 11, 2, 9, 26, 43, 50, 48, 44, 40, 37, 30, 29, 21, 19, 13, 7};

//int edge[] = {0, 1, 2, 4, 5, 8, 16, 20, 25, 29, 34, 38, 46, 49, 53, 54};

// From inside out //
int groupAll[] = {27, 18, 23, 32, 36, 31, 22, 10, 14, 17, 26, 35, 41, 44, 40, 37, 28, 19, 13, 3, 7, 9, 15, 24, 33, 42, 43, 48, 51, 47, 45, 39, 30, 21, 12, 11, 6, 1, 2, 8, 16, 25, 34, 49, 50, 54, 53, 52, 46, 38, 29, 20, 5, 4, 0};
// From inside out //


// LED 灯板分区 -----------------------------------//


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() { 
  
   //Theme 01
  //sectionColorFade(18, 109, 255, 0, 0, 0, 5, 7);
  // Theme 02
  //rainbowCycle_01(20, 3);
  // Theme 03
  //rainbowCycle_02(25, 3);
  // Theme 04 (Set All)
  //setAll(0, 0, 50);
  // Theme 05 (Fading Set All)
  //setAllFade(200, 0, 100, 20, 0);
  // Theme 06 LED waiting indicator
  //wait_LED(50, 50, 50, 15);
  // Theme 07
  //colorHoverCycle();
  // Theme 08
  //randomSparkle(50, 0, 0, 0, 0, 250);
  triImpulse(18, 209, 255, 255, 0, 0, 30);
  //randomPixel(255, 0, 0, 0, 0, 255, 150, 50, 1);
  
}

// --------------------------------------------------------------------------//

// LED 基本控制 -----------------------------------//
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void setAllFade (byte R, byte G, byte B, int speeds, int delays) {
  colorFade(R, G, B, groupAll, 55, speeds);
  delay(delays);
}

void colorFade(uint8_t r, uint8_t g, uint8_t b, int arr[], int size_arr, uint8_t wait) {
      
      uint8_t curr_r, curr_g, curr_b;
      uint32_t curr_col = strip.getPixelColor(arr[0]); // get the current colour
      curr_b = curr_col & 0xFF; curr_g = (curr_col >> 8) & 0xFF; curr_r = (curr_col >> 16) & 0xFF;  // separate into RGB components

      while ((curr_r != r) || (curr_g != g) || (curr_b != b)){  // while the curr color is not yet the target color
        if (curr_r < r) curr_r++; else if (curr_r > r) curr_r--;  // increment or decrement the old color values
        if (curr_g < g) curr_g++; else if (curr_g > g) curr_g--;
        if (curr_b < b) curr_b++; else if (curr_b > b) curr_b--;
        for (int i = 0; i < size_arr; i ++) {
         strip.setPixelColor(arr[i], curr_r, curr_g, curr_b);  // set the color
        }
        strip.show();
        IRDetect();
        //Serial.println(detect);
        if (detect == 1) delay(0); else if (detect == 0) delay(wait);  
        // 后面这里加声音break the loop，现在用IR信号代替
        //if (detect == 1) {
        //  setAll(0, 0, 0);
        //  break;
        //  } else if (detect == 0) delay(wait);  
      }
}

void pixelFade(uint8_t r, uint8_t g, uint8_t b, int pixel, uint8_t wait) {
      
      uint8_t curr_r, curr_g, curr_b;
      uint32_t curr_col = strip.getPixelColor(pixel); // get the current colour
      curr_b = curr_col & 0xFF; curr_g = (curr_col >> 8) & 0xFF; curr_r = (curr_col >> 16) & 0xFF;  // separate into RGB components

      while ((curr_r != r) || (curr_g != g) || (curr_b != b)){  // while the curr color is not yet the target color
        if (curr_r < r) curr_r++; else if (curr_r > r) curr_r--;  // increment or decrement the old color values
        if (curr_g < g) curr_g++; else if (curr_g > g) curr_g--;
        if (curr_b < b) curr_b++; else if (curr_b > b) curr_b--;
         strip.setPixelColor(pixel, curr_r, curr_g, curr_b);  // set the color
        strip.show();
        IRDetect();
        //Serial.println(detect);
        if (detect == 1) delay(0); else if (detect == 0) delay(wait);  
        // 后面这里加声音break the loop，现在用IR信号代替
        //if (detect == 1) {
        //  setAll(0, 0, 0);
        //  break;
        //  } else if (detect == 0) delay(wait);  
      }
}

void random_color2() {
  int randomN = random(1, 7);
  if (randomN == 1) {
    random_R = 255;
    random_G = random(1, 256);
    random_B = 0;
  }  
  if (randomN == 2) {
    random_R = random(1, 256);
    random_G = 255;
    random_B = 0;
  }
  if (randomN == 3) {
    random_R = 0;
    random_G = 255;
    random_B = random(1, 256);
  }
  if (randomN == 4) {
    random_R = 0;
    random_G = random(1, 256);
    random_B = 255;
  }
  if (randomN == 5) {
    random_R = random(1, 256);
    random_G = 0;
    random_B = 255;
  }
  if (randomN == 6) {
    random_R = 255;
    random_G = 0;
    random_B = random(1, 256);
  }
}

// LED 基本控制 -----------------------------------//


// IR 检测 -----------------------------------//
void IRDetect() {
  ifRead = analogRead(A0);
  //Serial.println(ifRead);
   if ( num_Value < 15 )
  {
    ana_Total = ana_Total + ifRead;
    num_Value = num_Value + 1;
    } else {
      avg_ana = ana_Total / 15;
      if ( ifRead - avg_ana > 80 ) {
        detect = 1;
        } else {
          detect = 0;
          }
      }
  //Serial.println(detect);    
}
// IR 检测 -----------------------------------//


// LED 其它灯效 -----------------------------------//
// Theme 01 ------//
void sectionColorFade(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t wait, int mode) {

  // Ring From Outside Fading
  if (mode == 1) {
   colorFade(r1, g1, b1, group4, (sizeof(group4) / sizeof(group4[0])), wait);
   colorFade(r1, g1, b1, group3, (sizeof(group3) / sizeof(group3[0])), wait);
   colorFade(r1, g1, b1, group2, (sizeof(group2) / sizeof(group2[0])), wait);
   colorFade(r1, g1, b1, group1, (sizeof(group1) / sizeof(group1[0])), wait);

   colorFade(r2, g2, b2, group4, (sizeof(group4) / sizeof(group4[0])), wait);
   colorFade(r2, g2, b2, group3, (sizeof(group3) / sizeof(group3[0])), wait);
   colorFade(r2, g2, b2, group2, (sizeof(group2) / sizeof(group2[0])), wait);
   colorFade(r2, g2, b2, group1, (sizeof(group1) / sizeof(group1[0])), wait);   
  }

  // Ring From Inside Fading
  if (mode == 2) {
   colorFade(r1, g1, b1, group1, (sizeof(group1) / sizeof(group1[0])), wait);
   colorFade(r1, g1, b1, group2, (sizeof(group2) / sizeof(group2[0])), wait);
   colorFade(r1, g1, b1, group3, (sizeof(group3) / sizeof(group3[0])), wait);
   colorFade(r1, g1, b1, group4, (sizeof(group4) / sizeof(group4[0])), wait);

   colorFade(r2, g2, b2, group1, (sizeof(group1) / sizeof(group1[0])), wait); 
   colorFade(r2, g2, b2, group2, (sizeof(group2) / sizeof(group2[0])), wait);
   colorFade(r2, g2, b2, group3, (sizeof(group3) / sizeof(group3[0])), wait);
   colorFade(r2, g2, b2, group4, (sizeof(group4) / sizeof(group4[0])), wait);  
  }

  // Triangle Fading
  if (mode == 3) {
   colorFade(r1, g1, b1, triangle1, (sizeof(triangle1) / sizeof(triangle1[0])), wait);
   colorFade(r1, g1, b1, triangle2, (sizeof(triangle2) / sizeof(triangle2[0])), wait);
   colorFade(r1, g1, b1, triangle3, (sizeof(triangle3) / sizeof(triangle3[0])), wait);
   colorFade(r1, g1, b1, triangle4, (sizeof(triangle4) / sizeof(triangle4[0])), wait);
   colorFade(r1, g1, b1, triangle5, (sizeof(triangle5) / sizeof(triangle5[0])), wait);
   colorFade(r1, g1, b1, triangle6, (sizeof(triangle6) / sizeof(triangle6[0])), wait);

   colorFade(r2, g2, b2, triangle1, (sizeof(triangle1) / sizeof(triangle1[0])), wait);
   colorFade(r2, g2, b2, triangle2, (sizeof(triangle2) / sizeof(triangle2[0])), wait);
   colorFade(r2, g2, b2, triangle3, (sizeof(triangle3) / sizeof(triangle3[0])), wait);
   colorFade(r2, g2, b2, triangle4, (sizeof(triangle4) / sizeof(triangle4[0])), wait);
   colorFade(r2, g2, b2, triangle5, (sizeof(triangle5) / sizeof(triangle5[0])), wait);
   colorFade(r2, g2, b2, triangle6, (sizeof(triangle6) / sizeof(triangle6[0])), wait);
  }

  if (mode == 4) {
   colorFade(r1, g1, b1, group1, (sizeof(group1) / sizeof(group1[0])), wait);
   colorFade(r1, g1, b1, group2, (sizeof(group2) / sizeof(group2[0])), 0);
   colorFade(r1, g1, b1, group3, (sizeof(group3) / sizeof(group3[0])), 0);
   colorFade(r1, g1, b1, group4, (sizeof(group4) / sizeof(group4[0])), 0);

   colorFade(r2, g2, b2, group1, (sizeof(group1) / sizeof(group1[0])), 0); 
  }

  if (mode == 5) {
   colorFade(r1, g1, b1, group1, (sizeof(group1) / sizeof(group1[0])), wait);
   colorFade(r1, g1, b1, group2, (sizeof(group2) / sizeof(group2[0])), 0);
   colorFade(r1, g1, b1, group3, (sizeof(group3) / sizeof(group3[0])), 0);
   colorFade(r1, g1, b1, group4, (sizeof(group4) / sizeof(group4[0])), 0);
   delay(500);
   for (int i = 0; i < (sizeof(group1) / sizeof(group1[0])); i ++) {
    strip.setPixelColor(group1[i], r2, g2, b2);  // set the color
    strip.show();
   }
   delay(100);
  }

  if (mode == 6) {
   for (int i = 0; i < (sizeof(groupAll) / sizeof(groupAll[0])); i ++) {
    strip.setPixelColor(groupAll[i], r2, g2, b2);  // set the color
    strip.show();
   }
   delay(100);
   colorFade(r1, g1, b1, groupAll, (sizeof(groupAll) / sizeof(groupAll[0])), wait); 
  }

  //mode 7 random color & random group
  if (mode == 7) {
    int random_num = random(1, 5);  
    random_color2();
    if (random_num == 1) {
      for (int i = 0; i < (sizeof(group1) / sizeof(group1[0])); i ++) {
        strip.setPixelColor(group1[i], random_R, random_G, random_B);  // set the color
        strip.show();
      }
      colorFade(r1, g1, b1, group1, (sizeof(group1) / sizeof(group1[0])), wait);
    }
    if (random_num == 2) {
      for (int i = 0; i < (sizeof(group2) / sizeof(group2[0])); i ++) {
        strip.setPixelColor(group2[i], random_R, random_G, random_B);  // set the color
        strip.show();
      }
      colorFade(r1, g1, b1, group2, (sizeof(group2) / sizeof(group2[0])), wait);
    }
    if (random_num == 3) {
      for (int i = 0; i < (sizeof(group3) / sizeof(group3[0])); i ++) {
        strip.setPixelColor(group3[i], random_R, random_G, random_B);  // set the color
        strip.show();
      }
      colorFade(r1, g1, b1, group3, (sizeof(group3) / sizeof(group3[0])), wait);
    }
    if (random_num == 4) {
      for (int i = 0; i < (sizeof(group4) / sizeof(group4[0])); i ++) {
        strip.setPixelColor(group4[i], random_R, random_G, random_B);  // set the color
        strip.show();
      }
      colorFade(r1, g1, b1, group4, (sizeof(group4) / sizeof(group4[0])), wait);
    }
  }

  if (mode == 8) {
    int random_num = random(1, 7);
    if (random_num == 1) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle1, (sizeof(triangle1) / sizeof(triangle1[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle1, (sizeof(triangle1) / sizeof(triangle1[0])), wait);
    }
    if (random_num == 2) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle2, (sizeof(triangle2) / sizeof(triangle2[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle2, (sizeof(triangle2) / sizeof(triangle2[0])), wait);
    }
    if (random_num == 3) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle3, (sizeof(triangle3) / sizeof(triangle3[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle3, (sizeof(triangle3) / sizeof(triangle3[0])), wait);
    }
    if (random_num == 4) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle4, (sizeof(triangle4) / sizeof(triangle4[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle4, (sizeof(triangle4) / sizeof(triangle4[0])), wait);
    }
    if (random_num == 5) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle5, (sizeof(triangle5) / sizeof(triangle5[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle5, (sizeof(triangle5) / sizeof(triangle5[0])), wait);
    }
    if (random_num == 6) {
      random_color2();
      colorFade(random_R, random_G, random_B, triangle6, (sizeof(triangle6) / sizeof(triangle6[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, triangle6, (sizeof(triangle6) / sizeof(triangle6[0])), wait);
    }
  }

  if (mode == 9) {
    int random_num = random(1, 5);
    if (random_num == 1) {
      random_color2();
      colorFade(random_R, random_G, random_B, group1, (sizeof(group1) / sizeof(group1[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, group1, (sizeof(group1) / sizeof(group1[0])), wait);
    }
    if (random_num == 2) {
      random_color2();
      colorFade(random_R, random_G, random_B, group2, (sizeof(group2) / sizeof(group2[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, group2, (sizeof(group2) / sizeof(group2[0])), wait);
    }
    if (random_num == 3) {
      random_color2();
      colorFade(random_R, random_G, random_B, group3, (sizeof(group3) / sizeof(group3[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, group3, (sizeof(group3) / sizeof(group3[0])), wait);
    }
    if (random_num == 4) {
      random_color2();
      colorFade(random_R, random_G, random_B, group4, (sizeof(group4) / sizeof(group4[0])), wait);
      random_color2();
      colorFade(random_R, random_G, random_B, group4, (sizeof(group4) / sizeof(group4[0])), wait);
    }
  }

  if (mode == 10) {
    for(i=0; i <= 18; i++)
    {
      colorFade(r1, g1, b1, tri_diagonal1[i], (sizeof(tri_diagonal1) / sizeof(tri_diagonal1[0])), 50);
      delay(50);
      colorFade(r2, g2, b2, tri_diagonal1[i], (sizeof(tri_diagonal1) / sizeof(tri_diagonal1[0])), 50);
    }
  }
  
  
}
// Theme 01 Ends ------//

// Theme 02 & 03 (Rainbow) ------//
void rainbowCycle_01(int SpeedDelay, int color_range) {
  
  byte *c;
  uint16_t i, j;
  
  for(j=0; j<256*1; j ++) { // 1 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / (NUM_LEDS * color_range)) + j) & 255);
      setPixel(groupAll[i], *c, *(c+1), *(c+2));
    }
    showStrip();
    IRDetect();
    if (detect == 1) delay(2); else if (detect == 0) delay(SpeedDelay);  
  }
}

void rainbowCycle_02(int SpeedDelay, int color_range) {
  int ring_1[] = {27};
  int ring_2[] = {18, 23, 32, 36, 31, 22};
  int ring_3[] = {10, 14, 17, 26, 35, 41, 44, 40, 37, 28, 19, 13};
  int ring_4[] = {3, 7, 9, 15, 24, 33, 42, 43, 48, 51, 47, 45, 39, 30, 21, 12, 11, 6};
  int ring_5[] = {1, 2, 8, 16, 25, 34, 49, 50, 54, 53, 52, 46, 38, 29, 20, 5, 4, 0};
  
  byte *c;
  uint16_t i, j;

  for(j=0; j<256; j++) { 
    // 5 cycles of all colors on wheel
    for(i=0; i < 1; i++) {
      c=Wheel(((i * 256 / (1 * color_range)) + j) & 255);
      setPixel(ring_1[i], *c, *(c+1), *(c+2));
    }
    
    for(i=0; i < 6; i++) {
      c=Wheel(((i * 256 / (6 * color_range)) + j) & 255);
      setPixel(ring_2[i], *c, *(c+1), *(c+2));
    }

    for(i=0; i< 12; i++) {
      c=Wheel(((i * 256 / (12 * color_range)) + j) & 255);
      setPixel(ring_3[i], *c, *(c+1), *(c+2));
    }
    
    for(i=0; i< 18; i++) {
      c=Wheel(((i * 256 / (18 * color_range)) + j) & 255);
      setPixel(ring_4[i], *c, *(c+1), *(c+2));
    }

    for(i=0; i< 18; i++) {
      c=Wheel(((i * 256 / (18 * color_range)) + j) & 255);
      setPixel(ring_5[i], *c, *(c+1), *(c+2));
    }
    showStrip();
    IRDetect();
    if (detect == 1) delay(8); else if (detect == 0) delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}
// Theme 02 & 03 Ends ------//

void colorHoverCycle () {
  IRDetect(); 
}

void randomSparkle(byte r, byte g, byte b, byte RR, byte GG, byte BB) {
  setAllFade(r, g, b, 10, 50);  
  
  int randomLED = random(0,55);
  setPixel(randomLED, RR, GG, BB);
  showStrip();;

  uint8_t curr_r, curr_g, curr_b;
  uint32_t curr_col = strip.getPixelColor(randomLED); // get the current colour
  curr_b = curr_col & 0xFF; curr_g = (curr_col >> 8) & 0xFF; curr_r = (curr_col >> 16) & 0xFF; 
  while ((curr_r != r) || (curr_g != g) || (curr_b != b)){  // while the curr color is not yet the target color
        if (curr_r < r) curr_r++; else if (curr_r > r) curr_r--;  // increment or decrement the old color values
        if (curr_g < g) curr_g++; else if (curr_g > g) curr_g--;
        if (curr_b < b) curr_b++; else if (curr_b > b) curr_b--;
        strip.setPixelColor(randomLED, curr_r, curr_g, curr_b);  
        strip.show();
        IRDetect();
        if (detect == 1) delay(0); else if (detect == 0) delay(20);       
   }
  
}

void triImpulse(int r1, int g1, int b1, int r2, int g2, int b2, int wait) {

  for(i=0; i< 18; i++){
    setPixel(tri_diagonal1[i], r1/4, g1/4, b1/4);
    showStrip();
    delay(wait/1.2);
  }
  for(i=0; i< 18; i++){
    setPixel(tri_diagonal1[i], r1/2, g1/2, b1/2);
    showStrip();
    delay(wait/2);
  }
  for(i=0; i< 18; i++){
    setPixel(tri_diagonal1[i], r1, g1, b1);
    showStrip();
    delay(wait/3);
  }
  
  delay(300);
      colorFade(0,0,0, tri_diagonal1, (sizeof(tri_diagonal1) / sizeof(tri_diagonal1[0])), 10);
      
  for(i=0; i< 18; i++){
    setPixel(tri_diagonal2[i], r2/4, g2/4, b2/4);
    showStrip();
    delay(wait/1.2);
  }   
  for(i=0; i< 18; i++){
    setPixel(tri_diagonal2[i], r2/2, g2/2, b2/2);
    showStrip();
    delay(wait/2);
  }
  for(i=0; i< 18; i++){
    setPixel(tri_diagonal2[i], r2, g2, b2);
    showStrip();
    delay(wait/3);
  }   
 delay(300);
      colorFade(0,0,0, tri_diagonal2, (sizeof(tri_diagonal2) / sizeof(tri_diagonal2[0])), 10);
  
}


void randomPixel(int r1, int g1, int b1, int r2, int g2, int b2, int allwait, int pixelwait, bool Fade){

  int allWait = allwait;
  int pixelWait = pixelwait;
  setAllFade(r1, g1, b1, 15, allWait);
  
  int all[55];
  
  for (i = 0; i<55; i++){
      all[i] = i;
  }
  
  for (int h = 0; h<55; h++){
    
     int j = random(0, 55-h);
     
     if (Fade == 1){
      
      pixelFade(r2, g2, b2, all[j], 5);
      showStrip();
     }
     else {
      setPixel(all[j], r2, g2, b2);
      showStrip();
      delay(pixelWait);
     }
      for(int k = j; k+1 < sizeof(all) / sizeof(all[0]) ; k++){
        all[k] = all[k+1];
     }
     
  }

  delay(500);

  setAllFade((r1+r2)/2, (g1+g2)/2, (b1+b2)/2, 20, 500);

  
  
}


// LED 其它灯效 -----------------------------------//
void wait_LED (byte R, byte G, byte B, int wait) {
  colorFade(R, G, B, group3, (sizeof(group3) / sizeof(group3[0])), wait);
  colorFade(0, 0, 0, group3, (sizeof(group3) / sizeof(group3[0])), wait);
}
