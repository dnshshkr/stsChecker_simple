void getVals() {
  for (uint8_t i = 0; i < chSize; i++) {
    rgbDict[0][1][i] = EEPROM.read(rgbDict[0][0][i]);
    rgbDict[1][1][i] = EEPROM.read(rgbDict[1][0][i]);
  }
}