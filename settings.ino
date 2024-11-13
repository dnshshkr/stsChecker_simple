void settings() {
begin_settings:
  printInfo();
  Serial.println(F("[Settings]"));
  Serial.println(F("1: Channel 1"));
  Serial.println(F("2: Channel 2"));
  Serial.print(F("3: Relay Type: Active ")), relayType ? Serial.println(F("HIGH")) : Serial.println(F("LOW"));
  Serial.println(F("4: Reboot"));
  Serial.println(F("5: Factory Reset"));
  Serial.println(F("S: Exit"));
waitCmd_settings:
  Serial.print(F("Selection: "));
  while (!Serial.available());
  char cmd = Serial.readStringUntil('\n').charAt(0);
  Serial.println(cmd);
  cmd = toupper(cmd);
  switch (cmd) {
    case '1': case '2': {
        setRGB(cmd - '0' - 1);
        break;
      }
    case '3': {
        relayType = !relayType;
        EEPROM.update(relayTypeAddr, relayType);
        break;
      }
    case '4':
      reboot();
    case '5': {
        if (factoryReset())
          reboot();
        else
          break;
      }
    case 'S': case's':
      return;
    default: {
        Serial.println(F("Invalid"));
        goto waitCmd_settings;
      }
  }
  goto begin_settings;
}
