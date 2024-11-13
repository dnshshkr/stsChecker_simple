const uint8_t gains[] = {1, 4, 16, 60};
const uint8_t gainsLen = sizeof(gains) / sizeof(gains[0]);
void gainSetting() {
  Serial.println("[Settings/Gain]");
  for (uint8_t i = 0; i < gainsLen; i++)
    Serial.print(i + 1), Serial.print(": "), Serial.println(gains[i]);
  Serial.println("S: Back");
waitCmd_gainSetting:
  Serial.print("Selection: ");
  while (!Serial.available());
  char choice = toupper(Serial.readStringUntil('\n').charAt(0));
  Serial.println(choice);
  if (choice == 'S')
    return;
  if (choice - '0' < 1 || choice - '0' > gainsLen) {
    Serial.println("Invalid");
    goto waitCmd_gainSetting;
  }
  setGain(choice - '0' - 1);
  uint8_t gain = getGain(choice - '0' - 1);
  Serial.print("Gain set to "), Serial.println(gain);
  return;
}
uint8_t getGain(uint8_t index) {
  return gains[index];
}
void setGain(uint8_t index) {
  static const tcs34725Gain_t values[] = {TCS34725_GAIN_1X, TCS34725_GAIN_4X, TCS34725_GAIN_16X, TCS34725_GAIN_60X};
  if (index >= 0 && index < gainsLen)
    tcs.setGain(values[index]);
  else {
    tcs.setGain(TCS34725_GAIN_4X);
    index = 1;
  }
  EEPROM.update(gainAddr, index);
}
