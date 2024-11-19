void settings() {
begin_settings:
  printInfo();
  Serial.print(F("[Settings]\n1: Channel 1\n2: Channel 2\n3: Relay Type: Active "));
  relayType ? Serial.println(F("HIGH")) : Serial.println(F("LOW"));
  Serial.print(F("4: Reproduce Color: "));
  repClr ? Serial.println(F("YES")) : Serial.println(F("NO"));
  if (repClr) {
    Serial.print(F("5: RGB LED Common Pin: "));
    rgbCom ? Serial.println(F("Anode")) : Serial.println(F("Cathode"));
  }
  Serial.print(F("R: Reboot\nF: Factory Reset\nS: Exit"));
  repClr ? Serial.println(F("\nTips: Attach an RGB LED to pins A0(R), A1(G), A2(B) to see color reproduction.")) : Serial.println();
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
      {
        repClr = !repClr;
        EEPROM.update(repClrAddr, repClr);
        break;
      }
    case '5':
      {
        if (!repClr)
          goto defaultLabel;
        rgbCom = !rgbCom;
        EEPROM.update(rgbComAddr, rgbCom);
        generateGammatable();
        break;
      }
    case 'R':
    case 'r':
      reboot();
    case 'F':
    case 'f':
      {
        if (factoryReset())
          reboot();
        else
          break;
      }
    case 'S':
    case 's':
      return;
defaultLabel:
    default:
      {
        Serial.println(F("Invalid"));
        goto waitCmd_settings;
      }
  }
  goto begin_settings;
}
