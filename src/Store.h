struct Store {
  char deviceId[64];
  char ssid[64];
  char password[64];
  int opMode;
  int projectId;
};

Store store;
uint addr = 0;

ESP8266WebServer server(80); // Create Web Server on Port 80