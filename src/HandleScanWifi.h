void handleScanWifi()
{
    DynamicJsonDocument doc(1024);
    doc["test"] = "ok";
    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}