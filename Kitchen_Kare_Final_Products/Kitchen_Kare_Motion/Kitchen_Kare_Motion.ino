// Import required libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFun_GridEYE_Arduino_Library.h>

Adafruit_SSD1306 display(128, 64, &Wire, 4);
GridEYE grideye;
Adafruit_BME280 bme;

// Arduino pin numbers
byte LED = 13;
byte SPEAKER = 8;
byte BUTTON = 7;
byte RELAY = 5;

// Testing variables
byte UNSAFETIME = 2; // the amount of seconds that is unsafe for there to be no human detection
short UNSAFETEMP = 80; // the temperature that is unsafe 

boolean pixelTable[64]; // an array populated with data from the infrared sensor
boolean humanDetection = false; // a variable that tells if there is human detection through the infrared sensor or button

void setup() {
  
  // Initialize parts
  pinMode(LED, OUTPUT);
  pinMode(BUTTON,INPUT);
  pinMode(RELAY, OUTPUT);
  Wire.begin();
  grideye.begin();
  Serial.begin(9600);

  // Doesn't continue if the BME cannot sense temperature properly
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor.");
    for(;;);
  }

  // Doesn't continue if the OLED screen doesn't work
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED allocation failed"));
    for(;;);
  }

  // Initializes the OLED screen
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  
  // Turn on appliance and get temperature
  detectHuman();
  short temp = getTemp();
  screen(temp);
  ovenON();

  // Checks for an unsafe temperature or if there is no human detection
  if (temp >= UNSAFETEMP || !humanDetection) { 
    int timeT = 0;
    
    // Alerts the user until time runs out or the user responds
    for (int i = 0; i < UNSAFETIME; i++) {

      // Turns on Light and Speaker
      lightON();
      speakerON();

      // Updates Human variable and displays Temp
      detectHuman();
      temp = getTemp();
      screen(temp);

      // Helps exit for loop
      if (humanDetection || temp < UNSAFETEMP)
        i = UNSAFETIME;

      // Buffer
      timeT++;
      delay(1000);
    }

    // Stops the alerts
    lightOFF();
    speakerOFF(); 

    // Shuts off the oven if the user did not respond
    if (timeT >= UNSAFETIME) { 
      ovenOFF();
      Serial.println("Plug turned off");
      delay(5000); // Oven off for 5 seconds
    }
  }

  // Continues to update the OLED screen if the temperature is safe or there is human detection
  while (temp < UNSAFETEMP || humanDetection) { 
    
    //Update Human variable and display temperature
    detectHuman();
    temp = getTemp();
    Serial.println("Temp: ");
    Serial.print(temp);
    Serial.print((char)247);
    Serial.print("F");-
    Serial.println();
    screen(temp);

    //Turn on Oven
    ovenON();
    Serial.println("Plug turned off");
    delay(1000);
    // NOTE: this can be modified to loop for a certain amount of time as well (cooking time? customizable)
  }
}

/**
 * Turns on the light (flashing)
 */
void lightON() {
  digitalWrite(LED, HIGH);
  delay(750);
  digitalWrite(LED, LOW);
}

/**
 * Turns off the light
 */
void lightOFF() {
  digitalWrite(LED, LOW);
}

/**
 * Turns the speaker on (on and off beeps)
 */
void speakerON() {
  noTone(SPEAKER);
  tone(SPEAKER, 523, 300);
}

/**
 * Turns off the speaker
 */
void speakerOFF() {
  noTone(SPEAKER);
}

/**
 * Turns the oven on
 */
void ovenON() {
  digitalWrite(RELAY, HIGH);
}

/**
 * Turns off the oven
 */
void ovenOFF() {
  digitalWrite(RELAY, LOW);
}

/**
 * Displays the temperature on the OLED screen
 */
void screen(short temp) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temp);
  display.print((char)247);
  display.print("F");
  display.display();
  //OLED not working case
  Serial.print("Temp: ");
  Serial.print((char)247);
  Serial.print("F");
  Serial.println("\n");
}

/**
 * Gets the temperature from the BME
 * @return temperature in Farenheight
 */
short getTemp() {
  return (short) (bme.readTemperature() * 9.0/5 + 32);
}

/**
 * Checks if there is human activity in the kitchen through the infrared sensor or if the button is being pressed
 */
void detectHuman() {
  for (byte i = 0; i < 64; i++)
    pixelTable[i] = map(grideye.getPixelTemperature(i), 15, 25, false, true);

  byte numDifferent = 0;
  for (byte i = 0; i < 64; i++) {
    if (pixelTable[i])
      numDifferent++;
  }

  Serial.println(numDifferent);

  if (numDifferent > 30 || digitalRead(BUTTON) == HIGH)
    humanDetection = true;
  
  else
    humanDetection = false;
}
