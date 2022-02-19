// test BME280

#include "Seeed_BME280.h"
#include <Wire.h>

BME280 bme280;

void setup() {
    Serial.begin(115200);
    //if (!bme280.init(0x77)) {
    if (!bme280.init(0x76)) {
        Serial.println("Device error!");
    }
}

void loop() {
    float pressure;

    //get and print temperatures
    Serial.print("Temp: ");
    Serial.print(bme280.getTemperature());
    Serial.println(" C");//The unit for  Celsius because original arduino don't support special symbols

    //get and print atmospheric pressure data
    Serial.print("Pressure: ");
    Serial.print(pressure = bme280.getPressure()/100);
    Serial.println(" hPa");

    //get and print humidity data
    Serial.print("Humidity: ");
    Serial.print(bme280.getHumidity());
    Serial.println("%");

    delay(1000);
}
