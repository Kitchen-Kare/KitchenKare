
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

GridEYE grideye;
Adafruit_BME280 bme;

boolean pixelTable[64];
boolean humanDetection = false;

void setup() {

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

  Wire.begin();
  grideye.begin();

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

void loop() {
  short temp = getTemp();
  detectHuman();
  screen(temp);

  delay(5000);
}

void screen(short temp) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Temp: ");
  display.print(temp);
  display.print((char)247);
  display.print("F");
  display.display();
}

short getTemp(void) {
  Serial.println(bme.readTemperature() * 9.0/5 + 32);
  return (short) (bme.readTemperature() * 9.0/5 + 32);
}

void detectHuman(void) {
  for (byte i = 0; i < 64; i++)
    pixelTable[i] = map(grideye.getPixelTemperature(i), 15, 25, false, true);

  byte numDifferent = 0;
  for (byte i = 0; i < 64; i++) {
    if (pixelTable[i])
      numDifferent++;
  }

  if (numDifferent > 4)
    humanDetection = true;
  else
    humanDetection = false;
}
