void setRGB(uint8_t ch) {
  static const String validCmd[] = { "S", "C", "LR", "HR", "LG", "HG", "LB", "HB" };
  static const uint8_t validCmdLen = sizeof(validCmd) / sizeof(validCmd[0]);
  bool sensorInitialized = tcs.init();
  uint8_t valsDict[2][chSize];
begin_setRGB:
  getVals();
  for (uint8_t i = 0; i < 2; i++) {
    for (uint8_t j = 0; j < chSize; j++)
      valsDict[i][j] = rgbDict[ch][i][j];
  }
  Serial.print(F("[Settings/Channel ")), Serial.print(ch + 1), Serial.println(F("]"));
  Serial.print(F("LR: ")), Serial.println(valsDict[1][0]);
  Serial.print(F("HR: ")), Serial.println(valsDict[1][1]);
  Serial.print(F("LG: ")), Serial.println(valsDict[1][2]);
  Serial.print(F("HG: ")), Serial.println(valsDict[1][3]);
  Serial.print(F("LB: ")), Serial.println(valsDict[1][4]);
  Serial.print(F("HB: ")), Serial.println(valsDict[1][5]);
  Serial.println(F("C: Self-calibrate\nS: Back"));
waitCmd_setRGB:
  Serial.print(F("Selection: "));
  while (!Serial.available())
    ;
  String color = Serial.readStringUntil('\n');
  Serial.println(color);
  color.toUpperCase();
  bool isValid = false;
  for (uint8_t i = 0; i < validCmdLen; i++) {
    if (color == validCmd[i]) {
      isValid = true;
      break;
    }
  }
  if (!isValid) {
    Serial.println(F("Invalid"));
    goto waitCmd_setRGB;
  }
  if (color == validCmd[0])  //  S
    return;
  else if (color == validCmd[1]) {  //  C
    if (sensorInitialized) {
      selfCalibrate(valsDict[0]);
      goto begin_setRGB;
    } else {
      Serial.println(F("Sensor is not initialized, unable to calibrate."));
      goto waitCmd_setRGB;
    }
  }
  uint8_t minVal, maxVal;
  getMinMax(valsDict[1], color, minVal, maxVal);
  String valStr;
  int val;
  while (true) {
    Serial.print(F("Insert value for ")), Serial.print(color), Serial.print(F(" (")), Serial.print(minVal), Serial.print(F("-")), Serial.print(maxVal), Serial.print(F("), or \"S\" to cancel: "));
    while (!Serial.available())
      ;
    valStr = Serial.readStringUntil('\n');
    Serial.println(valStr);
    if (toupper(valStr.charAt(0)) == 'S')
      goto begin_setRGB;
    val = valStr.toInt();
    if (val < minVal || val > maxVal) {
      Serial.println(F("Value out of range."));
      continue;
    } else
      break;
  }
  if (color == "LR")
    EEPROM.update(valsDict[0][0], val);
  else if (color == "HR")
    EEPROM.update(valsDict[0][1], val);
  else if (color == "LG")
    EEPROM.update(valsDict[0][2], val);
  else if (color == "HG")
    EEPROM.update(valsDict[0][3], val);
  else if (color == "LB")
    EEPROM.update(valsDict[0][4], val);
  else
    EEPROM.update(valsDict[0][5], val);
  goto begin_setRGB;
}
void getMinMax(uint8_t *vals, String color, uint8_t &minVal, uint8_t &maxVal) {
  const uint8_t absMin = 0, absMax = 255;
  if (color == "LR") {
    minVal = absMin;
    maxVal = vals[1];
  } else if (color == "HR") {
    minVal = vals[0];
    maxVal = absMax;
  } else if (color == "LG") {
    minVal = absMin;
    maxVal = vals[3];
  } else if (color == "HG") {
    minVal = vals[2];
    maxVal = absMax;
  } else if (color == "LB") {
    minVal = absMin;
    maxVal = vals[5];
  } else {
    minVal = vals[4];
    maxVal = absMax;
  }
}
