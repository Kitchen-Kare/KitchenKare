//Import required libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFun_GridEYE_Arduino_Library.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Initialize Variables
GridEYE grideye;
Adafruit_BME280 bme;

boolean pixelTable[64];
boolean humanDetection = false;

void setup() {
  //Start Serial Monitor
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen -- the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);
  display.clearDisplay();

  //Start Sparkfun Grid-Eye Human Sensor
  Wire.begin();
  grideye.begin();

  //If BME is not activated contingency
  if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

//Main loop
void loop() {
  short temp = getTemp();
  detectHuman();
  screen(temp);

  delay(5000);
}

//Display Temp on screen
void screen(short temp) {
  //Setup Text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Temp: ");
  
  //Get temp and show
  display.print(temp);
  display.print((char)247);
  display.print("F");
  display.display();
}

//Get Temp from BME
short getTemp(void) {
  Serial.println(bme.readTemperature() * 9.0/5 + 32);
  return (short) (bme.readTemperature() * 9.0/5 + 32);
}

void detectHuman(void) {
  //Initialize 8x8 array
  for (byte i = 0; i < 64; i++)
    pixelTable[i] = map(grideye.getPixelTemperature(i), 15, 25, false, true);

  //Calculate number of different pixels
  byte numDifferent = 0;
  for (byte i = 0; i < 64; i++) {
    if (pixelTable[i])
      numDifferent++;
  }

  //Benchmark for significant change
  if (numDifferent > 4)
    humanDetection = true;
  else
    humanDetection = false;
}
