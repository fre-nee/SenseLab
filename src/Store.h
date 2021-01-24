struct Store {
  char deviceId[64];
  char ssid[64];
  char password[64];
  int opMode;
  int projectId;
};

Store store;
uint addr = 0;
