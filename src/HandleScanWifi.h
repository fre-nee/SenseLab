void onComplete(int networksFound)
{
  DynamicJsonDocument doc(1024);
  Serial.printf("%d network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    DynamicJsonDocument wifi(1024);
    wifi["ssid"] = WiFi.SSID(i).c_str();
    wifi["channel"] = WiFi.channel(i);
    wifi["rssi"] = WiFi.RSSI(i);
    wifi["open"] = WiFi.encryptionType(i) == ENC_TYPE_NONE;
    String wifiString;
    serializeJson(wifi, wifiString);
    doc[String(i)] = wifiString;
  }
  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void handleScanWifi()
{
  WiFi.scanNetworks(onComplete);
}