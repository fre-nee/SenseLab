void handleWifiSetup()
{
    if (server.hasArg("plain") == false)
    {
        server.send(400, "text/plain", "Body not received");
        return;
    }

    DynamicJsonDocument bodyData(1024);
    String reqBody = server.arg("plain");
    deserializeJson(bodyData, reqBody);
    String ssid = bodyData["ssid"];
    String password = bodyData["password"];

    if (ssid && password)
    {
        char ssid_Buf[64];
        char password_Buf[64];
        ssid.toCharArray(ssid_Buf, 64);
        password.toCharArray(password_Buf, 64);

        strcpy(store.ssid, ssid_Buf);
        strcpy(store.password, password_Buf);
        store.opMode = 3;

        EEPROM.put(addr, store);
        EEPROM.commit();
    }

    server.send(200, "text/plain", "WiFi set successfully.");
    Serial.println("Resetting ESP");
    delay(5000);
    ESP.restart();
}