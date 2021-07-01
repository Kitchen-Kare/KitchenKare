//Import required libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

//Period between temperature scans
unsigned long delayTime;

void setup() {
    //Set up Serial Monitor (found under Tools in Menu)
    Serial.begin(9600);
    while(!Serial);    

    //Initialization
    Serial.println(F("BME280 test"));
    printValues();

    //If BME is not found, show error
    if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor.");
        while (1) delay(10);
    }

    //Begin Testing
    Serial.println("-- Default Test --");
    delayTime = 1000;
    printValues();
    Serial.println();
}


void loop() {
    //Run testing then delay and repeat 
    printValues();
    delay(delayTime);
}


void printValues() {
    //Prints Temperature
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    //Prints other Information
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
