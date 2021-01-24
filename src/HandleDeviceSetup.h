void handleDeviceSetup()
{
    if (server.hasArg("plain") == false)
    {
        server.send(502, "text/plain", "Body not received");
        return;
    }

    DynamicJsonDocument bodyData(1024);
    String reqBody = server.arg("plain");
    deserializeJson(bodyData, reqBody);
    String deviceId = bodyData["deviceId"];
    String projectId = bodyData["projectId"];

    if (deviceId && projectId)
    {
        char deviceId_Buf[64];
        char projectId_Buf[64];
        deviceId.toCharArray(deviceId_Buf, 64);
        projectId.toCharArray(projectId_Buf, 64);

        strcpy(store.deviceId, deviceId_Buf);
        strcpy(store.projectId, projectId_Buf);
        store.opMode = 2;

        EEPROM.put(addr, store);
        EEPROM.commit();
    }

    server.send(200, "text/plain", "Device ID set successfully.");
    Serial.println("Resetting ESP");
    delay(5000);
    ESP.restart();
}