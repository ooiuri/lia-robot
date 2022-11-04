#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds

// Clear the buffer.
  display.clearDisplay();

// Draw W01 WORD_HELLO bitmap on the screen
  display.drawBitmap(0, 0, image_blink_1, 128, 64, 1);
  display.display();

}

void loop() {
  
  delay(3000); // Pause for 2 seconds
 
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_1, 128, 64, 1);
  display.display();

  delay(200); // Pause for 2 seconds 
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_2, 128, 64, 1);
  display.display();

  delay(200); // Pause for 2 seconds 
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_3, 128, 64, 1);
  display.display();

  delay(200); // Pause for 2 seconds 
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_4, 128, 64, 1);
  display.display();

  delay(200); // Pause for 2 seconds 
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_5, 128, 64, 1);
  display.display();

  delay(200); // Pause for 2 seconds
  
  display.clearDisplay();
  
  // Draw 21 EYES_SLEEP bitmap on the screen
  display.drawBitmap(0, 0, image_blink_1, 128, 64, 1);
  display.display();
}
