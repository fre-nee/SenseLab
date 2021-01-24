#include <EEPROM.h>

int checkOpMode() {
  EEPROM.get(addr, store);   //get config
  int oprationMode = store.opMode;

  //if opteation mode is 0 set default config
  if (!oprationMode) {
    Store defultStore = {
      "",             // default deviceId
      "SenseLab",     // default SSID
      "",             // default password
      1,              // opMode
      ""               // projectId
    };
    EEPROM.put(addr, defultStore);
    EEPROM.commit();
  }
  EEPROM.get(addr, store);
  oprationMode = store.opMode;
  return oprationMode;
}
