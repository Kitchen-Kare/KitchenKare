
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_SSD1306 display(128, 64, &Wire, 4);
Adafruit_BME280 bme;

// arduino pin numbers
byte LED = 13;
byte SPEAKER = 8;
byte BUTTON = 7;

byte UNSAFETIME = 2; // the amount of seconds that is unsafe for there to be no human detection
short UNSAFETEMP = 75; // the temperature that is unsafe 

boolean humanDetection = false; // a variable that tells if there is human detection through the infrared sensor or button

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON,INPUT);
  Wire.begin();
  Serial.begin(9600);

  // doesn't continue if the BME cannot sense temperature properly
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor.");
    for(;;);
  }

  // doesn't continue if the OLED screen doesn't work
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED allocation failed"));
    for(;;);
  }

  // initializes the OLED screen
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  
  detectHuman();
  short temp = getTemp();
  screen(temp);

  if (temp >= UNSAFETEMP || !humanDetection) { // checks for an unsafe temperature or if there is no human detection
    int timeT = 0;
    for (int i = 0; i < UNSAFETIME; i++) { // alerts the user until time runs out or the user responds

      lightON();
      speakerON();
    
      detectHuman();
      temp = getTemp();
      screen(temp);
      
      if (humanDetection || temp < UNSAFETEMP)
        i = UNSAFETIME;

      timeT++;
      delay(1000);
    }

    lightOFF();
    speakerOFF(); // stops the alerts
    
  }

  while (temp < UNSAFETEMP || humanDetection) { // continues to update the OLED screen if the temperature is safe or there is human detection
    detectHuman();
    temp = getTemp();
    Serial.println("Temp: ");
    Serial.print(temp);
    Serial.print((char)247);
    Serial.print("F");-
    Serial.println();
    screen(temp);
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
 * Checks if there is human activity in the kitchen through if the button is being pressed
 */
void detectHuman() {

  if (digitalRead(BUTTON) == HIGH)
    humanDetection = true;
  
  else
    humanDetection = false;
}
