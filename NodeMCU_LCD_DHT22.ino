/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
//-------------------------------LCD
#include <Wire.h> 
#include <LiquidCrystal.h> //ประกาศ Library ของจอ I2C
//----------------------------------
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "DHT.h"
#include <DHT_U.h>

#define DHTPIN            D7   
#define DHTTYPE           DHT22

LiquidCrystal lcd(D6,D5,D4,D3,D2,D1);
DHT_Unified dht(DHTPIN, DHTTYPE);
DHT dhtt(DHTPIN, DHTTYPE, 15);

char auth[] = ""; // Your TOKEN 

SimpleTimer timer;

void sendUptime()
{
    // Read temperature & Humidity
  float t = dhtt.readTemperature();
  float h = dhtt.readHumidity();
  
  Blynk.virtualWrite(V5, millis() / 1000);
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Blynk.virtualWrite(V1, event.temperature);
   }
  lcd.print("Temp: |Humidity:");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.print(" C | ");
  lcd.print(h);
   lcd.setCursor(0, 2);
   
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    Blynk.virtualWrite(V2, event.relative_humidity);
  }

}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "", ""); //Your SSID / Password
 
  dht.begin();
  lcd.begin(16,2);
  
  // Setup a function to be called every second
  timer.setInterval(5000L, sendUptime);
}

void loop()
{
  Blynk.run();
  timer.run();
}
