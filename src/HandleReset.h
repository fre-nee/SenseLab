void handleReset()
{
    char ssid[64] = "";
    char password[64] = "";

    strcpy(store.ssid, ssid);
    strcpy(store.password, password);
    store.opMode = 2;

    EEPROM.put(addr, store);
    EEPROM.commit();
    server.send(200, "text/plain", "Device reset set successfully.");
    Serial.println("Resetting ESP");
    delay(5000);
    ESP.restart();
}