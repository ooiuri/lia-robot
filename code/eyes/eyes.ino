#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void blink(int delay_time) {
  for (int i = 0; i < image_blink_length; i++){
    display.clearDisplay();
    display.drawBitmap(0, 0, image_blink[i], 128, 64, 1);
    display.display();
    delay(delay_time); 
  };
};

void happy(int delay_time) {
  for (int i = 0; i < image_happy_length; i++){
    display.clearDisplay();
    display.drawBitmap(0, 0, image_happy[i], 128, 64, 1);
    display.display();
    delay(delay_time); 
  };
};

void running(int delay_time) {
  for (int i = 0; i < image_running_length; i++){
    display.clearDisplay();
    display.drawBitmap(0, 0, image_running[i], 128, 64, 1);
    display.display();
    delay(delay_time); 
  };
};

void middleRun(int delay_time){
  for (int i = 0; i < 2; i++){
    display.clearDisplay();
    display.drawBitmap(0, 0, image_running[i], 128, 64, 1);
    display.display();
    delay(delay_time); 
  };
  
  for(int k = 0; k < 100; k++){
    for (int i = 2; i < 9; i++){
      display.clearDisplay();
      display.drawBitmap(0, 0, image_running[i], 128, 64, 1);
      display.display();
      delay(delay_time); 
    };
  };

  for (int i = 2; i < image_running_length; i++){
    display.clearDisplay();
    display.drawBitmap(0, 0, image_running[i], 128, 64, 1);
    display.display();
    delay(delay_time); 
  };
}

void stare(int delay_time) {
  display.drawBitmap(0, 0, image_blink[0], 128, 64, 1);
  display.display();
  delay(delay_time);
};


void setup() {
  Serial.begin(115200);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds

// Clear the buffer.
  display.clearDisplay();

  stare(100);
  display.display();

}

void loop() {
  
  
  blink(50);
  
  stare(500);
  
  happy(100);

  stare(500);

  running(100);
  running(50);
  running(20);

  middleRun(20);

  stare(1000);
 

}
