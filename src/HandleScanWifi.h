void handleScanWifi()
{
  int nNetworks = WiFi.scanNetworks(false, true);
  DynamicJsonDocument wifiNetworks(1024);
  Serial.printf("%d network(s) found\n", nNetworks);
  for (int i = 0; i < nNetworks; i++)
  {
    String ssid;
    uint8_t encryptionType;
    int32_t RSSI;
    uint8_t* BSSID;
    int32_t channel;
    bool isHidden;

    WiFi.getNetworkInfo(i, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
    DynamicJsonDocument wifi(1024);
    wifi["ssid"] = ssid.c_str();
    wifi["channel"] = channel;
    wifi["rssi"] = WiFi.RSSI(i);
    wifi["open"] = encryptionType == ENC_TYPE_NONE;
    String wifiString;
    serializeJson(wifi, wifiString);
    wifiNetworks[String(i)] = wifiString;
  }
  String res;
  serializeJson(wifiNetworks, res);
  server.send(200, "application/json", res);
}