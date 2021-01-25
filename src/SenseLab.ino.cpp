# 1 "/var/folders/m5/n46sd_xs7rd50060l67627p00000gn/T/tmpuroui8cf"
#include <Arduino.h>
# 1 "/Users/ineelpatel/Documents/PlatformIO/Projects/SenseLab/src/SenseLab.ino"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
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

#define RESET_BUTTON_PIN 0
EasyButton reset_button(RESET_BUTTON_PIN);
int duration = 3000;
void onPressedForDuration();
void setup();
void loop();
#line 23 "/Users/ineelpatel/Documents/PlatformIO/Projects/SenseLab/src/SenseLab.ino"
void onPressedForDuration() {
    Serial.println("Button has been pressed for the given duration!");
}
# 37 "/Users/ineelpatel/Documents/PlatformIO/Projects/SenseLab/src/SenseLab.ino"
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  int oprationMode = checkOpMode();


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
  server.handleClient();
  MDNS.update();
  reset_button.read();
}