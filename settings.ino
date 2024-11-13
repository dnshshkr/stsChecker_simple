void settings() {
begin_settings:
  printInfo();
  Serial.println("[Settings]");
  Serial.println("1: Channel 1");
  Serial.println("2: Channel 2");
  Serial.print("3: Relay Type: Active "), relayType ? Serial.println("HIGH") : Serial.println("LOW");
  Serial.print("4: Baud Rate: "), Serial.print(getBaudRate(EEPROM.read(baudRateAddr))), Serial.println(" bps");
  Serial.print("5: Integration Time: "), Serial.print(getIntegTime(EEPROM.read(integTimeAddr))), Serial.println(" ms");
  Serial.print("6: Gain: "), Serial.print(getGain(EEPROM.read(gainAddr))), Serial.println("x");
  Serial.println("7: Reboot");
  Serial.println("8: Factory Reset");
  Serial.println("S: Exit");
waitCmd_settings:
  Serial.print("Selection: ");
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
    case '4': {
        if (baudRateSetting())
          reboot();
        else
          break;
      }
    case '5': {
        integrationTimeSetting();
        break;
      }
    case '6': {
        gainSetting();
        break;
      }
    case '7':
      reboot();
    case '8': {
        if (factoryReset())
          reboot();
        else
          break;
      }
    case 'S': case's':
      return;
    default: {
        Serial.println("Invalid");
        goto waitCmd_settings;
      }
  }
  goto begin_settings;
}
