#include <ESP8266WiFi.h>

IPAddress createAP(String ssid) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress ip = WiFi.softAPIP();
  return ip;
}
