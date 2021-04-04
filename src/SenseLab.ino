#include <ESP8266MQTTClient.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HttpClient.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

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

//Status LED pins
#define RED D5
#define GREEN D6
#define BLUE D7

MQTTClient mqtt;

const char *mqtt_server = "mqtt-senselab.herokuapp.com";
// WiFiClient espClient;
// PubSubClient client(espClient);

EasyButton reset_button(RESET_BUTTON_PIN);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

StaticJsonDocument<400> masterData;
StaticJsonDocument<1000> requestArray;

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

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//intial setup
void setup()
{
  Serial.begin(9600);               //serial monitor
  EEPROM.begin(512);                // allocate space in EEPROM
  int oprationMode = checkOpMode(); //get cuurect opMode

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);

  //if opMode is 1 create AP for initial device setup
  if (oprationMode == 1)
  {
    IPAddress ip = createAP(store.ssid);
    Serial.println("SenseLab - IP Adress ->" + ip.toString());

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
  }

  if (oprationMode == 2)
  {
    IPAddress ip = createAP(store.deviceId);
    Serial.println(String(store.deviceId) + " - IP Adress ->" + ip.toString());

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
  }

  if (oprationMode == 3)
  {
    IPAddress ip = connectWiFi(store.ssid, store.password);
    Serial.println(String(store.ssid) + " - IP Adress ->" + ip.toString());
    delay(1000);

    // client.setServer(mqtt_server, 80);
    // client.connect(store.deviceId);
    // Serial.print("MQTT server -> ");
    // Serial.println(client.connected());
    // client.setCallback(callback);

    mqtt.onData([](String topic, String data, bool cont) {
      Serial.printf("Data received, topic: %s, data: %s\r\n", topic.c_str(), data.c_str());
    });

    mqtt.onConnect([]() {
      Serial.printf("MQTT: Connected\r\n");
      Serial.printf("Subscribe id: %d\r\n", mqtt.subscribe("/qos0", 0));
      mqtt.subscribe("/data", 1);
    });

    mqtt.onDisconnect([]() {
      Serial.printf("MQTT: Disconnected\r\n");
    });

    mqtt.begin("ws://mqtt-senselab.herokuapp.com:80");
    mqtt.connect();

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
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
  JsonArray array = requestArray.to<JsonArray>();

  JsonObject masterObject = masterData.to<JsonObject>();

  String postMessage;

  sensors.requestTemperatures();

  // Serial.print("Temperature is: ");
  // Serial.println(sensors.getTempCByIndex(0));

  sensorValue = analogRead(sensorPin);
  int moisture = ((sensorValue)*100) / 1024;
  // Serial.print("moisture : ");
  // Serial.print(moisture);
  // Serial.println(" %");

  int oprationMode = checkOpMode();

  if (oprationMode == 3)
  {
    masterObject["deviceId"] = store.deviceId;
    masterObject["moisture"] = moisture;
    masterObject["temprature"] = sensors.getTempCByIndex(0);
    // masterObject["soilPh"] = 0;
    // masterObject["solarRadiation"] = 0;
    // masterObject["airTemprature"] = 0;
    // masterObject["airHumidity"] = 0;
    // masterObject["aqi"] = 0;

    array.add(masterObject);

    char JSONmessageBuffer[2000];

    serializeJson(array, JSONmessageBuffer);
    // serializeJsonPretty(array, Serial);

    // http.begin("http://mitti-backend-monorepo.herokuapp.com/api/functions/dumpData");
    // http.addHeader("Content-Type", "application/json");
    // http.addHeader("X-Parse-Application-Id", "mitti-backend");
    // http.addHeader("X-Parse-REST-API-Key", "vMvvybc1z4*Q$!J*k4P4NNx");

    // int httpCode = http.POST(JSONmessageBuffer);
    // Serial.print("httpCode -> ");
    // Serial.println(httpCode);
    mqtt.publish(store.deviceId, JSONmessageBuffer);
  }

  mqtt.handle();
  server.handleClient();
  MDNS.update();
  reset_button.read();
}