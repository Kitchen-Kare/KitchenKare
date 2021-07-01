//Import required libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Change this parameter for Testing
#define screenDuration 5

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

void setup() {
  //Start Serial Monitor
  Serial.begin(9600);

  //If Screen fails
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // The library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  for (int i=0; i<screenDuration*6; i++) {  
    //Initialize and Clear
    display.clearDisplay();
    display.setTextSize(1); // Draw 1X-scale text
    display.setTextColor(SSD1306_WHITE);

    //Print screenDuration
    display.setCursor(0, 0);
    display.println(F("screenDuration: "));
    display.setCursor(90, 0);
    display.println(screenDuration);
    display.setCursor(110, 0);
    display.println(F("sec"));

    //Print Count (to show loops done so far)
    display.setTextSize(2); // Draw 1.5X-scale text
    display.setCursor(5, 18);
    display.println(F("Count: "));
    display.setCursor(80, 18);
    display.println(i/screenDuration);

    //Print Time (to show progression in loop)
    display.setCursor(5, 35);
    display.println(F("Time: "));
    display.setCursor(80, 35);
    display.println(i%screenDuration);
    
    display.display();      // Show initial text
    delay(1000);
  }  

  //Show success screen
  for (int i=0; i<60; i++) {
    success();
  }

  //Redo Testing
  display.clearDisplay();
  display.setCursor(5, 18);
  display.println(F("Restarting"));
  display.display();  
  delay(1000);
}

//Print Success
void success() {
  display.clearDisplay();
  display.setCursor(30, 18);
  display.println(F("OLED"));
  display.setCursor(20, 32);
  display.println(F("Success"));
  display.display();  
  delay(1000);
  display.clearDisplay();
  display.display();  
  delay(1000);
}
