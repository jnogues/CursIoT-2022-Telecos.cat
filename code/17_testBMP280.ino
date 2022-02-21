//test bmp280

#include "Seeed_BMP280.h"
#include <Wire.h>

BMP280 bmp280;
#define INTERVAL 2000
double t0;
uint32_t MSL = 102009; // Mean Sea Level in Pa

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n\nBMP280 test");
  //if (!bmp280.init(0x77)) {
  if (!bmp280.init(0x76)) {
    Serial.println("Device not connected or broken!");
  }
}

void loop() 
{
  double t1 = millis();
  if (t1 - t0 > INTERVAL) {
    float p1;

    //get and print temperatures
    Serial.print("Temp: ");
    float t = bmp280.getTemperature();
    Serial.print(t);
    Serial.println(" C");
    Serial.print("Pres: ");
    p1 = bmp280.getPressure();
    Serial.print(p1 / 100.0);
    Serial.println(" HPa");

    t0 = millis();
  }

}
