struct Store
{
  char deviceId[64];
  char ssid[64];
  char password[64];
  int opMode;
  char projectId[64];
};

Store store;
uint addr = 0;

ESP8266WebServer server(80); // Create Web Server on Port 80

int sensorPin = A0;
int sensorValue;
