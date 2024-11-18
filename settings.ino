void settings() {
begin_settings:
  printInfo();
  Serial.print(F("[Settings]\n1: Channel 1\n2: Channel 2\n3: Relay Type: Active "));
  relayType ? Serial.println(F("HIGH")) : Serial.println(F("LOW"));
  Serial.println(F("4: Reboot\n5: Factory Reset\nS: Exit\nTips: Attach an RGB LED to pins A0(R), A1(G), A2(B) to see color reproduction."));
waitCmd_settings:
  Serial.print(F("Selection: "));
  while (!Serial.available())
    ;
  char cmd = Serial.readStringUntil('\n').charAt(0);
  Serial.println(cmd);
  cmd = toupper(cmd);
  switch (cmd) {
    case '1':
    case '2':
      {
        setRGB(cmd - '0' - 1);
        break;
      }
    case '3':
      {
        relayType = !relayType;
        EEPROM.update(relayTypeAddr, relayType);
        break;
      }
    case '4':
      reboot();
    case '5':
      {
        if (factoryReset())
          reboot();
        else
          break;
      }
    case 'S':
    case 's':
      return;
    default:
      {
        Serial.println(F("Invalid"));
        goto waitCmd_settings;
      }
  }
  goto begin_settings;
}
