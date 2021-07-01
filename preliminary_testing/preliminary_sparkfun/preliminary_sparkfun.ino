//Imports required libraries
#include <SparkFun_GridEYE_Arduino_Library.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

// Use these values (in degrees C) to adjust the contrast
byte HOT;
byte COLD;
// These are default values and will be changed later

int pixelTable[64];
int comparisonTable[64];

GridEYE grideye;

void setup() {
  
  // Start your preferred I2C object 
  Wire.begin();
  // Library assumes "Wire" for I2C but you can pass something else with begin() if you like
  grideye.begin();
  // Pour a bowl of serial
  Serial.begin(9600);
  // Start printing
  Serial.println(F("Kitchen Kare Sparkfun test"));

  bool status;

  // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }

  //Initial message
  Serial.println("-- Initializing Temperature --");
  Serial.println();
  delay(100); // let sensor boot up

  //Declare COLD temp
  Serial.print("Sensor COLD Temperature = ");
  Serial.print(amg.readThermistor()-3);
  Serial.println(" *C");
  COLD = amg.readThermistor()-3;

  //Declare HOT temp
  Serial.print("Sensor HOT Temperature = ");
  Serial.print(amg.readThermistor()+3);
  Serial.println(" *C");
  HOT = amg.readThermistor()+3;

  // Loop through all 64 pixels on the device and map each float value to a number
  // Between 0 and 3 using the HOT and COLD values we set at the top of the sketch
  for(unsigned char i = 0; i < 64; i++){
    comparisonTable[i] = map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3);
  }

}

void loop() {
  
  // Loop through all 64 pixels on the device and map each float value to a number
  // Between 0 and 3 using the HOT and COLD values we set at the top of the sketch
  for(unsigned char i = 0; i < 64; i++){
    pixelTable[i] = map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3);
    delay(1);
  }


  // Loop through the table of mapped values and print a character corresponding to each
  // Pixel's temperature. Add a space between each. Start a new line every 8 in order to 
  // Create an 8x8 grid
  Serial.println("Sensing...");
  for(unsigned char i = 0; i < 64; i++){
    if(pixelTable[i]==0){Serial.print(".");}
    else if(pixelTable[i]==1){Serial.print("o");}
    else if(pixelTable[i]==2){Serial.print("0");}
    else if(pixelTable[i]==3){Serial.print("O");}
    Serial.print(" ");
    if((i+1)%8==0){
      Serial.println();
    }
  }

  // In between updates, throw a few linefeeds to visually separate the grids. If you're using
  // A serial terminal outside the Arduino IDE, you can replace these linefeeds with a clearscreen
  // command
  Serial.println();
  Serial.println();

  // Toss in a delay because we don't need to run all out
  delay(5);

  // Display what's the change between loops
  Serial.println("Difference Count: ");
  Serial.print(count());
  if (count() > 8) 
    Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  else if (count() > 2)
    Serial.println("----------------------------------------------------");
}

//Compare old count to new count to measure change
byte count() {
  byte count = 0;
  for (unsigned char i=0; i<64; i++) {
    if (!pixelTable[i] == map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3))
      count++;
  }
  return count;
}
