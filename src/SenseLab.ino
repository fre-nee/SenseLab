#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "Store.h"
#include "checkOpMode.h"
#include "CreateAP.h"

/*
    opMode
    0: no data,
    1: default data stored on EEPROM,
    2: enable AP for set,
    3: send data to server
*/

//intial setup
void setup() {
  Serial.begin(9600);  //serial monitor
  EEPROM.begin(512);   // allocate space in EEPROM
  int oprationMode = checkOpMode();  //get cuurect opMode

  //if opMode is 1 create AP for initial device setup
  if (oprationMode == 1) {
    IPAddress ip = createAP(store.ssid);
    Serial.println("IP Adress ->" + ip);
  }
}

void loop() {
}
