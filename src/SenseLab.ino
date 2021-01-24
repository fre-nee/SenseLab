#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Store.h"
#include "CheckOpMode.h"
#include "CreateAP.h"
#include "HandleNotFound.h"
#include "HandleRoot.h"

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
    Serial.println("IP Adress ->" + ip.toString());
  }

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  MDNS.update();
}
