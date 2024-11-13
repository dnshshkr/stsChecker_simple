const float integTimes[] = {2.4, 24.0, 50.0, 60.0, 101.0, 120.0, 154.0, 180.0, 199.0, 240.0, 300.0, 360.0, 401.0, 420.0, 480.0, 499.0, 540.0, 600.0, 614.0};
const uint8_t integTimesLen = sizeof(integTimes) / sizeof(integTimes[0]);
void integrationTimeSetting() {
  Serial.println("[Settings/Integration TIme]");
  for (uint8_t i = 0; i < integTimesLen; i++)
    Serial.print(i + 1), Serial.print(": "), Serial.println(integTimes[i]);
  Serial.println("S: Back");
waitCmd_integrationTimeSetting:
  Serial.print("Selection: ");
  while (!Serial.available());
  String choiceStr = Serial.readStringUntil('\n');
  Serial.println(choiceStr);
  choiceStr.toUpperCase();
  if (choiceStr == "S")
    return;
  uint8_t choice = choiceStr.toInt();
  if (choice < 1 || choice > integTimesLen) {
    Serial.println("Invalid");
    goto waitCmd_integrationTimeSetting;
  }
  setIntegTime(choice - 1);
  float integTime = getIntegTime(choice - 1);
  Serial.print("Integration time set to "), Serial.println(integTime);
}
float getIntegTime(uint8_t index) {
  return integTimes[index];
}
void setIntegTime(uint8_t index) {
  static const uint8_t values[] = {
    TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_INTEGRATIONTIME_24MS, TCS34725_INTEGRATIONTIME_50MS, TCS34725_INTEGRATIONTIME_60MS, TCS34725_INTEGRATIONTIME_101MS, TCS34725_INTEGRATIONTIME_120MS,
    TCS34725_INTEGRATIONTIME_154MS, TCS34725_INTEGRATIONTIME_180MS, TCS34725_INTEGRATIONTIME_199MS, TCS34725_INTEGRATIONTIME_240MS, TCS34725_INTEGRATIONTIME_300MS, TCS34725_INTEGRATIONTIME_360MS,
    TCS34725_INTEGRATIONTIME_401MS, TCS34725_INTEGRATIONTIME_420MS, TCS34725_INTEGRATIONTIME_480MS, TCS34725_INTEGRATIONTIME_499MS, TCS34725_INTEGRATIONTIME_540MS, TCS34725_INTEGRATIONTIME_600MS,
    TCS34725_INTEGRATIONTIME_614MS
  };
  if (index >= 0 && index < integTimesLen)
    tcs.setIntegrationTime(values[index]);
  else {
    tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
    index = 2;
  }
  EEPROM.update(integTimeAddr, index);
}
