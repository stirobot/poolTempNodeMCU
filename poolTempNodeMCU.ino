//gets pool temp from DS18B20 sensor and posts it to thinger.io

#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS D2     // what pin we're connected to
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

// Configuration of the WiFi AP settings.
#define WIFI_AP "apname"
#define WIFI_PASSWORD "apassword"


ThingerESP8266 thing("a", "a", "a");

void setup() {
  Serial.begin(9600);
  sensors.begin();
  thing.add_wifi(WIFI_AP, WIFI_PASSWORD);
  thing["poolTemp"] >> outputValue(sensors.getTempFByIndex(0));
  Serial.begin(9600);
}

bool endpointFlag = false;

void loop() {
  //read data
  thing.handle();
  sensors.requestTemperatures();
  //Serial.println(sensors.getTempFByIndex(0));
  if ( ( (sensors.getTempFByIndex(0) > 82) || (sensors.getTempFByIndex(0) < 75) ) && !endpointFlag){
    thing.call_endpoint("poolTempWarningEmail",thing["poolTemp"]);
  endpointFlag = true;
  }
}

