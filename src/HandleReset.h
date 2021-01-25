void handleReset()
{
    char ssid[64] = "";
    char password[64] = "";

    strcpy(store.ssid, ssid);
    strcpy(store.password, password);
    store.opMode = 2;

    EEPROM.put(addr, store);
    EEPROM.commit();
    Serial.println("Device reset set successfully.");
    Serial.println("Resetting device in 5 sec.");
    delay(5000);
    ESP.restart();
}