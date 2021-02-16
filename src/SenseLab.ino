#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Store.h"
#include "CheckOpMode.h"
#include "CreateAP.h"
#include "HandleNotFound.h"
#include "HandleRoot.h"
#include "HandleScanWifi.h"
#include "HandleDeviceSetup.h"
#include "HandleWifiSetup.h"
#include "ConnectWiFi.h"
#include "HandleReset.h"
#include "EasyButton.h"

#define ONE_WIRE_BUS 4
#define RESET_BUTTON_PIN 0

EasyButton reset_button(RESET_BUTTON_PIN);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int duration = 3000;

void onPressedForDuration()
{
  Serial.println("Button has been pressed for the given duration!");
}

/*
    opMode
    0: no data,
    1: default data stored on EEPROM,
    2: enable AP for set,
    3: send data to server
*/

//intial setup
void setup()
{
  Serial.begin(9600);               //serial monitor
  EEPROM.begin(512);                // allocate space in EEPROM
  int oprationMode = checkOpMode(); //get cuurect opMode

  //if opMode is 1 create AP for initial device setup
  if (oprationMode == 1)
  {
    IPAddress ip = createAP(store.ssid);
    Serial.println("SenseLab - IP Adress ->" + ip.toString());
  }

  if (oprationMode == 2)
  {
    IPAddress ip = createAP(store.deviceId);
    Serial.println(String(store.deviceId) + " - IP Adress ->" + ip.toString());
  }

  if (oprationMode == 3)
  {
    IPAddress ip = connectWiFi(store.ssid, store.password);
    Serial.println(String(store.ssid) + " - IP Adress ->" + ip.toString());
  }

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/scanwifi", handleScanWifi);
  server.on("/devicesetup", handleDeviceSetup);
  server.on("/wifisetup", handleWifiSetup);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);

  reset_button.begin();

  reset_button.onPressedFor(duration, handleReset);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  sensors.requestTemperatures(); 
  
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));
  
  sensorValue = analogRead(sensorPin);
  int moisture = ((1024 - sensorValue ) * 100) / 1024;
  Serial.print("moisture : ");
  Serial.print(moisture);
  Serial.println(" %");

  server.handleClient();
  MDNS.update();
  reset_button.read();
  delay(2000);
}
